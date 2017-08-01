/*
 * TetraEnclosingPointByLinearSearch.cpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#include "TetraEnclosingPointByLinearSearch.hpp"

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Vector.hpp>
#include <FullMonteSW/Geometry/StandardArrayKernel.hpp>

#include <iostream>

using namespace std;

TetraEnclosingPointByLinearSearch::TetraEnclosingPointByLinearSearch()
{
}

TetraEnclosingPointByLinearSearch::~TetraEnclosingPointByLinearSearch()
{
}

void TetraEnclosingPointByLinearSearch::mesh(const TetraMesh* M)
{
	m_mesh=M;
}

void TetraEnclosingPointByLinearSearch::point(Point<3,double> p)
{
	m_point=p;
}

void TetraEnclosingPointByLinearSearch::update()
{
	m_inside.clear();
	m_maybe.clear();

	if (m_verbose)
		cout << "PointTetraLocator::locate() - Searching for tetra to enclose " <<
		m_point[0] << ' ' << m_point[1] << ' ' << m_point[2] << endl;

	for(const auto t : m_mesh->tetras())
	{
		array<double,4> 			h;
		array<Point<3,double>,4>	P = get(point_coords, *m_mesh, t);

		// loop over all faces, computing height of the point over each face
		for(unsigned f=0;f<4;++f)
		{
			const auto n = normalize(
					cross(
							P[tetra_face_opposite_point_indices[f].faceidx[0]],
							P[tetra_face_opposite_point_indices[f].faceidx[1]],
							P[tetra_face_opposite_point_indices[f].faceidx[2]]));

			h[f] = dot(n,m_point-P[tetra_face_opposite_point_indices[f].faceidx[0]]);
		}

		bool in = h[0] >= 0 && h[1] >= 0 && h[2] >= 0 && h[3] >= 0;

		bool maybe = h[0] >= -m_heightEpsilon && h[1] >= -m_heightEpsilon && h[2] >= -m_heightEpsilon && h[3] >= -m_heightEpsilon;


		if(in)
			m_inside.push_back(t);
		else if(maybe)
			m_maybe.push_back(t);

		if (m_verbose)
		{
			if (in | maybe)
			{
				cout << "  Tetra " << t.value() << " " << (in ? "encloses" : "may enclose") << " with heights ";
				for(unsigned i=0;i<4;++i)
					cout << h[i] << ' ';
			}

			//			TetraByFaceID IDfs = get(::faces, *this, TetraMesh::TetraDescriptor(i) );
			//			cout << "    Face|adj tetra: ";
			//
			//			for(unsigned i=0;i<4;++i)
			//				cout << IDfs[i] << '|' << get(tetra_below_face, *this, FaceDescriptor(IDfs[i])).value() << "  ";
			//			cout << endl;
			//
			//			++N;
			//			IDt=i;
		}
	}
}

TetraMesh::TetraDescriptor TetraEnclosingPointByLinearSearch::tetra() const
{
	if (m_inside.size() > 0)
	{
		if (m_inside.size() > 1)
			cout << "WARNING - TetraEnclosingPointByLinearSearch::tetra() found multiple (" << m_inside.size() << ") tetras; returning first" << endl;
		return m_inside.front();
	}
	else if (m_maybe.size() > 1)
	{
		cout << "WARNING - TetraEnclosingPointByLinearSearch::tetra() found no tetras but " << m_maybe.size() << " near-matches; returning first" << endl;
		return m_maybe.front();
	}
	else
		cout << "ERROR - No enclosing tetra found!" << endl;
	return TetraMesh::TetraDescriptor();
}
