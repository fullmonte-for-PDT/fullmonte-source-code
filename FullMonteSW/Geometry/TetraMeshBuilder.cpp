/*
 * TetraMeshBuilder.cpp
 *
 *  Created on: Nov 25, 2016
 *      Author: jcassidy
 */

#include "TetraMeshBuilder.hpp"
#include <iostream>
#include <array>
#include <vector>

#include <FullMonteSW/take_drop.hpp>

#include "Point.hpp"
#include "Vector.hpp"

#include "Cells.hpp"
#include "Points.hpp"
#include "Partition.hpp"

#include "FaceLinks.hpp"

#include <boost/range/algorithm_ext/is_sorted.hpp>
#include <boost/range/adaptor/indexed.hpp>

using namespace std;

TetraMeshBuilder::TetraMeshBuilder()
{

}

TetraMeshBuilder::~TetraMeshBuilder()
{
}

void TetraMeshBuilder::setNumberOfFaces(size_t Nf)
{
	m_faces.resize(Nf);
}

void TetraMeshBuilder::setNumberOfPoints(size_t Np)
{
	m_points.resize(Np,Point<3,double>::nan());
}

void TetraMeshBuilder::setNumberOfTetras(size_t Nt)
{
	m_tetraPoints.resize(Nt,TetraByPointID{{0U,0U,0U,0U}});
	m_tetraMaterials.resize(Nt,0U);
	m_tetraFaces.resize(Nt,array<unsigned,4>{{0,0,0,0}});
}

std::size_t TetraMeshBuilder::getNumberOfTetras() const
{
	return m_tetraPoints.size();
}

void TetraMeshBuilder::setPoint(const size_t i,const Point<3,double> P)
{
	m_points[i] = P;
}

void TetraMeshBuilder::setPoint(const size_t i,const array<float,3> P)
{
	setPoint(i,P.data());
}

void TetraMeshBuilder::setPoint(const size_t i,const float* P)
{
	m_points[i] = Point<3,double>{P[0],P[1],P[2]};
}

void TetraMeshBuilder::setPoint(const size_t i,array<double,3> P)
{
	setPoint(i,P.data());
}

void TetraMeshBuilder::setPoint(const size_t i,const double* P)
{
	setPoint(i,Point<3,double>(P));
}

void TetraMeshBuilder::sortTetraPoints()
{
	for(auto& T : m_tetraPoints)
		T = sortIndices(T);
}


/** Orients all tetras to be counterclockwise by swapping the 2nd and 3rd points if needed.
 * If run after sortTetraPoints(), will guarantee that all tetras are in the minimum lexicographical order possible for being
 * CCW oriented.
 */

void TetraMeshBuilder::orientTetras()
{
	for(auto& T : m_tetraPoints)
	{
		// load the points
		array<Point<3,double>,4> P;
		for(unsigned i=0;i<4;++i)
			P[i] = m_points[T[i]];

		// rectify if needed
		if (!isCCW(P))
			swap(T[2],T[3]);
	}
}

bool TetraMeshBuilder::isCCW(TetraByPointID T) const
{
	array<Point<3,double>,4> P;
	for(unsigned i=0;i<4;++i)
		P[i] = m_points[T[i]];
	return isCCW(P);
}

bool TetraMeshBuilder::isCCW(array<Point<3,double>,4> P)
{
	return dot(
		P[3]-P[0],
		cross(P[0],P[1],P[2])) > 0;
}

bool TetraMeshBuilder::isCCW(FaceByPointID F,Point<3,double> p) const
{
	array<Point<3,double>,4> P;
	for(unsigned i=0;i<3;++i)
		P[i] = m_points[F[i]];
	P[3] = p;
	return isCCW(P);
}

TetraByPointID TetraMeshBuilder::sortIndices(TetraByPointID T)
{
	boost::sort(T);
	return T;
}

FaceByPointID TetraMeshBuilder::sortIndices(FaceByPointID F)
{
	boost::sort(F);
	return F;
}


void TetraMeshBuilder::setTetraMaterial(std::size_t i,unsigned IDm)
{
	m_tetraMaterials[i] = IDm;
	m_materialCount = max(m_materialCount,IDm);
}

TetraMesh* TetraMeshBuilder::mesh()
{
	Points* P = new Points();
	P->resize(m_points.size());

	for(unsigned i=0;i<m_points.size();++i)
		P->set(i,m_points[i]);

	TetraCells* T = new TetraCells();
	T->resize(m_tetraPoints.size());

	for(unsigned i=0;i<m_tetraPoints.size();++i)
		T->set(i,m_tetraPoints[i]);

	Partition* p = new Partition();
	p->resize(m_tetraMaterials.size());

	for(unsigned i=0;i<m_tetraMaterials.size();++i)
		p->assign(i,m_tetraMaterials[i]);

	FaceTetraLinks* FT = new FaceTetraLinks();
	FT->resize(m_faces.size());

	for(unsigned i=0;i<m_faces.size();++i)
	{
		FaceTetraLink ft;
		ft.upTet().T = TetraMesh::TetraDescriptor(m_faces[i].tetras[0].IDt);
		ft.upTet().vertexPoint = TetraMesh::PointDescriptor(m_faces[i].tetras[0].IDp_opposite);

		ft.downTet().T = TetraMesh::TetraDescriptor(m_faces[i].tetras[1].IDt);
		ft.downTet().vertexPoint = TetraMesh::PointDescriptor(m_faces[i].tetras[1].IDp_opposite);

		ft.pointIDs = array<TetraMesh::PointDescriptor,3>{{
			TetraMesh::PointDescriptor(m_faces[i].IDps[0]),
			TetraMesh::PointDescriptor(m_faces[i].IDps[1]),
			TetraMesh::PointDescriptor(m_faces[i].IDps[2])}};
		FT->set(i,ft);
	}

	TetraFaceLinks* TF = new TetraFaceLinks();
	TF->resize(m_tetraPoints.size());

	for(unsigned i=1;i<m_tetraPoints.size();++i)
	{
		array<TetraFaceLink,4> tf;

		for(unsigned j=0;j<4;++j)
		{
			unsigned IDfd = m_tetraFaces[i][j];
			unsigned IDfu = m_tetraFaces[i][j] >> 1;
			bool down = m_tetraFaces[i][j] & 1;

			tf[j].faceID =  TetraMesh::DirectedFaceDescriptor(IDfd);
			tf[j].tetraID = TetraMesh::TetraDescriptor(m_faces[IDfu].tetras[down].IDt);
		}

		TF->set(i,tf);
	}

	TetraMesh* M = new TetraMesh;

	M->points(P);
	M->regions(p);
	M->tetraCells(T);
	M->faceTetraLinks(FT);
	M->tetraFaceLinks(TF);

	cout << m_tetraPoints.size() << " tetras, " << m_faces.size() << " faces" << endl;

	return M;
}

unsigned TetraMeshBuilder::sortedTetras() const
{
	unsigned N=1;
	for(unsigned i=1;i<m_tetraPoints.size();++i)
		N += boost::range::is_sorted(m_tetraPoints[i]);
	return N;
}

unsigned TetraMeshBuilder::orientedTetras() const
{
	unsigned N=1;
	for(unsigned i=1;i<m_tetraPoints.size();++i)
		N += isCCW(m_tetraPoints[i]);
	return N;
}

unsigned TetraMeshBuilder::orientedFaces() const
{
	unsigned NflippedFaces=0;

	// skip face 0 (dummy face is degenerate and therefore unoriented)
	for(const auto f : m_faces | drop(1) | boost::adaptors::indexed(1U))
	{
		if (f.value().tetras[0].IDt != 0 && !isCCW(f.value().IDps,m_points[f.value().tetras[0].IDp_opposite]))
			++NflippedFaces;

		if (f.value().tetras[1].IDt != 0 && isCCW(f.value().IDps,m_points[f.value().tetras[1].IDp_opposite]))
		{
			++NflippedFaces;
			cout << "Odd - face " << f.index() << " down-tetra is unexpectedly CCW" << endl;
		}
	}
	return NflippedFaces;
}
