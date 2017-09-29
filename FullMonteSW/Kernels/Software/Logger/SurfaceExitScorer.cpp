/*
 * SurfaceExitScorer.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: jcassidy
 */

#include "SurfaceExitScorer.hpp"

#include <FullMonteSW/OutputTypes/OutputData.hpp>
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap.hpp>

#include <FullMonteSW/Geometry/Units/BaseUnit.hpp>

#include <FullMonteSW/Geometry/FaceLinks.hpp>

#include <FullMonteSW/Kernels/Kernel.hpp>

#include <cassert>
#include <boost/range/algorithm.hpp>

SurfaceExitScorer::SurfaceExitScorer()
{
}

SurfaceExitScorer::~SurfaceExitScorer()
{
}

void SurfaceExitScorer::prepare(const Kernel* K)
{
	const TetraMesh* M = dynamic_cast<const TetraMesh*>(K->geometry());

	if (!M)
		throw std::logic_error("SurfaceExitScorer::prepare(K) invalid cast to TetraMesh");

	m_acc.resize(get(num_faces,*M));
}

void SurfaceExitScorer::postResults(OutputDataCollection* C) const
{
	// convert to float
	std::vector<float> vf(m_acc.size());

	for(unsigned i=0;i<m_acc.size();++i)
		vf[i] = m_acc[i];

	// create vector
	SpatialMap<float> *smap = new SpatialMap<float>(std::move(vf),AbstractSpatialMap::Surface,AbstractSpatialMap::Scalar);
	smap->quantity(&Units::energy);
	smap->units(&Units::packet);

	smap->name("SurfaceExitEnergy");

	C->add(smap);
}

void SurfaceExitScorer::clear()
{
	m_acc.clear();
}

SurfaceExitScorer::Logger SurfaceExitScorer::createLogger()
{
	return Logger();
}

SurfaceExitScorer::Logger::Logger()
{
}

SurfaceExitScorer::Logger::~Logger()
{
}
