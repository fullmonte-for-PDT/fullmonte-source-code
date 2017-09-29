/*
 * vtkDoseHistogramWrapper.cpp
 *
 *  Created on: Aug 12, 2016
 *      Author: jcassidy
 */

#include "vtkDoseHistogramWrapper.h"

#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkAbstractArray.h>
#include <vtkObjectFactory.h>

#include <string>
#include "SwigWrapping.hpp"

#include <FullMonteSW/OutputTypes/AbstractSpatialMap.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap.hpp>
#include <FullMonteSW/Queries/DoseHistogram.hpp>

#include <boost/range/adaptor/indexed.hpp>

#include <sstream>
#include <iostream>

using namespace std;

vtkDoseHistogramWrapper::vtkDoseHistogramWrapper()
{
	m_vtkTable = vtkTable::New();

	std::stringstream ss;

	vtkFloatArray* vtkArea = vtkFloatArray::New();

	switch(m_measureType)
	{
	case Surface:
		ss << "Surface area"; break;
	case Volume:
		ss << "Volume "; break;
	default: break;
	}
	switch(m_measureType)
	{
	case Relative:
		ss << "%"; break;
	default: break;
	}
	m_vtkTable->AddColumn(vtkArea);

	vtkFloatArray* vtkDose = vtkFloatArray::New();
	vtkDose->SetName("Dose");
	m_vtkTable->AddColumn(vtkDose);
}

vtkDoseHistogramWrapper::~vtkDoseHistogramWrapper()
{
	m_vtkTable->Delete();
}

void vtkDoseHistogramWrapper::source(const char *mptr)
{
	SwigPointerInfo pInfo = readSwigPointer(mptr);
	string type(pInfo.type.first, pInfo.type.second-pInfo.type.first);

	if (pInfo.p)
	{
		if (type == "OutputData")
			source(static_cast<OutputData*>(pInfo.p));
		else
		{
			cout << "ERROR: SWIG pointer '" << mptr << "' is not a VolumeAbsorbedEnergyDensityMap, actually a " << type << endl;
			source(static_cast<OutputData*>(nullptr));
		}
	}
	else
	{
		cout << "ERROR: Failed to convert SWIG pointer '" << mptr << "'" << endl;
		source(static_cast<OutputData*>(nullptr));
	}
}

void vtkDoseHistogramWrapper::source(OutputData* D)
{
	if (auto* p = dynamic_cast<DoseHistogram*>(D))
	{
		m_histogram = p;
		update();
	}
	else
	{
		m_histogram = nullptr;
		cout << "ERROR: vtkDoseHistogramWrapper::source(OutputData*) can't be cast to DoseHistogram" << endl;
	}
}


void vtkDoseHistogramWrapper::update()
{
	if (!m_histogram)
	{
		std::cout << "ERROR: vtkDoseHistogramWrapper::update() with null histogram pointer" << std::endl;
		return;
	}

	m_vtkTable->SetNumberOfRows(m_histogram->dim());

	const auto h0=m_histogram->begin();

	vtkFloatArray* x = vtkFloatArray::SafeDownCast(m_vtkTable->GetColumn(0));
	vtkFloatArray* y = vtkFloatArray::SafeDownCast(m_vtkTable->GetColumn(1));

	for(unsigned i=0;i<m_histogram->dim();++i)
	{
		float dose = (h0+i)->dose;
		float cmeas = (h0+i)->cmeasure;
		float cdf = (h0+i)->cdf;

		switch(m_measureMode)
		{
		case Relative:
			x->SetValue(i,dose);
			y->SetValue(i,1.0f-cdf);
			break;

		case Absolute:
			x->SetValue(i,dose);
			y->SetValue(i,cmeas);
			break;
		}
	}
}

vtkTable* vtkDoseHistogramWrapper::table() const
{
	return m_vtkTable;
}

vtkStandardNewMacro(vtkDoseHistogramWrapper)
