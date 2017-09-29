/*
 * DoseSurfaceHistogram.hpp
 *
 *  Created on: Aug 11, 2016
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_DOSEVOLUMEHISTOGRAMGENERATOR_HPP_
#define OUTPUTTYPES_DOSEVOLUMEHISTOGRAMGENERATOR_HPP_

#include "DoseHistogramGenerator.hpp"

class DoseVolumeHistogramGenerator : public DoseHistogramGenerator
{
public:
	DoseVolumeHistogramGenerator();
	virtual ~DoseVolumeHistogramGenerator();

protected:
	virtual std::vector<ElRegion> 	getPartition() override;
	virtual std::vector<float> 		getMeasures(const std::vector<unsigned>& elements) override;
	virtual std::vector<float> 		getDose(const std::vector<unsigned>& elements) override;
};

#endif /* OUTPUTTYPES_DOSESURFACEHISTOGRAMGENERATOR_HPP_ */
