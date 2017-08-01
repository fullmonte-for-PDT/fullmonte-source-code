/*
 * DoseSurfaceHistogramGenerator.cpp
 *
 *  Created on: Aug 11, 2016
 *      Author: jcassidy
 */

#include <FullMonteSW/OutputTypes/SpatialMap.hpp>
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include "DoseVolumeHistogramGenerator.hpp"

#include <vector>
#include <iostream>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/transformed.hpp>

using namespace std;


DoseVolumeHistogramGenerator::DoseVolumeHistogramGenerator()
{
}

DoseVolumeHistogramGenerator::~DoseVolumeHistogramGenerator()
{
}

vector<float> DoseVolumeHistogramGenerator::getMeasures(const std::vector<unsigned>& elements)
{
	vector<float> measures(elements.size());

	const TetraMesh* m = mesh();

	boost::copy(
			elements | boost::adaptors::transformed( [m](unsigned i){ return get(volume,*m,TetraMesh::TetraDescriptor(i)); }),
			measures.begin());

	return measures;
}

vector<DoseHistogramGenerator::ElRegion> DoseVolumeHistogramGenerator::getPartition()
{
	vector<ElRegion> tets;
	const TetraMesh* m = mesh();

	for(const auto t : m->tetras() | boost::adaptors::indexed(0U))
	{
		// get partition assignment for the tetra
		unsigned r = get(region,*mesh(),t.value());

		// add tetras with nonzero region code to vector
		if (r)
			tets.emplace_back(t.index(),r);
	}

	return tets;
}

vector<float> DoseVolumeHistogramGenerator::getDose(const std::vector<unsigned>& elements)
{
	vector<float>		d(elements.size(),std::numeric_limits<float>::quiet_NaN());
	const SpatialMap<float>* dmap = dose();

	if (dmap->spatialType() != AbstractSpatialMap::Volume)
	{
		std::cout << "ERROR in DoseVolumeHistogramGenerator::result(): Incorrect spatial type for input data" << std::endl;
		return d;
	}

	boost::transform(elements,
			d.begin(),
			[dmap](unsigned i){ return (*dmap)[i]; });

	return d;
}
