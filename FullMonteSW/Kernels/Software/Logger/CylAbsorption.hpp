/*
 * CylAbsorption.hpp
 *
 *  Created on: Oct 30, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_LOGGER_CYLABSORPTION_HPP_
#define KERNELS_SOFTWARE_LOGGER_CYLABSORPTION_HPP_

#include "AbstractScorer.hpp"
#include <cmath>

#include "BaseLogger.hpp"

#include "CylAbsorption.hpp"

#include "AtomicMultiThreadAccumulator.hpp"

#include <FullMonteSW/OutputTypes/SpatialMap2D.hpp>
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>
#include <FullMonteSW/OutputTypes/OutputData.hpp>

#include <FullMonteSW/Kernels/Kernel.hpp>

#include <FullMonteSW/Geometry/Layered.hpp>


/** Logs absorption events using a cylindrical coordinate system like MCML.
 *
 *
 * An absorption to (x,y,z) is scored in cylindrical bins using (r = sqrt(x2+y2),z)
 */

template<class AccumulatorT=AtomicMultiThreadAccumulator<double,double>>class CylAbsorptionScorer : public AbstractScorer
{
public:
	CylAbsorptionScorer();
	virtual ~CylAbsorptionScorer();


	////// Concrete Scorer override requirements

	virtual void postResults(OutputDataCollection* ) const override;
	virtual void clear() override;
	virtual void prepare(const Kernel* k);



	////// Scorer concept requirements

	class Logger;
	Logger createLogger();



	////// Configuration
	typedef AccumulatorT Accumulator;
	Accumulator& accumulator(){ return m_acc; }


private:
	float			m_dr=0.01f;
	float			m_dz=0.01f;
	unsigned		m_Nr=100;
	unsigned		m_Nz=100;
	Accumulator		m_acc;
};

template<class Accumulator>class CylAbsorptionScorer<Accumulator>::Logger: public BaseLogger
{
public:
	typedef CylAbsorptionScorer Scorer;

	Logger();
	~Logger();

	Logger(const Logger&) = default;
	Logger(Logger&&) = default;

	inline void eventAbsorb(AbstractScorer& S,Point3 p,unsigned IDt,double w0,double dw)
		{
			CylAbsorptionScorer<Accumulator>& CS = static_cast<CylAbsorptionScorer<Accumulator>&>(S);
			float r = std::sqrt(p[0]*p[0] + p[1]*p[1]);
			float z = p[2];

			unsigned ri = std::min(unsigned(std::floor(r/CS.m_dr)), CS.m_Nr-1);
			unsigned zi = std::min(unsigned(std::floor(z/CS.m_dz)), CS.m_Nz-1);

			m_handle.accumulate(CS.m_acc,ri*CS.m_Nz+zi,dw);
		}

	void eventClear(AbstractScorer& S);
	void eventCommit(AbstractScorer& S);

private:
	typename Accumulator::ThreadHandle m_handle;
};


template<class Accumulator>CylAbsorptionScorer<Accumulator>::CylAbsorptionScorer()
{
}

template<class Accumulator>CylAbsorptionScorer<Accumulator>::~CylAbsorptionScorer()
{
}

template<class Accumulator>void CylAbsorptionScorer<Accumulator>::postResults(OutputDataCollection* C) const
{
	std::vector<float> vf(m_acc.size());

	for(unsigned i=0;i<m_acc.size();++i)
		vf[i] = m_acc[i];

	SpatialMap<float> *vmap = new SpatialMap2D<float>(m_Nr,m_Nz,std::move(vf),AbstractSpatialMap::Volume,AbstractSpatialMap::Scalar);
	vmap->name("CylAbsorbedEnergy");

	C->add(vmap);
}

template<class Accumulator>void CylAbsorptionScorer<Accumulator>::clear()
{
	m_acc.clear();
}

template<class Accumulator>void CylAbsorptionScorer<Accumulator>::prepare(const Kernel* K)
{
	if (!K)
		throw std::logic_error("CylAbsorptionScorer::prepare(K) no kernel provided");

	const Geometry* G = K->geometry();

	if (!G)
		throw std::logic_error("CylAbsorptionScorer::prepare(K) no geometry provided");

	const Layered* L = dynamic_cast<const Layered*>(G);

	if (!L)
		throw std::logic_error("CylAbsorptionScorer::prepare(K) cannot cast geometry to Layered*");

	m_Nr = L->dims().ri;
	m_Nz = L->dims().zi;

	m_dr = L->resolution().r;
	m_dz = L->resolution().z;

	m_acc.resize(m_Nr*m_Nz);
}

template<class Accumulator>typename CylAbsorptionScorer<Accumulator>::Logger CylAbsorptionScorer<Accumulator>::createLogger()
{
	return CylAbsorptionScorer::Logger();
}





template<class Accumulator>CylAbsorptionScorer<Accumulator>::Logger::Logger()
{
}

template<class Accumulator>CylAbsorptionScorer<Accumulator>::Logger::~Logger()
{
}

template<class Accumulator>void CylAbsorptionScorer<Accumulator>::Logger::eventClear(AbstractScorer& S)
{
	m_handle.clear();
}


template<class Accumulator>void CylAbsorptionScorer<Accumulator>::Logger::eventCommit(AbstractScorer& S)
{
 	m_handle.commit(static_cast<Scorer&>(S).m_acc);
}


#endif /* KERNELS_SOFTWARE_LOGGER_CYLABSORPTION_HPP_ */
