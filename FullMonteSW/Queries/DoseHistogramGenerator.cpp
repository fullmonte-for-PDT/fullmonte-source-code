/*
 * DoseSurfaceHistogramGenerator.cpp
 *
 *  Created on: Aug 11, 2016
 *      Author: jcassidy
 */

#include <FullMonteSW/OutputTypes/SpatialMap.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include "DoseHistogramGenerator.hpp"

#include "DoseHistogram.hpp"
#include "DoseHistogramCollection.hpp"

#include "EmpiricalCDF.hpp"
#include <vector>
#include <algorithm>

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/transformed.hpp>


using namespace std;

DoseHistogramGenerator::DoseHistogramGenerator()
{
}

DoseHistogramGenerator::~DoseHistogramGenerator()
{
}

void DoseHistogramGenerator::mesh(const TetraMesh* M)
{
	m_mesh=M;
}

const TetraMesh* DoseHistogramGenerator::mesh() const
{
	return m_mesh;
}

void DoseHistogramGenerator::dose(const OutputData* phi)
{
	if (auto p = dynamic_cast<const SpatialMap<float>*>(phi))
		m_dose = p;
	else
	{
		m_dose = nullptr;
		//cout << "ERROR: DoseHistogramGenerator::dose(OutputData*) can't be cast to a SpatialMap<float>" << endl;
	}
}

const SpatialMap<float>* DoseHistogramGenerator::dose() const
{
	return m_dose;
}

void DoseHistogramGenerator::updateHistogram()
{
	// copy input to output before sorting
	m_histogram.resize(m_data.size());
	boost::copy(m_data,m_histogram.begin());

	// compute DVH by sorting within each region (could be parallelized)
	for(unsigned r=1;r<m_regionEnd.size();++r)
	{
		sort(
				m_histogram.begin()+m_regionEnd[r-1],
				m_histogram.begin()+m_regionEnd[r],
				InputElement::DoseLess());
	}

	// accumulate measure (area/volume) s.t. dvh[i].cmeasure is the total measure of elements receiving a dose <= dvh[i].dose
	// this could also be merged with the loop above and parallelized
	for(unsigned r=1;r<m_regionEnd.size(); ++r)
	{
		(m_histogram.begin()+m_regionEnd[r-1])->cmeasure=0;

		if (m_regionEnd[r-1] != m_regionEnd[r])
			for(vector<OutputElement>::iterator it=m_histogram.begin()+m_regionEnd[r-1]+1;
				it != m_histogram.begin()+m_regionEnd[r];
				++it)
			it->cmeasure += it->measure;
	}
}

void DoseHistogramGenerator::updatePartition()
{
	// call derived-class method to get the partition
	vector<ElRegion> elements = getPartition();
	m_elements.resize(elements.size());

	// group by region
	boost::sort(elements,ElRegion::RegionComp());

	// find the end of each region in the sorted vector of elements
	m_regionEnd.clear();

	for(
			vector<ElRegion>::const_iterator l=elements.begin();
			l != elements.end();
			l = lower_bound(l,elements.cend(),m_regionEnd.size(),ElRegion::RegionComp()))
	{
		m_regionEnd.push_back(l-elements.begin());
	}
	m_regionEnd.push_back(elements.size());

	// copy result
	boost::copy(
			elements | boost::adaptors::transformed([](const ElRegion& er){ return er.element; }),
			m_elements.begin());
}

void DoseHistogramGenerator::updateDose()
{
	// call derived-class dose method
	vector<float> dose = getDose(m_elements);

	// copy into input data vector
	m_data.resize(m_elements.size());
	for(unsigned i=0;i<dose.size();++i)
		m_data[i].dose = dose[i];
}

void DoseHistogramGenerator::updateMeasures()
{
	// call derived-class measure method
	vector<float>	measure = getMeasures(m_elements);

	// copy into input data vector
	m_data.resize(m_elements.size());
	for(unsigned i=0;i<measure.size();++i)
		m_data[i].measure = measure[i];
}

void DoseHistogramGenerator::update()
{
	updatePartition();
	updateMeasures();
	updateDose();

	updateHistogram();
}

OutputData* DoseHistogramGenerator::result() const
{
	DoseHistogramCollection* C = new DoseHistogramCollection();

	for(unsigned i=1;i<m_regionEnd.size();++i)
		if (m_regionEnd[i-1] != m_regionEnd[i])
		{
			DoseHistogram* H = new DoseHistogram(
					m_histogram.begin()+m_regionEnd[i-1],
					m_histogram.begin()+m_regionEnd[i],
					ordered);
			C->add(H);
		}

	return C;
}
