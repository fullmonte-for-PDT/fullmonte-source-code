/*
 * vtkFullMonteMeshFacesToPolyData.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: jcassidy
 */

#include "vtkFullMonteMeshFacesToPolyData.h"
#include "vtkHelperFunctions.hpp"

#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>



vtkStandardNewMacro(vtkFullMonteMeshFacesToPolyData)

vtkFullMonteMeshFacesToPolyData::vtkFullMonteMeshFacesToPolyData()
{

}

vtkFullMonteMeshFacesToPolyData::~vtkFullMonteMeshFacesToPolyData()
{

}

void vtkFullMonteMeshFacesToPolyData::outputDirectedFaces(bool st)
{
	m_outputDirectedFaces=st;
}

void vtkFullMonteMeshFacesToPolyData::mesh(TetraMesh* m)
{
	m_mesh=m;
}
//
//void vtkFullMonteMeshFacesToPolyData::partition(Partition* p)
//{
//	m_partition=p;
//}
//
//void vtkFullMonteMeshFacesToPolyData::showZeroElements(bool s)
//{
//	m_showZeroElements=s;
//}

void vtkFullMonteMeshFacesToPolyData::update()
{
	if (!m_vtkPD)
	{
		m_vtkPD = vtkPolyData::New();
		vtkPoints *p = vtkPoints::New();
		vtkCellArray*ca = vtkCellArray::New();

		m_vtkPD->SetPoints(p);
		m_vtkPD->SetPolys(ca);
	}

	getVTKPoints(*m_mesh,m_vtkPD->GetPoints());

	if (m_outputDirectedFaces)
		getVTKDirectedTriangleCells(*m_mesh,m_vtkPD->GetPolys());
	else
		getVTKTriangleCells(*m_mesh,m_vtkPD->GetPolys());
}

vtkPolyData* vtkFullMonteMeshFacesToPolyData::output() const
{
	return m_vtkPD;
}
