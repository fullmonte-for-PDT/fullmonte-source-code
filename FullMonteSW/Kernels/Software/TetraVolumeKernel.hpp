/*
 * TetraVolumeKernel.hpp
 *
 *  Created on: Feb 1, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_TETRAVOLUMEKERNEL_HPP_
#define KERNELS_SOFTWARE_TETRAVOLUMEKERNEL_HPP_

#include "TetraMCKernel.hpp"
#include "TetraMCKernelThread.hpp"

#include <FullMonteSW/Kernels/Software/Logger/LoggerTuple.hpp>

#include <FullMonteSW/Kernels/Software/Logger/VolumeAbsorptionScorer.hpp>
#include <FullMonteSW/Kernels/Software/Logger/ConservationScorer.hpp>
#include <FullMonteSW/Kernels/Software/Logger/EventScorer.hpp>

#include "Logger/BaseLogger.hpp"

typedef std::tuple<
		EventScorer,
		ConservationScorer,
		VolumeAbsorptionScorer>
	TetraVolumeScorer;

class TetraVolumeKernel : public TetraMCKernel<
	RNG_SFMT_AVX,
	TetraVolumeScorer
	>
{
public:
	TetraVolumeKernel(){}
};


#endif /* KERNELS_SOFTWARE_TETRAVOLUMEKERNEL_HPP_ */

