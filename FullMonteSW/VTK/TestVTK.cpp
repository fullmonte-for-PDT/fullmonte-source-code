/*
 * TestVTK.cpp
 *
 *  Created on: May 25, 2017
 *      Author: jcassidy
 */

#include <boost/test/unit_test.hpp>

#include <FullMonteSW/Storage/TIMOS/TIMOSMeshReader.hpp>

#include <vtkUnstructuredGrid.h>
#include <vtkCellData.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkUnstructuredGridReader.h>

#include "vtkFullMonteTetraMeshWrapper.h"

#include <FullMonteSW/Config.h>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Points.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>

#include "vtkFullMonteMeshFromUnstructuredGrid.h"


/** Load TIM-OS Digimouse mesh, save as vtk file
 *
 * Uses TIMOSMeshReader to read, vtkFullMonteTetraMeshWrapper to adapt, and VTK libraries to write
 * Then reads the written file, converts back to mesh, and checks that cell/point/region counts are OK.
 *
 */

BOOST_AUTO_TEST_CASE(ReadWriteReadDigi)
{
	TIMOSMeshReader R;

	R.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.mesh");
	R.read();
	TetraMesh* M = R.mesh();

	BOOST_TEST_CHECKPOINT("Read TIM-OS format file");

	vtkFullMonteTetraMeshWrapper* VTKM = vtkFullMonteTetraMeshWrapper::New();
	VTKM->mesh(M);
	VTKM->update();

	vtkUnstructuredGrid* ug = VTKM->regionMesh();

	// check that size is correct
	BOOST_CHECK_EQUAL(ug->GetNumberOfCells(), 306774U);
	BOOST_CHECK_EQUAL(ug->GetNumberOfPoints(),58245U);

	// Check that there is a regions field
	vtkAbstractArray* region;
	BOOST_CHECK((region = ug->GetCellData()->GetScalars("Region")) != nullptr);

	if (region)
		BOOST_CHECK_EQUAL(region->GetNumberOfTuples(),306774U);

	vtkUnstructuredGridWriter* W = vtkUnstructuredGridWriter::New();
	W->SetFileName("mouse.vtk");
	W->SetInputData(ug);
	W->Update();
	W->Delete();


	BOOST_TEST_CHECKPOINT("Wrote vtk file");


	vtkUnstructuredGridReader *VTKR = vtkUnstructuredGridReader::New();
	VTKR->SetFileName("mouse.vtk");
	VTKR->Update();
	vtkUnstructuredGrid* ug2 = VTKR->GetOutput();


	BOOST_TEST_CHECKPOINT("Read test-generated vtk file");

	vtkFullMonteMeshFromUnstructuredGrid* FM = vtkFullMonteMeshFromUnstructuredGrid::New();
	FM->unstructuredGrid(ug2);
	FM->update();
	TetraMesh* M2 = FM->mesh();

	BOOST_REQUIRE(M2 != nullptr);
	BOOST_CHECK_EQUAL(M2->points()->size(), 58245U);
	BOOST_CHECK_EQUAL(M2->tetraCells()->size(),306774U);
	BOOST_CHECK_EQUAL(M2->regions()->size(),306774U);
}
