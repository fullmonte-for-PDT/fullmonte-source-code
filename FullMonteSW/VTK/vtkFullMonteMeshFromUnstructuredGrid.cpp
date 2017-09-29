/*
 * vtkFullMonteMeshFromUnstructuredGrid.cpp
 *
 *  Created on: May 24, 2017
 *      Author: jcassidy
 */

#include <vtkPoints.h>
#include <vtkUnstructuredGrid.h>
#include <vtkObject.h>
#include <vtkCellData.h>
#include <vtkCellTypes.h>
#include <vtkObjectFactory.h>

#include "SwigWrapping.hpp"

#include <vtkTclUtil.h>

class TetraMesh;

#include <FullMonteSW/Geometry/PTTetraMeshBuilder.hpp>

#include "vtkFullMonteMeshFromUnstructuredGrid.h"

vtkStandardNewMacro(vtkFullMonteMeshFromUnstructuredGrid)

vtkFullMonteMeshFromUnstructuredGrid::vtkFullMonteMeshFromUnstructuredGrid()
{
}

vtkFullMonteMeshFromUnstructuredGrid::~vtkFullMonteMeshFromUnstructuredGrid()
{
}

void vtkFullMonteMeshFromUnstructuredGrid::unstructuredGrid(vtkUnstructuredGrid* ug)
{
	m_ug = ug;
}

void vtkFullMonteMeshFromUnstructuredGrid::regionLabelFieldName(const std::string fieldName)
{
	m_regionLabelFieldName = fieldName;
}

void vtkFullMonteMeshFromUnstructuredGrid::regionLabelFieldName(const char* fieldName)
{
	m_regionLabelFieldName = fieldName;
}


void vtkFullMonteMeshFromUnstructuredGrid::update()
{
	PTTetraMeshBuilder builder;

	bool hasReferenceToPointZero=false;
	bool hasZeroTetra=false;

	// check valid structure, and whether there's a zero tetra & zero point present
	// normally for FullMonte, tetra zero is a special dummy (0,0,0,0) and point zero is not part of any tetra

	if (!m_ug)
	{
		cout << "vtkFullMonteMeshFromUnstructuredGrid called with null data" << endl;
		return;
	}

	for(unsigned t=0;t<m_ug->GetNumberOfCells();++t)
	{
		vtkIdType* cellPts;
		vtkIdType  nCellPts;

		if (m_ug->GetCellType(t) != VTK_TETRA)
		{
			cout << "ERROR in vtkFullMonteMeshFromUnstructuredGrid: non-tetra cell" << endl;
			return;
		}

		m_ug->GetCellPoints(t, nCellPts, cellPts);

		if (nCellPts != 4)
		{
			cout << "ERROR in vtkFullMonteMeshFromUnstructuredGrid: tetra cell with nCellPts != 4" << endl;
			return;
		}

		if (t==0)
		{
			hasZeroTetra = true;
			for(unsigned i=0;i<nCellPts;++i)
				hasZeroTetra &= cellPts[i] == 0;
		}
		else
			for(unsigned i=0;i<nCellPts;++i)
				hasReferenceToPointZero |= cellPts[i] == 0;
	}

	bool insertZeroPoint = hasReferenceToPointZero;
	bool insertZeroTetra = !hasZeroTetra;

	if (insertZeroPoint)
		cout << "vtkFullMonteMeshFromUnstructuredGrid: inserting zero point as existing zero point is referenced by a tetra" << endl;
	if (insertZeroTetra)
		cout << "vtkFullMonteMeshFromUnstructuredGrid: inserting zero cell as existing zero cell has nonzero point references" << endl;


	// extract the points

	if (!m_builder)
		m_builder = new PTTetraMeshBuilder();

	m_builder->setNumberOfPoints(m_ug->GetNumberOfPoints()+insertZeroPoint);

	if (insertZeroPoint)
	{
		Point<3,double> P;
		m_ug->GetPoint(1,P.data());
		m_builder->setPoint(0,P);
	}

	for(unsigned i=0;i<m_ug->GetNumberOfPoints();++i)
	{
		Point<3,double> P;
		m_ug->GetPoint(i,P.data());
		m_builder->setPoint(i+insertZeroPoint, P);
	}



	// extract tetra cells

	m_builder->setNumberOfTetras(m_ug->GetNumberOfCells()+insertZeroTetra);

	if (insertZeroTetra)
		m_builder->setTetra(0,TetraByPointID{0U,0U,0U,0U},0U);

	for(unsigned i=0;i<m_ug->GetNumberOfCells(); ++i)
	{
		vtkIdType* p;
		vtkIdType Npc=0;

		m_ug->GetCellPoints(i,Npc,p);		// already checked they're tetras and have 4 points

		TetraByPointID IDps;

		for(unsigned j=0;j<4;++j)
			IDps[j] = p[j]+insertZeroPoint;

		m_builder->setTetra(i+insertZeroTetra, IDps);
	}



	// extract the partition if present

	vtkCellData* cd = m_ug->GetCellData();
	vtkDataArray* regions=nullptr;

	if (!cd)
	{
		cout << "vtkFullMonteMeshFromUnstructuredGrid: No cell data provided; assuming mesh is all region 1" << endl;
	}
	else if ((regions = cd->GetScalars(m_regionLabelFieldName.c_str())))
	{
	}
	else if ((regions = cd->GetScalars()))
	{
		const char *name = regions->GetName();
		if (name != m_regionLabelFieldName)
			cout << "vtkFullMonteMeshFromUnstructuredGrid: Could not find specified region label field '" << m_regionLabelFieldName << "', using default field '" << name << "' instead" << endl;
	}
	else
	{
		cout << "vtkFullMonteMeshFromUnstructuredGrid: Cell data without scalars provided; assuming mesh is all region 1" << endl;
		cd=nullptr;
	}

	if(regions)
	{
		if (regions->GetTuple1(0) != 0 && !insertZeroTetra)
			cout << "vtkFullMonteMeshFromUnstructuredGrid: Cell 0 is not assigned to material 0" << endl;

		for(unsigned i=0;i<m_ug->GetNumberOfCells();++i)
			m_builder->setTetraMaterial(i+insertZeroTetra, regions->GetTuple1(i));
	}
	else
		for(unsigned i=1;i<m_builder->getNumberOfTetras();++i)
			m_builder->setTetraMaterial(i,1);
}

TetraMesh* vtkFullMonteMeshFromUnstructuredGrid::mesh()
{
	m_builder->build();
	return m_builder->mesh();
}

const char* vtkFullMonteMeshFromUnstructuredGrid::meshPtr() const
{
	m_builder->build();

	TetraMesh* M = m_builder->mesh();

	stringstream ss;
	uint64_t p = endianSwap(reinterpret_cast<uint64_t>(M));

	ss << '_' << setw(16) << setfill('0') << hex << p << "_p_TetraMesh";

	return ss.str().c_str();
}
