/*
 * VolumeAbsorptionScorer.cpp
 *
 *  Created on: Sep 29, 2016
 *      Author: jcassidy
 */

#include "VolumeAbsorptionScorer.hpp"

#include <FullMonteSW/OutputTypes/SpatialMap.hpp>

#include <FullMonteSW/OutputTypes/OutputData.hpp>
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>

#include <FullMonteSW/Geometry/Units/BaseUnit.hpp>
#include <FullMonteSW/Geometry/TetraMesh.hpp>

#include <FullMonteSW/Kernels/Kernel.hpp>

VolumeAbsorptionScorer::VolumeAbsorptionScorer()
{
}

VolumeAbsorptionScorer::~VolumeAbsorptionScorer()
{
}

void VolumeAbsorptionScorer::clear()
{
	m_acc.clear();
}

void VolumeAbsorptionScorer::prepare(const Kernel* K)
{
	const TetraMesh* M = dynamic_cast<const TetraMesh*>(K->geometry());

	if (!M)
		throw std::logic_error("VolumeAbsorptionScorer::prepare(K) can't cast to TetraMesh");

	m_acc.resize(get(num_tetras,*M));
	clear();
}

void VolumeAbsorptionScorer::postResults(OutputDataCollection* C) const
{
	// convert to float
	std::vector<float> vf(m_acc.size());

	for(unsigned i=0;i<m_acc.size();++i)
		vf[i] = m_acc[i];

	// create vector
	SpatialMap<float> *vmap = new SpatialMap<float>(std::move(vf),AbstractSpatialMap::Volume,AbstractSpatialMap::Scalar);

	vmap->quantity(&Units::energy);
	vmap->units(&Units::packet);

	vmap->name("VolumeEnergy");

	C->add(vmap);
}

void VolumeAbsorptionScorer::queueSize(unsigned q)
{
	m_queueSize=q;
}

VolumeAbsorptionScorer::Logger VolumeAbsorptionScorer::createLogger()
{
	return VolumeAbsorptionScorer::Logger(m_queueSize);
}




/**********************************************************************************************************************************
 * Logger for volume absorption scoring
 */

VolumeAbsorptionScorer::Logger::Logger(std::size_t qSize) :
	m_handle(qSize)
{
}

void VolumeAbsorptionScorer::Logger::eventClear()
{
	m_handle.clear();
}

void VolumeAbsorptionScorer::Logger::eventCommit(AbstractScorer& S)
{
 	m_handle.commit(static_cast<VolumeAbsorptionScorer&>(S).m_acc);
}
