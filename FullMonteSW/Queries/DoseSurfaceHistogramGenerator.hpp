/*
 * DoseSurfaceHistogram.hpp
 *
 *  Created on: Aug 11, 2016
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_DOSESURFACEHISTOGRAMGENERATOR_HPP_
#define OUTPUTTYPES_DOSESURFACEHISTOGRAMGENERATOR_HPP_

#include <vector>
#include "DoseHistogramGenerator.hpp"

class Partition;

class DoseSurfaceHistogramGenerator : public DoseHistogramGenerator
{
public:
	DoseSurfaceHistogramGenerator();
	virtual ~DoseSurfaceHistogramGenerator();

	void partition(Partition* p);


protected:
	virtual std::vector<ElRegion> getPartition() override;
	virtual std::vector<float> getMeasures(const std::vector<unsigned>& elements) override;
	virtual std::vector<float> getDose(const std::vector<unsigned>& elements) override;

private:
	Partition*		m_partition=nullptr;
};

#endif /* OUTPUTTYPES_DOSESURFACEHISTOGRAMGENERATOR_HPP_ */
