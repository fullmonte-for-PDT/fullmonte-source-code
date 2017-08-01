/*
 * DoseSurfaceHistogramGenerator.cpp
 *
 *  Created on: Aug 11, 2016
 *      Author: jcassidy
 */

#include <FullMonteSW/OutputTypes/SpatialMap.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>
#include "DoseSurfaceHistogramGenerator.hpp"

#include "DoseHistogram.hpp"

#include <vector>
#include <iostream>

using namespace std;

DoseSurfaceHistogramGenerator::DoseSurfaceHistogramGenerator()
{
}

DoseSurfaceHistogramGenerator::~DoseSurfaceHistogramGenerator()
{
}

void DoseSurfaceHistogramGenerator::partition(Partition* P)
{
	m_partition=P;
}

vector<DoseHistogramGenerator::ElRegion> DoseSurfaceHistogramGenerator::getPartition()
{
	vector<DoseHistogramGenerator::ElRegion> F;

	if (!m_partition)
	{
		cout << "DoseSurfaceHistogramGenerator::getPartition() - no partition provided for directed faces" << endl;
		return F;
	}

	for(const auto df : mesh()->directedFaces() | boost::adaptors::indexed(0U))
	{
		// get partition assignment for the tetra
		unsigned r = m_partition->get(get(id,*mesh(),df.value()));

		if (r)
			F.emplace_back(df.index(),r);
	}

	cout << "DoseSurfaceHistogramGenerator::getPartition() added " << F.size() << " elements to histograms (out of " << get(num_directed_faces,*mesh()) << ")" << endl;

	return F;
}

vector<float> DoseSurfaceHistogramGenerator::getMeasures(const vector<unsigned>& elements)
{
	vector<float> measures(elements.size());
	const TetraMesh* m = mesh();

	boost::transform(elements,
			measures.begin(),
			[m](unsigned i){ return get(area,*m,TetraMesh::DirectedFaceDescriptor(i)); });

	return measures;
}

vector<float> DoseSurfaceHistogramGenerator::getDose(const std::vector<unsigned>& elements)
{
	const OutputData* dmap = dose();

	auto p = dynamic_cast<const SpatialMap<float>*>(dmap);

	if (!p)
	{
		cout << "ERROR: DoseSurfaceHistogramGenerator::dose(OutputData*) can't be cast to a SpatialMap<float>" << endl;
		return vector<float>();
	}

	vector<float> d(elements.size(),std::numeric_limits<float>::quiet_NaN());

	boost::transform(elements,
			d.begin(),
			[p](unsigned i){ return p->get(i); });

	return d;
}
