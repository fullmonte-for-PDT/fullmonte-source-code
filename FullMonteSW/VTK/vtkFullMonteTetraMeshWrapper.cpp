/*
 * vtkFullMonteTetraMeshWrapper.cpp
 *
 *  Created on: Mar 3, 2016
 *      Author: jcassidy
 */

#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkIdTypeArray.h>
#include <vtkObjectFactory.h>
#include <vtkUnsignedShortArray.h>
#include <vtkUnstructuredGrid.h>

#include <boost/range/adaptor/indexed.hpp>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>
#include <FullMonteSW/Geometry/Point.hpp>
#include <FullMonteSW/Geometry/Points.hpp>


#include "vtkFullMonteTetraMeshWrapper.h"

#include "SwigWrapping.hpp"

using namespace std;

vtkStandardNewMacro(vtkFullMonteTetraMeshWrapper)

void vtkFullMonteTetraMeshWrapper::mesh(const char* mptr)
{
	SwigPointerInfo pi = readSwigPointer(mptr);

	string s(pi.type.first,pi.type.second-pi.type.first);

	cout << "vtkFullMonteTetraMeshWrapper::mesh(const char* mptr) received type " << s << " (SWIG string " << mptr << ")" << endl;
	mesh(static_cast<const TetraMesh*>(pi.p));
}

const TetraMesh* vtkFullMonteTetraMeshWrapper::mesh() const
{
	return m_mesh;
}

vtkPolyData* vtkFullMonteTetraMeshWrapper::faces() const
{
	vtkPolyData *pd = vtkPolyData::New();
	pd->SetPoints(points());
	pd->SetPolys(m_faces);
	return pd;
}


/** Copy the TetraMesh faces, including the 0 element (dummy containing 0,0,0)
 *
 */

void getVTKTriangleCells(const TetraMesh& M,vtkCellArray* ca)
{
	assert(ca);

	size_t Nf=0;

	// Create triangle ID array
	vtkIdTypeArray *ids = vtkIdTypeArray::New();
	ids->SetNumberOfComponents(1);

	for(const auto f : M.faces())
	{
		const auto IDps = get(points,M,f);

		// copy regular elements 1..Nt to tetras 1..Nt
		ids->InsertNextValue(3);
		for(unsigned k=0;k<3;++k)
			ids->InsertNextValue((vtkIdType)(IDps[k].value()));
		++Nf;
	}

	ca->SetCells(Nf, ids);
	ids->Delete();
}

/** Copy the TetraMesh directed faces, including the zero element
 *
 */

void getVTKDirectedTriangleCells(const TetraMesh& M,vtkCellArray* ca)
{
	assert(ca);

	size_t Nf=0;

	// Create triangle ID array
	vtkIdTypeArray *ids = vtkIdTypeArray::New();
	ids->SetNumberOfComponents(1);

	for(const auto f : M.directedFaces())
	{
		const auto IDps = get(points,M,f);

		ids->InsertNextValue(3);
		for(unsigned k=0;k<3;++k)
			ids->InsertNextValue((vtkIdType)(IDps[k].value()));
		++Nf;
	}

	cout << "getVTKDirectedTriangleCells returned " << Nf << " faces (" << get(num_faces,M) << " undirected, " << get(num_directed_faces,M) << " directed)" << endl;

	ca->SetCells(Nf, ids);
	ids->Delete();
}


//#include "SwigWrapping.hpp"
//
//template<typename T>struct SwigConversionTraits;
//
//template<>struct SwigConversionTraits<FilterBase<int>>{ static const std::string name; };
//
//const std::string SwigConversionTraits<FilterBase<int>>::name = "FilterBase<int>";

//#include <FullMonteSW/Geometry/Filters/TriFilterRegionBounds.hpp>

template<typename T,typename U>U* castFromTo(void* p)
{
	return static_cast<U*>(static_cast<T*>(p));
}

//class TriFilterRegionBounds;
//
//template<typename T>T* swigCast(const char* fromType)
//{
//	// grab value and type from SWIG text string, check validity (non-null)
//	SwigPointerInfo i = readSwigPointer(fromType);
//	const std::string t(i.type.first, i.type.second-i.type.first);
//
//	const std::string toType = SwigConversionTraits<typename std::remove_cv<T>::type>::name;
//
//	T* o=nullptr;
//
//	if (t == "TriFilterRegionBounds")
//		o = dynamic_cast<T*>(static_cast<TriFilterRegionBounds*>(i.p));
//
//	std::cout << "INFO: swigCast('" << fromType << "') -> " << i.p << " [" << t << "] -> " << o << " [" << toType << "]" << std::endl;
//
//	return o;
//}

vtkFullMonteTetraMeshWrapper::vtkFullMonteTetraMeshWrapper()
{
	m_points=vtkPoints::New();
	m_tetras=vtkCellArray::New();

	m_regions=vtkUnsignedShortArray::New();
		m_regions->SetName("Region");

	m_faces=vtkCellArray::New();
}

vtkFullMonteTetraMeshWrapper::~vtkFullMonteTetraMeshWrapper()
{
	m_points->Delete();
	m_regions->Delete();
	m_tetras->Delete();
	m_faces->Delete();
}

void vtkFullMonteTetraMeshWrapper::mesh(const TetraMesh* m)
{
	m_mesh = m;
	update();
}


vtkPoints* vtkFullMonteTetraMeshWrapper::points() const
{
	return m_points;
}

vtkUnsignedShortArray* vtkFullMonteTetraMeshWrapper::regions() const
{
	return m_regions;
}

void vtkFullMonteTetraMeshWrapper::update()
{
	assert(m_points);
	assert(m_regions);
	assert(m_tetras);
	assert(m_mesh);

	getVTKPoints(*m_mesh,m_points);
	getVTKTetraCells(*m_mesh,m_tetras);
	getVTKTetraRegions(*m_mesh,m_regions);

	//assert(m_faces);
	//getVTKTriangleCells(*mesh(),m_faces);
	Modified();
}

vtkUnstructuredGrid* vtkFullMonteTetraMeshWrapper::regionMesh() const
{
	vtkUnstructuredGrid* ug = blankMesh();
	ug->GetCellData()->SetScalars(m_regions);
	return ug;
}

vtkUnstructuredGrid* vtkFullMonteTetraMeshWrapper::blankMesh() const
{
	vtkUnstructuredGrid* ug = vtkUnstructuredGrid::New();
	ug->SetPoints(m_points);
	ug->SetCells(VTK_TETRA,m_tetras);
	return ug;
}


/** Extract vtkPoints from a TetraMesh. Point 0 is a duplicate of point 1 to avoid goofy bounding boxes when the object is
 * located far from the origin.
 *
 * @param 	M		The mesh
 * @param	P		Destination point set (will be overwritten)
 */

void getVTKPoints(const TetraMesh& M,vtkPoints* P)
{
	assert(P);

	unsigned Np = M.points()->size();

	P->SetNumberOfPoints(Np);

	for(unsigned i=1;i<Np;++i)
	{
		Point<3,double> Pc = get(coords,M,TetraMesh::PointDescriptor(i));
		P->SetPoint(i,Pc.data());
	}

	P->SetPoint(0,P->GetPoint(1));
}



/** Copy the TetraMesh tetras, including the 0 element (dummy containing 0,0,0,0)
 *
 */

void getVTKTetraCells(const TetraMesh& M,vtkCellArray* ca)
{
	assert(ca);

	size_t Nt=0;

	// Create tetra ID array
	vtkIdTypeArray *ids = vtkIdTypeArray::New();
	ids->SetNumberOfComponents(1);

	for(const auto t : M.tetras())
	{
		TetraByPointID IDps = get(points,M,t);

		// copy regular elements 1..Nt to tetras 1..Nt
		ids->InsertNextTuple1(4);
		for(unsigned k=0;k<4;++k)
		{
			ids->InsertNextTuple1((vtkIdType)(IDps[k]));
			assert(IDps[k] < get(num_points,M));
		}
		++Nt;
	}

	ca->SetCells(Nt, ids);
	ids->Delete();
}


/** Get region codes for all tetra elements (assign tetra 0 -> region 0 always)
 * @param	M		The mesh
 * @param	R		vtkUnsignedShortArray to hold region codes (will be overwritten)
 */

void getVTKTetraRegions(const TetraMesh& M,vtkUnsignedShortArray* R)
{
	assert(R);

	R->SetNumberOfTuples(M.tetraCells()->size());
	R->SetNumberOfComponents(1);

	for(const auto t : M.tetras() | boost::adaptors::indexed(0U))
		R->SetValue(t.index(), get(region,M,t.value()));

	R->SetTuple1(0,0);
}
