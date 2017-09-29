/*
 * vtkFullMontePacketPositionTraceToPolyData.cpp
 *
 *  Created on: Mar 6, 2016
 *      Author: jcassidy
 */

#include <FullMonteSW/OutputTypes/PacketPositionTraceSet.hpp>
#include <FullMonteSW/OutputTypes/PacketPositionTrace.hpp>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>

#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include "vtkFullMontePacketPositionTraceSetToPolyData.h"

#include "SwigWrapping.hpp"

#include <cmath>
#include <string>
using namespace std;

vtkStandardNewMacro(vtkFullMontePacketPositionTraceSetToPolyData);

vtkFullMontePacketPositionTraceSetToPolyData::~vtkFullMontePacketPositionTraceSetToPolyData()
{
	m_vtkPD->Delete();
	m_vtkWeight->Delete();
	m_vtkTime->Delete();
	m_vtkLength->Delete();
	m_vtkStepCount->Delete();
}

vtkFullMontePacketPositionTraceSetToPolyData::vtkFullMontePacketPositionTraceSetToPolyData()
{
	m_vtkPD = vtkPolyData::New();

		m_vtkWeight = vtkFloatArray::New();
		m_vtkWeight->SetName("Packet Weight");

			m_vtkLogWeight = vtkFloatArray::New();
			m_vtkLogWeight->SetName("log(Packet Weight)");


		m_vtkTime = vtkFloatArray::New();
		m_vtkTime->SetName("Time of flight");

		m_vtkLength = vtkFloatArray::New();
		m_vtkLength->SetName("Path length");

		m_vtkStepCount = vtkUnsignedShortArray::New();
		m_vtkStepCount->SetName("Step count");

		m_vtkTetra = vtkIntArray::New();
		m_vtkTetra->SetName("Tetra");

			m_vtkMaterial = vtkUnsignedShortArray::New();
			m_vtkMaterial->SetName("Material");

}

void vtkFullMontePacketPositionTraceSetToPolyData::source(const PacketPositionTraceSet* q)
{
	m_traces=q;
}

void vtkFullMontePacketPositionTraceSetToPolyData::source(const char* swigPtrString)
{
	SwigPointerInfo pInfo = readSwigPointer(swigPtrString);
	string type(pInfo.type.first, pInfo.type.second-pInfo.type.first);

	if (pInfo.p)
	{
		if (type == "OutputData")
			source(dynamic_cast<const PacketPositionTraceSet*>(static_cast<const OutputData*>(pInfo.p)));
		else if (type == "PacketPositionTraceSet")
			source(static_cast<const PacketPositionTraceSet*>(pInfo.p));
		else
		{
			cout << "ERROR: SWIG pointer '" << swigPtrString << "' is not a PacketPositionTraceSet, actually a " << type << endl;
			source(static_cast<const PacketPositionTraceSet*>(nullptr));
		}
	}
	else
	{
		cout << "ERROR: Failed to convert SWIG pointer '" << swigPtrString << "'" << endl;
		source(static_cast<const PacketPositionTraceSet*>(nullptr));
	}
}

const PacketPositionTraceSet* vtkFullMontePacketPositionTraceSetToPolyData::source() const
{
	return m_traces;
}


void vtkFullMontePacketPositionTraceSetToPolyData::update()
{
	if (!m_traces)
	{
		cout << "ERROR! vtkFullMontePacketPositionTraceSetToPolyData::update() called with null traces" << endl;
		return;
	}

	vtkIdType Nt = m_traces->nTraces();	// number of traces
	cout << Nt << " traces" << endl;

	vtkIdType Np = m_traces->nPoints();	// number of points (total over all traces)
	cout << Np << " points" << endl;


	// get/clear points
	vtkPoints* P = m_vtkPD->GetPoints();
	if (!P)
	{
		P = vtkPoints::New();
		m_vtkPD->SetPoints(P);
	}

	P->SetNumberOfPoints(Np);

	if (m_includeWeight)
		m_vtkWeight->SetNumberOfTuples(Np);

	if (m_includeLogWeight)
		m_vtkLogWeight->SetNumberOfTuples(Np);

	if (m_includeTime)
		m_vtkTime->SetNumberOfTuples(Np);

	if (m_includeLength)
		m_vtkLength->SetNumberOfTuples(Np);

	if (m_includeSteps)
		m_vtkStepCount->SetNumberOfTuples(Np);

	if(m_includeTetra)
		m_vtkTetra->SetNumberOfTuples(Np);

	if(m_includeMaterial)
		m_vtkMaterial->SetNumberOfTuples(Np);

	// cell connectivity
	vtkIdTypeArray* IDps = vtkIdTypeArray::New();
		IDps->SetNumberOfTuples(Np+Nt);

	vtkIdType IDp=0;

	vtkIdType i=0;			// Cell array counter (increments 1x/trace + 1x/point)

	for(const auto trace : m_traces->traces())
	{
		unsigned Nstep=0;
		IDps->SetValue(i,trace->count());
		++i;

		for(const auto step : trace->steps())
		{
			P->SetPoint(IDp,step.pos.data());
			IDps->SetValue(i,IDp);

			if (m_includeWeight)
				m_vtkWeight->SetValue(IDp,step.w);

			if (m_includeTime)
				m_vtkTime->SetValue(IDp,step.t);

			if(m_includeLength)
				m_vtkLength->SetValue(IDp,step.l);

			if (m_includeSteps)
				m_vtkLength->SetValue(IDp,++Nstep);

			if (m_includeLogWeight)
				m_vtkLogWeight->SetValue(IDp,std::log10(step.w));

			if(m_includeTetra)
				m_vtkTetra->SetValue(IDp,step.IDt);

			if(m_includeMaterial)
				m_vtkMaterial->SetValue(IDp,step.IDm);

			++IDp;
			++i;
		}
	}

	// create cell array and assign it to the lines
	vtkCellArray* ca = vtkCellArray::New();
	ca->SetCells(Nt,IDps);
	m_vtkPD->SetLines(ca);

	if(m_includeWeight)
		m_vtkPD->GetPointData()->AddArray(m_vtkWeight);
	if(m_includeLogWeight)
		m_vtkPD->GetPointData()->AddArray(m_vtkLogWeight);
	if(m_includeSteps)
		m_vtkPD->GetPointData()->AddArray(m_vtkStepCount);
	if(m_includeLength)
		m_vtkPD->GetPointData()->AddArray(m_vtkLength);
	if(m_includeTime)
		m_vtkPD->GetPointData()->AddArray(m_vtkTime);
	if(m_includeMaterial)
		m_vtkPD->GetPointData()->AddArray(m_vtkMaterial);
	if(m_includeTetra)
		m_vtkPD->GetPointData()->AddArray(m_vtkTetra);


	Modified();
}

vtkPolyData* vtkFullMontePacketPositionTraceSetToPolyData::getPolyData() const
{
	return m_vtkPD;
}

void vtkFullMontePacketPositionTraceSetToPolyData::includeWeight(bool e)
{
	m_includeWeight=e;
}

void vtkFullMontePacketPositionTraceSetToPolyData::includeLogWeight(bool e)
{
	m_includeLogWeight=e;
}

void vtkFullMontePacketPositionTraceSetToPolyData::includeTime(bool e)
{
	m_includeTime=e;
}

void vtkFullMontePacketPositionTraceSetToPolyData::includeLength(bool e)
{
	m_includeLength=e;
}

void vtkFullMontePacketPositionTraceSetToPolyData::includeSteps(bool e)
{
	m_includeSteps=e;
}

void vtkFullMontePacketPositionTraceSetToPolyData::includeTetra(bool e)
{
	m_includeTetra=e;
}
void vtkFullMontePacketPositionTraceSetToPolyData::includeMaterial(bool e)
{
	m_includeMaterial=e;
}



