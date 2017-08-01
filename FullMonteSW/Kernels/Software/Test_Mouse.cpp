/*
 * Test_Mouse.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: jcassidy
 */

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <FullMonteSW/Config.h>

#include <FullMonteSW/Storage/TIMOS/TIMOSMeshReader.hpp>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>

#include <FullMonteSW/Kernels/Software/TetraSVKernel.hpp>

#include <FullMonteSW/OutputTypes/MCConservationCounts.hpp>

#include <FullMonteSW/OutputTypes/OutputDataSummarize.hpp>
#include <FullMonteSW/Queries/EnergyToFluence.hpp>
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap.hpp>

#include <FullMonteSW/Storage/TIMOS/TIMOSMaterialReader.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSSourceReader.hpp>

#include <string>

using namespace std;



// headers needed to write out VTK files if so enabled
#ifdef WRAP_VTK
#include <vtkCellTypes.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkUnsignedShortArray.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkUnstructuredGridWriter.h>

#include <FullMonteSW/VTK/vtkFullMonteArrayAdaptor.h>
#include <FullMonteSW/VTK/vtkFullMonteTetraMeshWrapper.h>
#include <vtkMergeDataObjectFilter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <vtkGeometryFilter.h>
#include <vtkExtractCells.h>
#endif


BOOST_AUTO_TEST_CASE(mouse)
{
	const unsigned Npkt = 1000000;
	TIMOSMeshReader R;

	// load mesh
	R.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.mesh");
	R.read();
	TetraMesh* M = R.mesh();

	BOOST_REQUIRE_EQUAL(get(num_faces,*M), 629709U);
	BOOST_REQUIRE_EQUAL(get(num_tetras,*M),306774U);
	BOOST_REQUIRE_EQUAL(get(num_points,*M),58245U);

	// create materials
	MaterialSet* MS = new MaterialSet();

	MS->matchedBoundary(false);

	Material* air = new Material(.0f,.0f,.0f,1.0f);
	Material* muscle = new Material(0.23f,10.f,0.9f,1.37f);
	Material* heart = new Material(0.11f,11.f,0.9f,1.37f);
	Material* stomach = new Material(0.21f,17.0f,0.9f,1.37f);
	Material* kidney = new Material(0.12f,12.0f,0.9f,1.37f);
	Material* lung = new Material(0.35f,23.0f,0.9f,1.37f);
	Material* liver = new Material(0.45f,20.0f,0.9f,1.37f);

	MS->exterior(air);
	MS->set(1,muscle);		// muscle
	MS->set(2,muscle);		// brain
	MS->set(3,heart);		// heart
	MS->set(4,muscle);		// bladder
	MS->set(5,stomach);		// stomach
	MS->set(6,muscle);		// spleen
	MS->set(7,kidney);		// kidney
	MS->set(8,kidney);		// kidney
	MS->set(9,lung);		// lung
	MS->set(10,lung);		// lung
	MS->set(11,muscle);		// pancreas
	MS->set(12,liver);		// liver
	MS->set(13,muscle);		// skeleton
	MS->set(14,muscle);		// muscle
	MS->set(15,muscle);		// muscle
	MS->set(16,muscle);		// kidney
	MS->set(17,muscle);		// kidney



	// create source
	Source::Point* pSrc = new Source::Point(1.0f,array<float,3>{{ 10.0f, 41.0f, 11.0f }});	// source in tetra 120569
	Source::Abstract* src = pSrc;



	TetraSVKernel K;
	K.geometry(M);
	K.rouletteWMin(1e-5);
	K.materials(MS);
	K.source(src);
	K.packetCount(Npkt);
	K.threadCount(8);

	cout << "Async start" << endl;
	K.startAsync();

	while(!K.done())
	{
		cout << "Progress: " << setw(8) << fixed << setprecision(3) << 100.0f*K.progressFraction() << endl << flush;
		usleep(1000000);
	}

	K.finishAsync();

	OutputDataSummarize summ(std::cout);

	OutputDataCollection* C = K.results();


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Basic output data checks

	BOOST_CHECK( !C->getByName("") );
	BOOST_CHECK( (!C->getByName("foobar")) );
	BOOST_CHECK_EQUAL(C->size(), 4U);

	MCEventCountsOutput* events = nullptr;
	BOOST_REQUIRE( (events = dynamic_cast<MCEventCountsOutput*>(C->getByName("EventCounts"))) );

	MCConservationCountsOutput* cons = nullptr;
	BOOST_REQUIRE((cons = dynamic_cast<MCConservationCountsOutput*>(C->getByName("ConservationCounts"))));


	SpatialMap<float>* volumeEnergy=nullptr;
	BOOST_REQUIRE((volumeEnergy = dynamic_cast<SpatialMap<float>*>(C->getByName("VolumeEnergy"))));

	SpatialMap<float>* surfaceEnergy=nullptr;
	BOOST_REQUIRE((surfaceEnergy = dynamic_cast<SpatialMap<float>*>(C->getByName("SurfaceExitEnergy"))));




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Conservation checks

	// unusual situations that shouldn't happen ever
	BOOST_WARN_EQUAL(cons->w_abnormal, 0.0);
	BOOST_WARN_EQUAL(cons->w_nohit, 0.0);

	// shouldn't happen in this test case (no time gate)
	BOOST_CHECK_EQUAL(cons->w_time, 0.0);

	// conservation relationships - approximate
	BOOST_CHECK_CLOSE(cons->w_roulette,cons->w_die,1.0f);

	// conservation relationships - exact
	BOOST_CHECK_CLOSE(cons->w_launch, double(Npkt), 1e-6);
	BOOST_CHECK_CLOSE(cons->w_launch+cons->w_roulette, 	cons->w_die+cons->w_exit+cons->w_absorb+cons->w_time+cons->w_abnormal+cons->w_nohit, 1e-4);


	double wAbs = 0.0;
	double wExit = 0.0;

	for(unsigned i=1;i<volumeEnergy->dim();++i)
		wAbs += (*volumeEnergy)[i];

	for(unsigned i=1;i<surfaceEnergy->dim(); ++i)
		wExit += (*surfaceEnergy)[i];

	cout << "Energy balance: launched " << fixed << setprecision(2) << cons->w_launch << " ==> " << wAbs << " absorbed + " << wExit << " exited (=" << wAbs+wExit << " total)" << endl;

	BOOST_CHECK_CLOSE(wAbs,  cons->w_absorb, 1e-4);
	BOOST_CHECK_CLOSE(wExit, cons->w_exit, 1e-4);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Event checksums


	BOOST_WARN_EQUAL(events->Nabnormal,0ULL);

	BOOST_CHECK_EQUAL(events->Nlaunch, Npkt);
	BOOST_CHECK_EQUAL(events->Nexit+events->Ndie+events->Ntime+events->Nnohit,Npkt);
	BOOST_CHECK_EQUAL(events->Ntir+events->Nfresnel+events->Nrefr,events->Ninterface);

	BOOST_CHECK_EQUAL(events->Nabsorb,events->Nscatter+events->Ndie);



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Re-run and make sure results clear properly

	K.packetCount(100000);
	K.runSync();

	OutputDataCollection* C2 = K.results();

	BOOST_CHECK_EQUAL( C2->size(), 4U );

	MCEventCountsOutput* events2 = nullptr;
	BOOST_REQUIRE( (events2 = dynamic_cast<MCEventCountsOutput*>(C2->getByName("EventCounts"))) );

	MCConservationCountsOutput* cons2 = nullptr;
	BOOST_REQUIRE((cons2 = dynamic_cast<MCConservationCountsOutput*>(C2->getByName("ConservationCounts"))));


	SpatialMap<float>* volumeEnergy2=nullptr;
	BOOST_REQUIRE((volumeEnergy2 = dynamic_cast<SpatialMap<float>*>(C2->getByName("VolumeEnergy"))));

	SpatialMap<float>* surfaceEnergy2=nullptr;
	BOOST_REQUIRE((surfaceEnergy2 = dynamic_cast<SpatialMap<float>*>(C2->getByName("SurfaceExitEnergy"))));

	BOOST_CHECK_EQUAL(events2->Nlaunch, 100000U);
	BOOST_CHECK_CLOSE(cons2->w_launch,100000.0f,1e-4f);
	BOOST_CHECK_CLOSE(cons2->w_die + cons2->w_exit + cons2->w_absorb - cons2->w_roulette, 100000.0f,1e-2f);

	double wExit2=0.0f;
	double wAbs2=0.0f;

	for(unsigned i=1;i<volumeEnergy2->dim();++i)
		wAbs2 += (*volumeEnergy2)[i];
	for(unsigned i=1;i<surfaceEnergy2->dim();++i)
		wExit2 += (*surfaceEnergy2)[i];

	BOOST_CHECK_CLOSE(wAbs2 + wExit2, 100000.0f, 1e-2);




	cout << "Fluence conversion" << endl;
	EnergyToFluence FC;
	FC.mesh(M);
	FC.materials(MS);
	FC.source(C->getByName("VolumeEnergy"));
	FC.update();

	cout << "Output" << endl;


#ifdef WRAP_VTK
	vtkFullMonteTetraMeshWrapper* vtkM = vtkFullMonteTetraMeshWrapper::New();
		vtkM->mesh(M);
		vtkM->update();

	/// Build surface rep
	vtkFullMonteArrayAdaptor *vtkVolPhi = vtkFullMonteArrayAdaptor::New();
		vtkVolPhi->source(FC.result());
		vtkVolPhi->array()->SetName("Volume Fluence");
		vtkVolPhi->update();

	vtkUnstructuredGrid* ug = vtkM->blankMesh();
		ug->GetCellData()->AddArray(vtkM->regions());
		ug->GetCellData()->AddArray(vtkVolPhi->array());

	vtkUnstructuredGridWriter *W = vtkUnstructuredGridWriter::New();
		W->SetInputData(ug);
		W->SetFileName("mouse.volume.vtk");
		W->Update();
		W->Delete();
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Values specific to this test case

// check event counts are within 1% of expected (expect failures for small packet counts due to statistical noise)
//	BOOST_CHECK_CLOSE(double(events->Nabsorb), double(Npkt)*375.65, 1.0f);
//
//	BOOST_CHECK_CLOSE(double(events->Nbound), double(Npkt)*166.4, 1.0f);
//
//	BOOST_CHECK_CLOSE(double(events->Ndie), double(Npkt)*0.392, 1.0f);
//	BOOST_CHECK_CLOSE(double(events->Nexit), double(Npkt)*0.607959, 1.0f);
//
//	// energy disposition specific to this test case
//	BOOST_CHECK_CLOSE(cons->w_exit,double(Npkt)*0.06396,1.0f);
//	BOOST_CHECK_CLOSE(cons->w_absorb, double(Npkt)*0.936043,1.0f);

BOOST_AUTO_TEST_CASE(mouseSurf)
{
	const unsigned Npkt=10000000;
	TIMOSMeshReader MR;
	MR.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.mesh");
	MR.read();
	TetraMesh* M = MR.mesh();
	BOOST_REQUIRE(M);
	BOOST_REQUIRE_EQUAL(get(num_points,*M),58245U);
	BOOST_REQUIRE_EQUAL(get(num_tetras,*M),306774U);
	BOOST_REQUIRE_EQUAL(get(num_faces,*M),629709U);


	TIMOSSourceReader SR;
	SR.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.source");
	SR.read();
	Source::Abstract* src = SR.source();
	BOOST_REQUIRE(src);


	TIMOSMaterialReader MATR;
	MATR.filename(FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.opt");
	MATR.read();
	MaterialSet* MS = MATR.materials();
	BOOST_REQUIRE(MS);


	TetraSVKernel SK;
	SK.geometry(M);
	SK.rouletteWMin(1e-5);
	SK.materials(MS);
	SK.source(src);
	SK.packetCount(Npkt);
	SK.threadCount(TEST_THREAD_COUNT);

	cout << "Async start" << endl;
	SK.startAsync();

	while(!SK.done())
	{
		cout << "Progress: " << setw(8) << fixed << setprecision(3) << 100.0f*SK.progressFraction() << endl << flush;
		usleep(1000000);
	}

	SK.finishAsync();

	OutputDataCollection* O = SK.results();

	const MCConservationCountsOutput* cons = dynamic_cast<const MCConservationCountsOutput*>(O->getByName("ConservationCounts"));

	BOOST_REQUIRE(cons);

	// unusual situations that shouldn't happen ever
	BOOST_WARN_EQUAL(cons->w_abnormal, 0.0);
	BOOST_WARN_EQUAL(cons->w_nohit, 0.0);

	// shouldn't happen in this test case (no time gate)
	BOOST_CHECK_EQUAL(cons->w_time, 0.0);

	// conservation relationships - approximate
	BOOST_CHECK_CLOSE(cons->w_roulette,cons->w_die,1.0f);

	// conservation relationships - exact
	BOOST_CHECK_CLOSE(cons->w_launch, double(Npkt), 1e-6);
	BOOST_CHECK_CLOSE(cons->w_launch+cons->w_roulette, 	cons->w_die+cons->w_exit+cons->w_absorb+cons->w_time+cons->w_abnormal+cons->w_nohit, 1e-4);

	BOOST_CHECK_CLOSE(cons->w_exit,double(Npkt)*0.06396,1.0f);
	BOOST_CHECK_CLOSE(cons->w_absorb, double(Npkt)*0.936043,1.0f);



//	const MCEventCountsOutput* e = SK.getResultByType<MCEventCountsOutput>();
//
//	BOOST_WARN_EQUAL(e->Nabnormal,0ULL);
//
//	BOOST_CHECK_EQUAL(e->Nlaunch, Npkt);
//	BOOST_CHECK_EQUAL(e->Nexit+e->Ndie+e->Ntime+e->Nnohit,Npkt);
//	BOOST_CHECK_EQUAL(e->Ntir+e->Nfresnel+e->Nrefr,e->Ninterface);
//
//	BOOST_CHECK_EQUAL(e->Nabsorb,e->Nscatter+e->Ndie);
//
//	// check event counts are within 1% of expected (expect failures for small packet counts due to statistical noise)
//	BOOST_CHECK_CLOSE(double(e->Nabsorb), double(Npkt)*375.65, 1.0f);
//
//	BOOST_CHECK_CLOSE(double(e->Nbound), double(Npkt)*166.4, 1.0f);
//
//	BOOST_CHECK_CLOSE(double(e->Ndie), double(Npkt)*0.392, 1.0f);
//	BOOST_CHECK_CLOSE(double(e->Nexit), double(Npkt)*0.607959, 1.0f);
//
//	const SurfaceExitEnergyMap *S = SK.getResultByType<SurfaceExitEnergyMap>();
//
//	if (!S)
//		throw std::logic_error("Failed to get SurfaceExitEnergyMap");
//
//	EnergyToFluence FC;
//	FC.mesh(&M);
//
//	SurfaceFluenceMap phiS = FC.convertToFluence(*S);
//
//#ifdef WRAP_VTK
//
//	vtkFullMonteTetraMeshWrapper* vtkM = vtkFullMonteTetraMeshWrapper::New();
//		vtkM->mesh(&M);
//		vtkM->update();
//
//	/// Build surface rep
//
//	vtkFullMonteSpatialMapWrapper<vtkFloatArray,float,unsigned> *vtkSurfPhi = vtkFullMonteSpatialMapWrapper<vtkFloatArray,float,unsigned>::New();
//		vtkSurfPhi->source(phiS.get());
//		vtkSurfPhi->array()->SetName("Surface Fluence J/cm2");
//		vtkSurfPhi->update();
//
//	// Create fields for cell data
//	vtkFieldData* vtkSurfaceField = vtkFieldData::New();
//		vtkSurfaceField->AddArray(vtkSurfPhi->array());
//
//	// Create data object holding only the cell data, no geometry
//	vtkDataObject* vtkSurfDO = vtkDataObject::New();
//		vtkSurfDO->SetFieldData(vtkSurfaceField);
//
//	cout << "VTK faces: " << vtkM->faces()->GetNumberOfCells() << " fluence entries: " << vtkSurfPhi->array()->GetNumberOfTuples() << endl;
//
//	// Merge data object onto geometry
//	vtkMergeDataObjectFilter *vtkMergeSurfaceFluence = vtkMergeDataObjectFilter::New();
//		vtkMergeSurfaceFluence->SetDataObjectInputData(vtkSurfDO);
//		vtkMergeSurfaceFluence->SetInputData(vtkM->faces());
//		vtkMergeSurfaceFluence->SetOutputFieldToCellDataField();
//
//	const unsigned region = 0U;
//
//	std::vector<unsigned> IDfs = M.getRegionBoundaryTris(region);
//
//	vtkIdList* surfaceIDs = vtkIdList::New();
//
//	boost::for_each(IDfs, [surfaceIDs](unsigned i){ surfaceIDs->InsertNextId(i); });
//
//	vtkExtractCells *vtkExtractSurface = vtkExtractCells::New();
//		vtkExtractSurface->SetInputConnection(vtkMergeSurfaceFluence->GetOutputPort());
//		vtkExtractSurface->SetCellList(surfaceIDs);
//		vtkExtractSurface->Update();
//
//	vtkGeometryFilter* ug2pd = vtkGeometryFilter::New();
//		ug2pd->SetInputConnection(vtkExtractSurface->GetOutputPort());
//
//	vtkPolyDataWriter *W = vtkPolyDataWriter::New();
//		W->SetInputConnection(ug2pd->GetOutputPort());
//		W->SetFileName("mouse.surface.vtk");
//		W->Update();
//		W->Delete();
//#endif
}
