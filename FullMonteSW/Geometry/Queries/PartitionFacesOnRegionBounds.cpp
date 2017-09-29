/*
 * PartitionFacesOnRegionBounds.cpp
 *
 *  Created on: Jun 6, 2017
 *      Author: jcassidy
 */

#include "PartitionFacesOnRegionBounds.hpp"

#include <FullMonteSW/Geometry/Partition.hpp>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>

PartitionFacesOnRegionBounds::PartitionFacesOnRegionBounds()
{

}

PartitionFacesOnRegionBounds::~PartitionFacesOnRegionBounds()
{

}

void PartitionFacesOnRegionBounds::regions(Partition* p)
{
	m_partition = p;
}

void PartitionFacesOnRegionBounds::mesh(TetraMesh* m)
{
	m_mesh = m;
}

void PartitionFacesOnRegionBounds::update()
{
	m_result.clear();
	m_result.resize(get(num_directed_faces,*m_mesh));

	Partition* p = m_partition ? m_partition : m_mesh->regions();

	if(!p)
		throw std::logic_error("PartitionFacesOnRegionBounds::update() no partition available");

	for(const auto fd : m_mesh->directedFaces())
	{
		unsigned rUp   = p->get(get(id,*m_mesh,get(tetra_above_face,*m_mesh,fd)));
		unsigned rDown = p->get(get(id,*m_mesh,get(tetra_below_face,*m_mesh,fd)));

		m_result[get(id,*m_mesh,fd)] = rUp != rDown;
	}
}

Partition* PartitionFacesOnRegionBounds::result() const
{
	Partition* P = new Partition();
	P->resize(m_result.size());

	for(unsigned i=0;i<m_result.size();++i)
		P->assign(i,m_result[i]);

	return P;
}



