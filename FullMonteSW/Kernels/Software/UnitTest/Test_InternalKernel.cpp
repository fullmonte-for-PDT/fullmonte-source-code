/*
 * Test_InternalKernel.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: jcassidy
 */

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>

#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkUnsignedShortArray.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkThreshold.h>
#include <vtkDataObject.h>
#include <vtkGeometryFilter.h>

#include <FullMonteSW/VTK/vtkFullMonteMeshFromUnstructuredGrid.h>
#include <FullMonteSW/VTK/vtkFullMonteMeshFacesToPolyData.h>
#include <FullMonteSW/VTK/vtkFullMonteArrayAdaptor.h>

#include <FullMonteSW/Queries/DirectedSurfaceOfRegion.hpp>
#include <FullMonteSW/Queries/EnergyToFluence.hpp>
#include <FullMonteSW/Queries/DoseSurfaceHistogramGenerator.hpp>

#include <FullMonteSW/OutputTypes/OutputDataSummarize.hpp>
#include <FullMonteSW/Queries/DoseHistogramCollection.hpp>
#include <FullMonteSW/Queries/DoseHistogram.hpp>



#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>
#include <FullMonteSW/Geometry/Queries/PartitionFacesOnRegionBounds.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>

#include <FullMonteSW/Kernels/Software/TetraSVKernel.hpp>

#include <FullMonteSW/Config.h>

#include <string>
using namespace std;

#include <FullMonteSW/Storage/TIMOS/TIMOSMeshReader.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSMaterialReader.hpp>
#include <FullMonteSW/Storage/TextFile/TextFileDoseHistogramWriter.hpp>


BOOST_AUTO_TEST_CASE(bladderInternal)
{
	string meshfn(FULLMONTE_DATA_DIR "/bladder/bladder&rectum&prostate.mesh.vtk");
	string optfn(FULLMONTE_DATA_DIR "/bladder/bladder_clear.opt");

	TIMOSMaterialReader MR;
	MR.filename(optfn);
	MR.read();
	MaterialSet* mats = MR.materials();

	vtkUnstructuredGridReader* VTKR = vtkUnstructuredGridReader::New();
		VTKR->SetFileName(meshfn.c_str());
		VTKR->Update();

	vtkUnstructuredGrid* ug = VTKR->GetOutput();

	BOOST_REQUIRE_EQUAL( ug->GetNumberOfPoints(), 2304939U	);
	BOOST_REQUIRE_EQUAL( ug->GetNumberOfCells(),  14150643U	);

	BOOST_TEST_CHECKPOINT("VTK mesh loaded");

	vtkFullMonteMeshFromUnstructuredGrid* VTKM = vtkFullMonteMeshFromUnstructuredGrid::New();
		VTKM->unstructuredGrid(ug);
		VTKM->regionLabelFieldName("");
		VTKM->update();

	TetraMesh* M = VTKM->mesh();

	BOOST_REQUIRE_EQUAL( get(num_tetras,*M), 14150644U );
	BOOST_REQUIRE_EQUAL( get(num_points,*M), 2304940U );

	BOOST_TEST_CHECKPOINT("VTK mesh converted");

	Source::Point src;
	src.position(array<float,3>{{ 2.8f, 48.8f, -1400.9f }}); // bladder



//	/// TEMPORARY DIGIMOUSE CODE
//	TIMOSMeshReader R;
//	R.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.mesh");
//	R.read();
//
//	TIMOSMaterialReader MR;
//	MR.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.opt");
//	MR.read();
//
//	MaterialSet* mats = MR.materials();
//
//	TetraMesh* M = R.mesh();
//
//
//	Source::Point src;
//	src.position(array<float,3>{{ 20.7f, 49.8f, 15.1f }}); // Digimouse
//	/// END



	PartitionFacesOnRegionBounds PR;

		PR.mesh(M);
		PR.update();

	Partition* organSurfaces = PR.result();

	cout << "Organ surface partition has " << organSurfaces->size() << " elements" << endl;

	vtkUnsignedShortArray* vtkP = vtkUnsignedShortArray::New();
		vtkP->SetNumberOfTuples(organSurfaces->size());
		vtkP->SetName("Partition bool");

	for(unsigned i=0;i<organSurfaces->size();++i)
		vtkP->SetValue(i,organSurfaces->get(i));



	////// Get the directed surface partition
	DirectedSurfaceOfRegion DS;
		DS.mesh(M);
		DS.update();

	Partition* P = DS.region();

	vtkUnsignedShortArray* vtkPart = vtkUnsignedShortArray::New();
	vtkPart->SetNumberOfTuples(P->size());
	for(unsigned i=0;i<P->size();++i)
		vtkPart->SetValue(i,P->get(i));

	vtkPart->SetName("Partition Region");

	cout << "Partition regions have " << vtkPart->GetNumberOfTuples() << " entries" << endl;





	// Output all directed faces

	vtkFullMonteMeshFacesToPolyData *VTKF = vtkFullMonteMeshFacesToPolyData::New();
		VTKF->mesh(M);
		VTKF->outputDirectedFaces(true);
		VTKF->update();

	vtkPolyData *pd = VTKF->output();




	TetraSVKernel K;
	K.packetCount(1000000);
	K.rouletteWMin(1e-5);
	K.threadCount(8);
	K.geometry(M);
	K.materials(mats);
	K.source(&src);
	M->internalFacesForFluenceCounting(organSurfaces);
	K.runSync();

	OutputDataCollection* C = K.results();

	cout << "Kernel done" << endl;

	OutputDataSummarize summ(cout);
	summ.visit(C);

	//const unsigned Nfd =

	SpatialMap<float>* Edir = nullptr;
	BOOST_REQUIRE( (Edir=dynamic_cast<SpatialMap<float>*>(C->getByName("DirectedSurfaceEnergy"))) );
	//BOOST_CHECK_EQUAL( phiD->size(), Nfd);

	EnergyToFluence EF;
	EF.mesh(M);
	EF.source(Edir);
	EF.update();
	OutputData* phiDirD = EF.result();

	SpatialMap<float>* phiDir = nullptr;
	BOOST_REQUIRE( (phiDir=dynamic_cast<SpatialMap<float>*>(phiDirD)) );

	DS.source(phiDir);
	DS.update();

	OutputData* exitingD = DS.total();
	SpatialMap<float>* total = nullptr;
	BOOST_REQUIRE( (total=dynamic_cast<SpatialMap<float>*>(exitingD)) );


	// Set up dose-surface histogram

	DoseSurfaceHistogramGenerator* DSHG = new DoseSurfaceHistogramGenerator();

	DSHG->mesh(M);
	DSHG->dose(total);
	DSHG->partition(P);
	DSHG->update();

	DoseHistogramCollection* DSHC = dynamic_cast<DoseHistogramCollection*>(DSHG->result());

	DoseHistogram* bladderDSH = DSHC->get(1);

	auto it = bladderDSH->end();
	it--;

	cout << "Created DSH with " << bladderDSH->dim() << " elements and " << it->cmeasure << " total area" << endl;

	for(const auto e : *bladderDSH)
	{
		cout << setw(8) << "cmeasure=" << e.cmeasure << " dose=" << e.dose << endl;
	}

	TextFileDoseHistogramWriter DW;
	DW.filename("bladder.dsh.txt");
	DW.collection(DSHC);
	DW.write();


	// Write out regions 1-4 surfaces to .vtk file

	vtkFullMonteArrayAdaptor* vtkPhi = vtkFullMonteArrayAdaptor::New();
	vtkPhi->source(total);
	vtkPhi->update();

	vtkAbstractArray* a = vtkPhi->array();
	a->SetName("Internal fluence");

	pd->GetCellData()->AddArray(vtkP);
	pd->GetCellData()->AddArray(vtkPart);
	pd->GetCellData()->AddArray(a);

	pd->GetCellData()->SetActiveScalars("Partition Region");

	vtkThreshold  *TH = vtkThreshold::New();
		TH->ThresholdBetween(0.5,10.0);
		TH->SetInputData(pd);
		TH->SetInputArrayToProcess(0,0,0,vtkDataObject::FIELD_ASSOCIATION_CELLS,"Partition Region");
		TH->Update();

	vtkGeometryFilter* G = vtkGeometryFilter::New();
		G->SetInputConnection(TH->GetOutputPort());

	vtkPolyDataWriter *PW = vtkPolyDataWriter::New();
		PW->SetFileName("bladder.surf.vtk");
		PW->SetInputConnection(G->GetOutputPort());
		PW->Update();
		PW->Delete();
}

