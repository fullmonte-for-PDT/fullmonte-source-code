/*
 * DoseHistogram.cpp
 *
 *  Created on: Apr 20, 2017
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_EMPIRICALCDF_HPP_
#define OUTPUTTYPES_EMPIRICALCDF_HPP_

#include <vector>
#include <functional>
#include <utility>
#include <cmath>

#include <boost/range/algorithm.hpp>

#include "DoseHistogram.hpp"

#include <FullMonteSW/OutputTypes/OutputDataType.hpp>

#include <iostream>
#include <iomanip>
using namespace std;

const ordered_t ordered;

DoseHistogram::const_iterator DoseHistogram::begin() const
{
	return m_histogram.begin();
}

DoseHistogram::const_iterator DoseHistogram::end() const
{
	return m_histogram.end();
}

float DoseHistogram::totalMeasure() const
{
	return m_histogram.empty() ? 0.0f : m_histogram.back().cmeasure;
}

std::size_t DoseHistogram::dim() const
{
	return m_histogram.size();
}

DoseHistogram::const_iterator DoseHistogram::doseAtPercentile(float p) const
{
	return m_histogram.begin();
}

DoseHistogram::const_iterator DoseHistogram::percentileOfDose(float d) const
{
	return m_histogram.begin();
}

const OutputDataType doseHistogramOutputDataType{
	"DoseHistogram"
};

const OutputDataType* DoseHistogram::type() const
{
	return &doseHistogramOutputDataType;
}


#endif /* OUTPUTTYPES_EMPIRICALCDF_HPP_ */


