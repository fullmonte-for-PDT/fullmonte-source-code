/*
 * MCMLKernel.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_MCMLKERNEL_HPP_
#define KERNELS_SOFTWARE_MCMLKERNEL_HPP_

#include "TetraMCKernel.hpp"
#include "RNG_SFMT_AVX.hpp"

#include "TetraMCKernelThread.hpp"

#include "Logger/LoggerTuple.hpp"
#include "Logger/CylAbsorption.hpp"
#include "Logger/ConservationScorer.hpp"
#include "Logger/EventScorer.hpp"
#include "Logger/PathScorer.hpp"

#include "Logger/AtomicMultiThreadAccumulator.hpp"
#include "Logger/QueuedMultiThreadAccumulator.hpp"

typedef std::tuple<
		EventScorer,
		ConservationScorer,
		CylAbsorptionScorer<AtomicMultiThreadAccumulator<double,double>>
		>
		MCMLScorerPack;

typedef std::tuple<
		EventScorer,
		ConservationScorer,
		CylAbsorptionScorer<QueuedMultiThreadAccumulator<double,double>>
		>
		MCMLScorerPackQ;

class MCMLKernel : public TetraMCKernel<RNG_SFMT_AVX,MCMLScorerPack>
{
public:
	MCMLKernel(){}

	CylAbsorptionScorer<AtomicMultiThreadAccumulator<double,double>>& cylAbsorptionScorer(){ return get<2>(m_scorer); }
};

class MCMLKernelQ : public TetraMCKernel<RNG_SFMT_AVX,MCMLScorerPackQ>
{
public:
	MCMLKernelQ(){}

	CylAbsorptionScorer<QueuedMultiThreadAccumulator<double,double>>& cylAbsorptionScorer(){ return get<2>(m_scorer); }
};


typedef std::tuple<
		EventScorer,
		ConservationScorer,
		CylAbsorptionScorer<AtomicMultiThreadAccumulator<double,double>>,
		PathScorer
		>
		MCMLScorerPackWithTraces;

class MCMLKernelWithTraces : public TetraMCKernel<RNG_SFMT_AVX,MCMLScorerPackWithTraces>
{
public:
	MCMLKernelWithTraces(){}

};



#endif /* KERNELS_SOFTWARE_MCMLKERNEL_HPP_ */
