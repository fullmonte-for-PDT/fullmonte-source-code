/*
 * TetrasFromTetraMesh.cpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#include "TetrasFromTetraMesh.hpp"

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>

#include <functional>

TetrasFromTetraMesh::TetrasFromTetraMesh()
{
}

TetrasFromTetraMesh::~TetrasFromTetraMesh()
{
}

void TetrasFromTetraMesh::mesh(const TetraMesh* M)
{
	m_mesh=M;
}

void TetrasFromTetraMesh::setFacesForFluenceCounting(const Partition* P)
{
	m_faceCountingPartition = P;
}

void TetrasFromTetraMesh::update()
{
	// build the kernel tetras (halfspace representation) from the provided mesh
	makeKernelTetras();

	// update the list of faces for fluence counting (may not apply to all kernels)
	updateFaceCountingPartition();

	checkKernelFaces();
}

void TetrasFromTetraMesh::updateFaceCountingPartition()
{
	if (m_faceCountingPartition)
	{
		for(Tetra& tet : m_tetras)
		{
			unsigned flag = 0;
			for(unsigned i=0;i<4;++i)
			{
				if (m_faceCountingPartition->get(tet.IDfds[i]))
					flag |= (1<<(i<<3));
			}
			tet.faceFlags = flag;
		}
	}
	else
		for (Tetra& tet : m_tetras)
			tet.faceFlags = 0;
}

/** Validates the orientation of faces within the kernel tetra structure.
 *
 */

bool TetrasFromTetraMesh::checkKernelFaces() const
{
	bool status_ok=true;

	cout << "INFO: Checking faces on " << m_tetras.size() << " tetras" << endl;

	// iterate over all tetras
	for(unsigned t=1; t < m_tetras.size(); ++t)
	{
		Tetra T = m_tetras[t];

		// face reciprocity: each face points to either zero tetra or a tetra that points back to this one
		for(unsigned f=0;f<4;++f)
		{
			Tetra adjTetra;

			if (T.adjTetras[f] >= m_tetras.size())
			{
				status_ok=false;
				cout << "  Tetra " << t << " face " << f << " connects to a tetra out of array range" << endl;
			}
			else
				adjTetra = m_tetras[T.adjTetras[f]];

			if (T.adjTetras[f] != 0 && boost::count_if(adjTetra.adjTetras, [t](unsigned i){ return i==t; }) != 1)
			{
				cout << "  Tetra " << t << " face " << f << " connects to tetra " << T.adjTetras[f] << " but that tetra connects to ";
				for(unsigned i=0;i<4;++i)
					cout << m_tetras[t].adjTetras[i] << ' ';
				cout << endl;
				//cout << "    Face also connects to tetra " <<

				status_ok = false;
			}
		}

//		// face orientation:
//		for(unsigned f=0;f<4;++f)
//		{
//
//		}

//		bool tet_ok=true;
//		for(int f=0;f<4;++f)
//		{
//			std::array<float,4> h = to_array<float,4>(m_tetras[IDt].heights(to_m128(tetraPointCoords[f])));
//
//			for(unsigned i=0;i<4;++i)
//				if (h[i] < -4e-5)
//				{
//					tet_ok=status_ok=false;
//					cout << "Error: incorrect height of " << f << "'th tetra point (ID " << IDps[f] << ") over " << i << "'th face (ID" << m_tetras[IDt].IDfs[i] << "): " << h[i] << endl;
//				}
//
//			if (!tet_ok)
//			{
//				cout << "tet " << IDt << " face " << f << " opposite-corner heights: ";
//				for(unsigned i=0;i<4;++i)
//					cout << h[i] << ' ';
//				cout << endl;
//			}
//		}
	}
//
//	printTetra(m_tetras[0],std::cout);
//	printTetra(m_tetras[120569],std::cout);

	return status_ok;
}



/** Converts the TetraMesh representation to the packed data structures used by the kernel.
 *
 *
 * Produces: m_tetras
 *
 */

union __m128_union {
	std::array<float,4>		f;
	__m128					v;
	__m128i					vi;
};

Tetra TetrasFromTetraMesh::convert(const TetraMesh& M,TetraMesh::TetraDescriptor t)
{
	Tetra T;

	T.faceFlags = 0;

	const auto F = get(directed_faces,M,t);

//	cout << "Face IDs: ";
//	for(unsigned i=0;i<4;++i)
//		cout << F[i].value() << ' ';
//	cout << endl;

	__m128_union mtx[4];

	for(unsigned i=0;i<4;++i)
	{
		T.IDfds[i] 		= get(id,M,F[i]);
		T.adjTetras[i] 	= get(id,
				M,
				get(tetra_below_face,M,F[i]));

		array<double,3> n;
		float c;

		tie(n,c) = get(face_plane,M,F[i]);

		// save the matrix transposed so all x/y/z/c values live in a single __m128
		for(unsigned j=0;j<3;++j)
			mtx[j].f[i] = n[j];
		mtx[3].f[i] = c;
	}

	T.nx = mtx[0].v;
	T.ny = mtx[1].v;
	T.nz = mtx[2].v;
	T.C  = mtx[3].v;

	T.matID = get(region,M,t);

	return T;
}

void TetrasFromTetraMesh::makeKernelTetras()
{
	m_tetras.resize(get(num_tetras,*m_mesh));
	cout << "Converting " << m_tetras.size() << " tetras" << endl;

	boost::transform(
		m_mesh->tetras(),
		m_tetras.begin(),
		std::bind(convert,*m_mesh,std::placeholders::_1));
}

const std::vector<Tetra>& TetrasFromTetraMesh::tetras() const
{
	return m_tetras;
}

