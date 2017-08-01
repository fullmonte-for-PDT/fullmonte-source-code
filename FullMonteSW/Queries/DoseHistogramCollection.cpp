/*
 * DoseHistogramCollection.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: jcassidy
 */

#include "DoseHistogramCollection.hpp"
#include <algorithm>

DoseHistogramCollection::DoseHistogramCollection()
{
}

DoseHistogramCollection::~DoseHistogramCollection()
{
}

unsigned DoseHistogramCollection::count() const
{
	return m_histograms.size();
}

DoseHistogram* DoseHistogramCollection::get(unsigned i) const
{
	return m_histograms.at(i);
}

void DoseHistogramCollection::add(DoseHistogram* h)
{
	if (std::find(m_histograms.begin(),m_histograms.end(),h) == m_histograms.end())
		m_histograms.push_back(h);
	else
		throw std::logic_error("Histogram is already present in collection");
}

void DoseHistogramCollection::remove(DoseHistogram* h)
{
	auto it = std::find(m_histograms.begin(),m_histograms.end(),h);
	if (it != m_histograms.end())
		m_histograms.erase(it);
	else
		throw std::logic_error("No such histogram in the collection");
}

DoseHistogramCollection* DoseHistogramCollection::clone() const
{
	return new DoseHistogramCollection(*this);
}

#include <FullMonteSW/OutputTypes/OutputDataType.hpp>
const OutputDataType doseHistogramCollectionOutputDataType{
	"DoseHistogram"
};

const OutputDataType* DoseHistogramCollection::type() const
{
	return &doseHistogramCollectionOutputDataType;
}
