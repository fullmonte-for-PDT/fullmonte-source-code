/*
 * TestQueries.cpp
 *
 *  Created on: May 24, 2017
 *      Author: jcassidy
 */

#include <boost/test/unit_test.hpp>

#include "TetrasNearPoint.hpp"
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Points.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSMeshReader.hpp>

#include "TetraEnclosingPointByLinearSearch.hpp"

#include <boost/range/adaptor/indexed.hpp>

#include <FullMonteSW/Config.h>

#ifdef WRAP_VTK
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkUnsignedIntArray.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkDataObject.h>
#include <vtkMergeDataObjectFilter.h>
#include <vtkFieldData.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>

#include <FullMonteSW/VTK/vtkFullMonteTetraMeshWrapper.h>
#endif

const TetraMesh*	digimouse_mesh=nullptr;

struct Digimouse
{
	Digimouse()
	{
		TIMOSMeshReader R;

		R.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.mesh");
		R.read();

		digimouse_mesh = R.mesh();

		if (!(
			digimouse_mesh->points()->size() == 58245U &&
			digimouse_mesh->tetraCells()->size() == 306774U))
		{
			BOOST_TEST_MESSAGE("Digimouse did not load properly");
			delete digimouse_mesh;
			digimouse_mesh=nullptr;
		}
	}

	~Digimouse()
	{
		delete digimouse_mesh;
		digimouse_mesh=nullptr;
	}
};

BOOST_GLOBAL_FIXTURE(Digimouse);

/** Select tetras from Digimouse using TetrasNearPoint
 * Check the correct number are selected, and write out a labeled mesh to VTK.
 *
 */

BOOST_AUTO_TEST_CASE(TetrasNearPointOnMouse)
{
	BOOST_REQUIRE(digimouse_mesh);

	Point<3,double> centre{17.6, 50.5, 7.6};
	float radius = 5.0f;

	TetrasNearPoint TP;

	TP.mesh(digimouse_mesh);

	TP.centre(centre);
	TP.radius(radius);

	std::vector<unsigned> IDts;

	for(TP.start(); !TP.done(); TP.next())
	{
		//cout << "Tetra " << TP.currentTetraID() << endl;
		IDts.push_back(TP.currentTetraID());
	}

	BOOST_CHECK_EQUAL(IDts.size(), 6791ULL);

#ifdef WRAP_VTK

	// create mesh wrapper
	vtkFullMonteTetraMeshWrapper* VTKM = vtkFullMonteTetraMeshWrapper::New();

	VTKM->mesh(digimouse_mesh);
	VTKM->update();


	// create label field for selected tetras
	vtkUnsignedIntArray* labels = vtkUnsignedIntArray::New();

	labels->SetName("Selected");
	labels->SetNumberOfTuples(digimouse_mesh->tetraCells()->size());

	for(unsigned i=0;i<labels->GetNumberOfTuples();++i)
		labels->SetTuple1(i,0);

	for(const auto t : IDts)
		labels->SetTuple1(t,1);


	// merge selected labels onto digimouse_mesh

	vtkFieldData* selectField = vtkFieldData::New();
		selectField->AddArray(labels);

	vtkDataObject* selectObject = vtkDataObject::New();
		selectObject->SetFieldData(selectField);

	vtkMergeDataObjectFilter* merge = vtkMergeDataObjectFilter::New();
		merge->SetInputData(VTKM->regionMesh());
		merge->SetDataObjectInputData(selectObject);
		merge->SetOutputFieldToCellDataField();


	// write out

	vtkUnstructuredGridWriter *W = vtkUnstructuredGridWriter::New();

	W->SetInputConnection(merge->GetOutputPort());
	W->SetFileName("TetrasNearPoint.selected.vtk");
	W->Update();
	W->Delete();


	// create sphere indicating radius for selection, and save it as well

	vtkSphereSource* sph = vtkSphereSource::New();
		sph->SetCenter(centre[0],centre[1],centre[2]);
		sph->SetRadius(radius);
		sph->SetThetaResolution(10);
		sph->SetPhiResolution(10);
		sph->Update();

	vtkPolyDataWriter *PW = vtkPolyDataWriter::New();
	PW->SetInputData(sph->GetOutput());
	PW->SetFileName("TetrasNearPoint.sphere.vtk");
	PW->Update();
	PW->Delete();



#endif
}




BOOST_AUTO_TEST_CASE(PointLocate)
{
	BOOST_REQUIRE(digimouse_mesh);

	TetraEnclosingPointByLinearSearch TP;

	vector<pair<Point<3,double>,unsigned>> stim{
		make_pair(Point<3,double>{ 17.6, 31.1, 5.8 }, 187093U),		// inside
		make_pair(Point<3,double>{ 24.3, 45.4, 13.5}, 151382U),		// inside
		make_pair(Point<3,double>{ 7.8, 17.6,  3.2 }, 1169U),		// inside right paw
		make_pair(Point<3,double>{ 5.8, 17.9,  3.2 }, 0U)			// outside right paw
	};

#ifdef WRAP_VTK
	vtkPoints* pq = vtkPoints::New();		// query points
	vtkPoints* pt = vtkPoints::New();		// tetra points

	vtkCellArray* ca = vtkCellArray::New();

	vtkPolyData* pd = vtkPolyData::New();
	pd->SetPoints(pq);
	pd->SetVerts(ca);

	vtkUnstructuredGrid* ug = vtkUnstructuredGrid::New();
	ug->SetPoints(pt);
#endif

	TP.mesh(digimouse_mesh);

	for(const auto s : stim | boost::adaptors::indexed(0U))
	{
		TP.point(s.value().first);
		TP.update();

		BOOST_CHECK_EQUAL(TP.tetra().value(), s.value().second);
		cout << endl;

#ifdef WRAP_VTK
		vtkIdType ids[4];

		ids[0] = pq->InsertNextPoint(s.value().first.data());
		ca->InsertNextCell(1,ids);

		array<Point<3,double>,4> p = get(point_coords,*digimouse_mesh,TP.tetra());

		for(unsigned i=0;i<4;++i)
			ids[i] = pt->InsertNextPoint(p[i].data());
		ug->InsertNextCell(VTK_TETRA,4,ids);
#endif
	}

#ifdef WRAP_VTK
	vtkPolyDataWriter *PW = vtkPolyDataWriter::New();
	PW->SetFileName("querypoints.vtk");
	PW->SetInputData(pd);
	PW->Update();
	PW->Delete();

	vtkUnstructuredGridWriter *UW = vtkUnstructuredGridWriter::New();
	UW->SetFileName("resulttets.vtk");
	UW->SetInputData(ug);
	UW->Update();
	UW->Delete();
#endif
}
