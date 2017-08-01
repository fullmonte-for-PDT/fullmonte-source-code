/*
 * TetraOrientationChecker.cpp
 *
 *  Created on: May 18, 2017
 *      Author: jcassidy
 */

#include "ErrorChecker.hpp"

TetraOrientationChecker::TetraOrientationChecker()
{
}

TetraOrientationChecker::~TetraOrientationChecker();
{
}

void TetraOrientationChecker::setup()
{
	m_range = m_mesh->tetras();
	m_tetraIt = begin(m_range);
}

bool TetraOrientationChecker::nextTestCase()
{
	if (m_tetraIt == end(m_range))
		return false;

	m_tetraIt++;
	return m_tetraIt != end(m_range);
}

Error* TetraOrientationChecker::test()
{
	array<Point<3,double>,4> 	Pt = get(point_coords,*m_mesh,m_tetraIt);

	// Ph[i] is the height of IDps[i] over the face made up by IDps[j] where j=0..3 excluding i
	array<double,4> 			Ph;

	for(unsigned i=0;i<4;++i)
	{
		array<Point<3,double>,3> Pf;
		for(unsigned j=0;j<3;++j)
			Pf[j] = Pt[tetra_face_opposite_point_indices[i].faceidx[j]];

		Point<3,double> P = Pt[tetra_face_opposite_point_indices[i].oppidx];


		if (!tet_ok)
		{
			cout << "tet " << IDt << " face " << f << " (ID " << T.IDfs[f] << ") opposite-corner heights: ";
			for(unsigned i=0;i<4;++i)
				cout << h[i] << ' ';
			cout << endl;
		}
	}


	for(unsigned i=0;i<4;++i)
		if (h[i] < -m_pointHeightTolerance)
		{
			tet_ok=false;
			cout << "ERROR MeshChecker()::pointHeights incorrect height of " << f << "'th tetra point (ID " << IDps[f] << ") over " << i << "'th face (ID" << T.IDfs[i] << "): " << h[i] << endl;
		}


	return tet_ok;
}


