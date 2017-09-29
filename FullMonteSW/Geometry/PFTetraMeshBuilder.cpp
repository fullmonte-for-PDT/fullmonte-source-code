/*
 * PointFaceTetraMeshBuilder.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: jcassidy
 */

#include <FullMonteSW/Geometry/PFTetraMeshBuilder.hpp>

using namespace std;

PFTetraMeshBuilder::PFTetraMeshBuilder()
{

}

PFTetraMeshBuilder::~PFTetraMeshBuilder()
{

}

void PFTetraMeshBuilder::setFace(std::size_t IDf,std::array<unsigned,3> IDps,std::array<unsigned,2> IDts)
{
	m_facePoints[IDf] = IDps;
	m_faceTetras[IDf] = IDts;
	addFaceToTetra(IDf,IDts[0],IDps,true);
	addFaceToTetra(IDf,IDts[1],IDps,false);
}

void PFTetraMeshBuilder::addFaceToTetra(size_t IDf,size_t IDt,array<unsigned,3> IDps,bool up)
{
	int IDf_directed = up ? IDf : -IDf;

	// ignore face/tetra 0 (dummy)
	if (IDt == 0 || IDf == 0)
		return;

	if (m_tetraPoints[IDt][0] == 0)			// tetra is blank (0 faces added so far)
	{
		boost::sort(IDps);
		boost::copy(IDps,m_tetraPoints[IDt].begin());
		m_tetraFaces[IDt][0] = IDf_directed;
	}
	else if (m_tetraPoints[IDt][3] == 0)	// tetra is populated but last tetra point is blank (only 1 face added so far)
	{
		boost::sort(IDps);

		// compare i'th elements until mismatch
		unsigned mismatchPos=0;
		for(mismatchPos=0; mismatchPos<3 && m_tetraPoints[IDt][mismatchPos] == IDps[mismatchPos];++mismatchPos)
			{}

		unsigned IDp_insert = IDps[mismatchPos];
		unsigned insertPos=mismatchPos;

		for(insertPos=mismatchPos; insertPos<3 && m_tetraPoints[IDt][insertPos] < IDp_insert;++insertPos)
			{}

		for(unsigned dst=insertPos+1; dst<4; ++dst)
			m_tetraPoints[IDt][dst] = m_tetraPoints[dst-1][insertPos];

		m_tetraPoints[IDt][insertPos] = IDp_insert;
		m_faceTetras[IDf][1] = IDt;
		m_tetraFaces[IDt][1] = IDf_directed;
	}
	else									// points are fully populated, just add the face
	{
		// insert face ID into array at first nonzero slot (fully populated -> at least 2 faces added so far)
		unsigned i=2 + (m_tetraFaces[IDt][2]!=0);

		m_tetraFaces[IDt][i] = IDf_directed;
		m_faceTetras[IDf][!up] = IDt;
	}
}

