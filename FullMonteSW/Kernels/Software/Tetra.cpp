/*
 * Tetra.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: jcassidy
 */

#include "Tetra.hpp"

#include <iostream>
#include <iomanip>

using namespace std;

void printTetra(const Tetra& T,ostream& os)
{
	os << "==========================================================================================" << endl;
	os << "Tetra with material ID " << T.matID << endl;

	// transpose componentwise nx/ny/nz/C[0..3] vectors into (nx, ny, nz, c)[0..3]
	__m128 nx=T.nx,ny=T.ny,nz=T.nz,C=T.C;
	_MM_TRANSPOSE4_PS(nx,ny,nz,C);

	float tmp[16];
	float *n[4] = { tmp, tmp+4, tmp+8, tmp+12 };

	_mm_store_ps(n[0],nx);
	_mm_store_ps(n[1],ny);
	_mm_store_ps(n[2],nz);
	_mm_store_ps(n[3],C);

	for(unsigned f=0;f<4;++f)
	{
		os << "\tFace ID " << setw(6) << T.IDfds[f] << " Tet ID: " << setw(6) << T.adjTetras[f] << " Normal: ";
		for(unsigned j=0;j<3;++j)
			os << setw(9) << fixed << setprecision(6) << n[f][j] << ' ';
		os << " with constant " << setw(8) << setprecision(6) << n[f][3] << endl;
	}
}

//void TetraMesh::printTetra(unsigned IDt) const
//{
//	cout << "Tetra ID " << IDt << endl;
//
//	for(unsigned f=0;f<4;++f)
//	{
//		int IDf = m_tetraFaces[IDt][f];
//		Face F = getFace(IDf);
//
//		cout << "  Face " << f << " (ID " << IDf << ") tetra point heights (expect +): ";
//		for(unsigned p=0;p<4;++p)
//			cout << setw(10) << F.pointHeight(m_points[m_tetraPoints[IDt][p]]) << ' ';
//		cout << endl;
//
//		unsigned IDt_adj = IDf < 0 ?
//				m_faceTetras[-IDf][0] :
//				m_faceTetras[IDf][1];
//
//		cout << "    Adjacent tetra ID " << IDt_adj << " point heights over this face (expect -): ";
//		for(unsigned p=0;p<4;++p)
//			cout << setw(10) << (F.pointHeight(m_points[m_tetraPoints[IDt_adj][p]])) << ' ';
//		cout << endl;
//	}
//}
//
