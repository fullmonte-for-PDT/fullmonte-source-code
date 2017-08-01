/*
 * InternalSurfaceScorer.hpp
 *
 *  Created on: Sep 28, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_LOGGER_DIRECTEDSURFACESCORER_HPP_
#define KERNELS_SOFTWARE_LOGGER_DIRECTEDSURFACESCORER_HPP_

#include "AbstractScorer.hpp"
#include "BaseLogger.hpp"
#include "AtomicMultiThreadAccumulator.hpp"

#include "../Tetra.hpp"

#include <FullMonteSW/OutputTypes/SpatialMap.hpp>
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>

#include <FullMonteSW/Kernels/Software/Packet.hpp>
#include <FullMonteSW/Kernels/Kernel.hpp>

#include <FullMonteSW/Geometry/FaceLinks.hpp>



/** Scores the directed fluence passing through an interior surface
 *
 * Relies on a flag set in the kernel Tetra; for the i'th face of the tetra, fluence will be scored if (flags>>(i<<3)) & 0x01,
 * ie. 32b value treated as 4 8b concatenated values with LSB setting fluence accumulation.
 */

template<class AccumulatorT>class DirectedSurfaceScorer : public AbstractScorer
{
public:
	DirectedSurfaceScorer();
	~DirectedSurfaceScorer();

	typedef AccumulatorT Accumulator;

	/// Scorer required overrides
	virtual void prepare(const Kernel* K) override;
	virtual void clear() override;
	virtual void postResults(OutputDataCollection* C) const override;

	Accumulator& accumulator(){ return m_accumulator; }

	class Logger : public BaseLogger
	{
	public:
		Logger(typename Accumulator::ThreadHandle&& h) : m_handle(std::move(h)){}
		inline void eventNewTetra(AbstractScorer& S,const Packet& pkt,const Tetra& tet,const unsigned idx)
		{
			auto& DS = static_cast<DirectedSurfaceScorer&>(S);

			unsigned IDf=tet.IDfds[idx];
			if ((tet.faceFlags >> (idx << 3)) & 0x1)		// check if LSB of faceFlags is set for this face
				m_handle.accumulate(DS.m_accumulator,IDf,pkt.weight());
		}

		// commit is a no-op since it accumulates atomically as it goes
		void eventCommit(AbstractScorer& S)
			{ m_handle.commit(static_cast<DirectedSurfaceScorer&>(S).m_accumulator); }

		// clear is a no-op since the logger itself holds no state
		void eventClear(AbstractScorer& S)
			{ m_handle.clear(); }
	private:
		typename Accumulator::ThreadHandle m_handle;
	};


	Logger createLogger();

private:
	Accumulator m_accumulator;
};



template<class AccumulatorT>DirectedSurfaceScorer<AccumulatorT>::DirectedSurfaceScorer()
{
}

template<class AccumulatorT>DirectedSurfaceScorer<AccumulatorT>::~DirectedSurfaceScorer()
{
}

template<class AccumulatorT>void DirectedSurfaceScorer<AccumulatorT>::postResults(OutputDataCollection *C) const
{
	std::vector<float> se(m_accumulator.size());

	for(unsigned i=0;i<m_accumulator.size();++i)
		se[i] = m_accumulator[i];

	auto m = new SpatialMap<float>(se,AbstractSpatialMap::DirectedSurface,AbstractSpatialMap::UnknownValueType);
	m->name("DirectedSurfaceEnergy");

	C->add(m);
}

template<class AccumulatorT>void DirectedSurfaceScorer<AccumulatorT>::prepare(const Kernel* K)
{
	const TetraMesh* M = dynamic_cast<const TetraMesh*>(K->geometry());

	if(!M)
		throw std::logic_error("DirectedSurfaceScorer::prepare() invalid cast to TetraMesh");

	m_accumulator.resize(get(num_directed_faces,*M));
}

template<class AccumulatorT>void DirectedSurfaceScorer<AccumulatorT>::clear()
{
	m_accumulator.clear();
}

template<class AccumulatorT>typename DirectedSurfaceScorer<AccumulatorT>::Logger DirectedSurfaceScorer<AccumulatorT>::createLogger()
{
	return Logger(m_accumulator.createThreadHandle());
}



#endif /* KERNELS_SOFTWARE_LOGGER_DIRECTEDSURFACESCORER_HPP_ */
