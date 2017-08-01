/*
 * DirectedSurfaceOfRegion.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: jcassidy
 */

#include "DirectedSurfaceOfRegion.hpp"
#include <FullMonteSW/Geometry/Partition.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap.hpp>
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>

using namespace std;

DirectedSurfaceOfRegion::DirectedSurfaceOfRegion()
{

}

DirectedSurfaceOfRegion::~DirectedSurfaceOfRegion()
{

}

void DirectedSurfaceOfRegion::mesh(TetraMesh* m)
{
	m_mesh=m;
}

void DirectedSurfaceOfRegion::partition(Partition *p)
{
	m_partition=p;
}

void DirectedSurfaceOfRegion::source(OutputData* d)
{
	auto m = dynamic_cast<SpatialMap<float>*>(d);
	if (!d)
	{
		m_directedData=nullptr;
		cout << "DirectedSurfaceOfRegion::source(d) - input cannot be cast to SpatialMap<float>" << endl;
	}
	else
		m_directedData = m;
}

void DirectedSurfaceOfRegion::update()
{
	m_faces.clear();
	m_faces.resize(get(num_faces,*m_mesh));

	Partition *p = m_partition ? m_partition : m_mesh->regions();

	// populate face structure to help speed indexing
	for(const auto f : m_mesh->faces() | boost::adaptors::indexed(0U))
	{
		const auto upTetra = get(tetra_above_face,*m_mesh,f.value());
		const auto dnTetra = get(tetra_below_face,*m_mesh,f.value());

		m_faces[f.index()].dnRegion = p->get(get(id,*m_mesh,dnTetra));
		m_faces[f.index()].upRegion = p->get(get(id,*m_mesh,upTetra));
		m_faces[f.index()].upIDf = get(id,*m_mesh,get(directed_face,*m_mesh,f.value(),false));
		m_faces[f.index()].dnIDf = get(id,*m_mesh,get(directed_face,*m_mesh,f.value(),true));
	}

	cout << "DirectedSurfaceOfRegion::update() finished with " << m_faces.size() << " undirected faces" << endl;
}

OutputData* DirectedSurfaceOfRegion::entering() const
{
	vector<float> v(2*m_faces.size(),0.0f);
	for(const auto f : m_faces)
	{
		v[f.upIDf] = m_directedData->get(f.upIDf);
		v[f.dnIDf] = m_directedData->get(f.dnIDf);
	}

	return new SpatialMap<float>(move(v),AbstractSpatialMap::DirectedSurface,AbstractSpatialMap::Scalar);
}

OutputData* DirectedSurfaceOfRegion::exiting() const
{
	vector<float> v(2*m_faces.size(),0.0f);
	for(const auto f : m_faces)
	{
		v[f.upIDf] = m_directedData->get(f.dnIDf);
		v[f.dnIDf] = m_directedData->get(f.upIDf);
	}

	return new SpatialMap<float>(move(v),AbstractSpatialMap::DirectedSurface,AbstractSpatialMap::Scalar);
}

Partition* DirectedSurfaceOfRegion::region() const
{
	Partition* P = new Partition();
	P->resize(get(num_directed_faces,*m_mesh));

	for(const auto f : m_faces)
	{
		if (f.upRegion != f.dnRegion)			// at boundary faces, write region codes
		{
			P->assign(f.upIDf,f.upRegion);
			P->assign(f.dnIDf,f.dnRegion);
		}
		else									// at internal faces, write zero
		{
			P->assign(f.upIDf,0U);
			P->assign(f.dnIDf,0U);
		}
	}

	return P;
}

OutputData* DirectedSurfaceOfRegion::total() const
{
	vector<float> v(2*m_faces.size(),0.0f);
	for(const auto f : m_faces)
		v[f.upIDf] = v[f.dnIDf] = m_directedData->get(f.dnIDf)+m_directedData->get(f.upIDf);
	return new SpatialMap<float>(move(v),AbstractSpatialMap::Surface,AbstractSpatialMap::Scalar);
}



