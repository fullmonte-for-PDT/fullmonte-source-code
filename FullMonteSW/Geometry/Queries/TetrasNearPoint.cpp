/*
 * TetrasNearPoint.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: jcassidy
 */


#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Points.hpp>
#include <FullMonteSW/Geometry/Vector.hpp>

#include <iostream>

#include "TetrasNearPoint.hpp"

#include <cmath>

TetrasNearPoint::TetrasNearPoint()
{
}

TetrasNearPoint::~TetrasNearPoint()
{
}

void TetrasNearPoint::centre(Point<3,double> P)
{
	m_centre=P;
}

void TetrasNearPoint::radius(float r)
{
	m_radius2 = r*r;
}

void TetrasNearPoint::mesh(const TetraMesh* M)
{
	m_mesh=M;
}

void TetrasNearPoint::start()
{
	// Precompute distances since Np << Nt
	m_squaredDistances.resize(m_mesh->points()->size());

	m_squaredDistances[0] = std::numeric_limits<float>::infinity();

	for(unsigned i=1;i<m_squaredDistances.size();++i)
	{
		Point<3,double> P = m_mesh->points()->get(i);
		m_squaredDistances[i] = norm2_l2(P,m_centre);
	}

	// Set up at zero tetra and advance to first tetra meeting criteria
	m_tetraID = 0;
	next();
}

void TetrasNearPoint::next()
{
	for(++m_tetraID; m_tetraID < m_mesh->tetraCells()->size(); ++m_tetraID)
	{
		bool any=false,all=true;

		const auto IDps = get(points,*m_mesh,TetraMesh::TetraDescriptor(m_tetraID));

		for(unsigned i=0; i<4; ++i)
		{
			bool within = m_squaredDistances[IDps[i]] <m_radius2;
			any |= within;
			all &= within;
		}

		// end loop if tetra meets the criteria
		if (all | (any & !m_requireAllWithin))
			break;
	}
}

unsigned TetrasNearPoint::currentTetraID() const
{
	return m_tetraID;
}

bool TetrasNearPoint::done() const
{
	return m_tetraID >= m_mesh->tetraCells()->size();
}
