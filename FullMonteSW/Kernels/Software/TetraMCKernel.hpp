/*
 * TetraMCKernel.hpp
 *
 *  Created on: Feb 1, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_TETRAMCKERNEL_HPP_
#define KERNELS_SOFTWARE_TETRAMCKERNEL_HPP_

#include <FullMonteSW/Geometry/Sources/Print.hpp>
#include <FullMonteSW/Geometry/MaterialSet.hpp>
#include <FullMonteSW/Geometry/Material.hpp>

#include "Emitters/Base.hpp"			//	TODO: Only here until a Layered factory exists
#include "Emitters/Point.hpp"
#include "Emitters/Directed.hpp"

#include <FullMonteSW/Geometry/Partition.hpp>

#include "Logger/AbstractScorer.hpp"

#include "Tetra.hpp"
#include "Material.hpp"

#include <FullMonteSW/Kernels/Software/ThreadedMCKernelBase.hpp>
#include <FullMonteSW/Kernels/Software/Emitters/TetraMeshEmitterFactory.cpp>

#include "TetraMCKernel.hpp"

#include "TetrasFromLayered.hpp"
#include "TetrasFromTetraMesh.hpp"

#include <FullMonteSW/Geometry/Layer.hpp>
#include <FullMonteSW/Geometry/Layered.hpp>


class Partition;
class TetraMesh;

#include <boost/align/aligned_alloc.hpp>
#include "Logger/tuple_for_each.hpp"

#include "RNG_SFMT_AVX.hpp"

template<class RNGT,class ScorerT>class TetraMCKernel : public ThreadedMCKernelBase
{
public:
	TetraMCKernel()
	{
		tuple_for_each(m_scorer, [this](AbstractScorer& s){ addScorer(&s); });
	}

	typedef RNGT										RNG;
	class 												Thread;
	typedef ScorerT 									Scorer;

#ifndef SWIG
	typedef decltype(createLogger(*static_cast<ScorerT*>(nullptr))) 	Logger;
#endif

protected:
	virtual ThreadedMCKernelBase::Thread* makeThread() final override;

	virtual void parentPrepare() 						override;

	std::vector<x86Kernel::Material> 		m_mats;

	Emitter::EmitterBase<RNG>* 				m_emitter;

	Scorer									m_scorer;
	std::vector<Tetra>						m_tetras;
};

template<class RNGT,class Scorer>void TetraMCKernel<RNGT,Scorer>::parentPrepare()
{
	////// Geometry setup

	const Geometry* G = geometry();

	if (!G)
		throw std::logic_error("TetraMCKernel<RNG,Scorer>::parentPrepare() no geometry specified");

	if (const TetraMesh* M = dynamic_cast<const TetraMesh*>(G))
	{
		cout << "Building kernel tetras from TetraMesh" << endl;
		TetrasFromTetraMesh TF;

		TF.mesh(M);
		TF.setFacesForFluenceCounting(M->internalFacesForFluenceCounting());
		TF.update();

		m_tetras = TF.tetras();


		if (!m_src)
			throw std::logic_error("TetraMCKernel<RNGT,Scorer>::parentPrepare() no sources specified");

		// TODO: Move this out of here as it doesn't quite belong
		Emitter::TetraEmitterFactory<RNGT> factory(M);

		((Source::Abstract*)m_src)->acceptVisitor(&factory);

		m_emitter = factory.emitter();

		cout << "Sources set up" << endl;
	}
	else if (const Layered* L = dynamic_cast<const Layered*>(G))
	{
		cout << "Building kernel tetras from Layered geometry" << endl;
		TetrasFromLayered TL;

		TL.layers(L);
		TL.update();

		m_tetras = TL.tetras();

		// TODO: Move this out of here as it doesn't quite belong
		if (m_src)
			cout << "TetraMCKernel::parentPrepare() ignoring provided source with Layered geometry (only PencilBeam allowed)" << endl;


		SSE::UnitVector3 d{{ 0.0f, 0.0f, 1.0f}};
		SSE::UnitVector3 a{{ 1.0f, 0.0f, 0.0f }};
		SSE::UnitVector3 b{{ 0.0f, 1.0f, 0.0f }};

		array<float,3> origin{{0.0f,0.0f,0.0001f}};

		Emitter::Point P{1U,SSE::Vector3(origin.data())};
		Emitter::Directed D(PacketDirection(d,a,b));

		m_emitter = new Emitter::PositionDirectionEmitter<RNGT,Emitter::Point,Emitter::Directed>(P,D);
	}
	else
		throw std::logic_error("TetraMCKernel<RNG,Scorer>::parentPrepare() geometry is neither Layered nor TetraMesh");

	cout << "Done with " << m_tetras.size() << " tetras" << endl;





	////// Material setup

	if (!materials())
		throw std::logic_error("TetraMCKernel<RNGT,Scorer>::parentPrepare() no materials specified");

	m_mats.resize(materials()->size());

	// copy materials
	for(unsigned i=0;i<materials()->size();++i)
	{
		::Material* m = materials()->get(i);
		m_mats[i] = x86Kernel::Material(m->absorptionCoeff(), m->scatteringCoeff(),m->refractiveIndex());

//		cout << "Material [" << i << "] muA=" << m->absorptionCoeff() << " muS=" << m->scatteringCoeff() << " g=" << m->anisotropy() << " n=" << m->refractiveIndex() << endl;
	}

	cout << "Materials ok" << endl;

	resetSeedRng();
}


template<class RNGT,class Scorer>ThreadedMCKernelBase::Thread* TetraMCKernel<RNGT,Scorer>::makeThread()
{
	void *p = boost::alignment::aligned_alloc(32,sizeof(typename TetraMCKernel<RNG_SFMT_AVX,Scorer>::Thread));

	if (!p)
	{
		cerr << "Allocation failure in TetraMCKernel<RNGT,Scorer>::makeThread()" << endl;
		throw std::bad_alloc();
	}

	// create the thread-local state

	typename TetraMCKernel<RNG_SFMT_AVX,Scorer>::Thread* t = new (p) typename TetraMCKernel<RNG_SFMT_AVX,Scorer>::Thread(*this,createLogger(m_scorer));

	// seed its RNG
	t->seed(TetraMCKernel<RNG_SFMT_AVX,Scorer>::getUnsignedRNGSeed());

	return t;
}

#endif /* KERNELS_SOFTWARE_TETRAMCKERNEL_HPP_ */


