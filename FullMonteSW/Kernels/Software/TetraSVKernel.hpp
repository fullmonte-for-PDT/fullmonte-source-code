/*
 * TetraSVKernel.hpp
 *
 *  Created on: Feb 1, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_TETRASVKERNEL_HPP_
#define KERNELS_SOFTWARE_TETRASVKERNEL_HPP_

#include "TetraMCKernel.hpp"
#include "TetraMCKernelThread.hpp"

#include <FullMonteSW/Kernels/Software/Logger/LoggerTuple.hpp>

#include "Logger/BaseLogger.hpp"

#include "Logger/VolumeAbsorptionScorer.hpp"
#include "Logger/SurfaceExitScorer.hpp"
#include "Logger/ConservationScorer.hpp"
#include "Logger/EventScorer.hpp"
#include "Logger/DirectedSurfaceScorer.hpp"

#include "Logger/QueuedMultiThreadAccumulator.hpp"

typedef std::tuple<
		EventScorer,
		ConservationScorer,
		VolumeAbsorptionScorer,
		SurfaceExitScorer,
		DirectedSurfaceScorer<QueuedMultiThreadAccumulator<double,double>>>
		SVScorer;

class TetraSVKernel : public TetraMCKernel<RNG_SFMT_AVX,SVScorer>
{
public:
	TetraSVKernel(){}

	typedef RNG_SFMT_AVX RNG;

	EventScorer& 														eventScorer() 			{ return get<0>(m_scorer); }
	ConservationScorer& 												conservationScorer() 	{ return get<1>(m_scorer); }
	VolumeAbsorptionScorer& 											volumeScorer() 			{ return get<2>(m_scorer); }
	SurfaceExitScorer& 													surfaceScorer() 		{ return get<3>(m_scorer); }
	DirectedSurfaceScorer<QueuedMultiThreadAccumulator<double,double>>&	directedSurfaceScorer()	{ return get<4>(m_scorer); }
};



#endif /* KERNELS_SOFTWARE_TETRASVKERNEL_HPP_ */

