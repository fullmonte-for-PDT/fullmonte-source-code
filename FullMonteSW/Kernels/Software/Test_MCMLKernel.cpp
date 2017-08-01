/*
 * Test_MCML.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

/** Load the sample file provided with MCML and run it
 *
 */

#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include <FullMonteSW/Config.h>

#include <FullMonteSW/Storage/MCML/MCMLInputReader.hpp>
#include <FullMonteSW/Storage/MCML/MCMLCase.hpp>

#include <FullMonteSW/Kernels/Software/MCMLKernel.hpp>

#include <FullMonteSW/Storage/TextFile/TextFileMatrixWriter.hpp>

#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap2D.hpp>

#ifdef WRAP_VTK
#include <vtkPolyData.h>
#include <vtkPolyDataWriter.h>
#include <FullMonteSW/VTK/vtkFullMontePacketPositionTraceSetToPolyData.h>
#include <FullMonteSW/OutputTypes/PacketPositionTraceSet.hpp>
#endif

BOOST_AUTO_TEST_CASE(MCMLSampleTraces)
{
	MCMLInputReader R;

	R.filename(FULLMONTE_DATA_DIR "/MCML/sample/sample.mci");
	R.read();

	BOOST_REQUIRE_EQUAL( R.cases(), 2U );

	// work with first case
	R.start();

	MCMLCase* C = R.current();

	MCMLKernelWithTraces K;
//	K.packetCount(C->packets());
	K.packetCount(100);
	K.threadCount(8);
	K.rouletteWMin(C->rouletteThreshold());
	K.geometry(C->geometry());
	K.materials(C->materials());

	K.runSync();

	OutputDataCollection* DC = K.results();

	BOOST_REQUIRE_EQUAL( DC->size(), 4U );

	MCConservationCountsOutput* cons = nullptr;

	BOOST_REQUIRE( cons = dynamic_cast<MCConservationCountsOutput*>(DC->getByName("ConservationCounts")) );
	cout << "Absorbed: " << fixed << setprecision(6) << cons->w_absorb << endl;
	cout << "Exited:   " << fixed << setprecision(6) << cons->w_exit << endl;
	cout << "Roulette: " << fixed << setprecision(6) << cons->w_die << endl;
	cout << "Launched: " << fixed << setprecision(6) << cons->w_launch << endl;

	PacketPositionTraceSet* tr = nullptr;
	BOOST_REQUIRE( (tr = dynamic_cast<PacketPositionTraceSet*>(DC->getByName("PacketTraces"))) );

#ifdef WRAP_VTK
	vtkFullMontePacketPositionTraceSetToPolyData* TR = vtkFullMontePacketPositionTraceSetToPolyData::New();
	TR->includeTetra(true);
	TR->includeMaterial(false);
	TR->source(tr);
	TR->update();

	vtkPolyDataWriter* W = vtkPolyDataWriter::New();
	W->SetFileName("mcmltraces.vtk");
	W->SetInputData(TR->getPolyData());
	W->Update();
	W->Delete();
#endif
}

BOOST_AUTO_TEST_CASE(MCMLSample)
{
	MCMLInputReader R;

	R.filename(FULLMONTE_DATA_DIR "/MCML/sample/sample.mci");
	R.read();

	BOOST_REQUIRE_EQUAL( R.cases(), 2U );

	// work with first case
	R.start();

	MCMLCase* C = R.current();

	MCMLKernel K;
	K.packetCount(C->packets());
	K.threadCount(8);
	K.rouletteWMin(C->rouletteThreshold());
	K.geometry(C->geometry());
	K.materials(C->materials());

	{
		cout << "Atomic Accumulation" << endl;
		boost::timer::auto_cpu_timer t;
		K.runSync();
	}


	OutputDataCollection* DC = K.results();

	BOOST_REQUIRE_EQUAL( DC->size(), 3U );

	cout << "Accumulations: " << K.cylAbsorptionScorer().accumulator().accumulationCount() << endl;
	cout << "Retries due to conflict: " << K.cylAbsorptionScorer().accumulator().retryCount() << endl;

	MCConservationCountsOutput* cons = nullptr;

	BOOST_REQUIRE( (cons = dynamic_cast<MCConservationCountsOutput*>(DC->getByName("ConservationCounts"))) );
	cout << "Absorbed: " << fixed << setprecision(6) << cons->w_absorb << endl;
	cout << "Exited:   " << fixed << setprecision(6) << cons->w_exit << endl;
	cout << "Roulette: " << fixed << setprecision(6) << cons->w_die << endl;
	cout << "Launched: " << fixed << setprecision(6) << cons->w_launch << endl;

	MCEventCountsOutput* e = nullptr;
	BOOST_REQUIRE( (e = dynamic_cast<MCEventCountsOutput*>(DC->getByName("EventCounts"))) );
	cout << "Died:     " << e->Ndie << endl;
	cout << "Exit:     " << e->Nexit << endl;

	SpatialMap2D<float>* abs = nullptr;
	BOOST_REQUIRE( (abs=dynamic_cast<SpatialMap2D<float>*>(DC->getByName("CylAbsorbedEnergy"))) );

	cout << "  Total absorption in map: " << abs->sum() << endl;
	cout << "    Writing to fullmonte_sample.A_rz.txt" << endl;

	TextFileMatrixWriter W;
	W.filename("fullmonte_sample.A_rz.txt");
	W.source(abs);
	W.write();
}


BOOST_AUTO_TEST_CASE(MCMLSampleQ)
{
	MCMLInputReader R;

	R.filename(FULLMONTE_DATA_DIR "/MCML/sample/sample.mci");
	R.read();

	BOOST_REQUIRE_EQUAL( R.cases(), 2U );

	// work with first case
	R.start();

	MCMLCase* C = R.current();

	MCMLKernelQ K;
	K.cylAbsorptionScorer().accumulator().queueSize(1024);
	K.packetCount(C->packets());
	K.threadCount(8);
	K.randSeed(2);
	K.rouletteWMin(C->rouletteThreshold());
	K.geometry(C->geometry());
	K.materials(C->materials());

	{
		cout << "Queued Accumulation" << endl;
		boost::timer::auto_cpu_timer t;
		K.runSync();
	}


	OutputDataCollection* DC = K.results();

	BOOST_REQUIRE_EQUAL( DC->size(), 3U );

	MCConservationCountsOutput* cons = nullptr;

	BOOST_REQUIRE( (cons = dynamic_cast<MCConservationCountsOutput*>(DC->getByName("ConservationCounts"))) );
	cout << "Absorbed: " << fixed << setprecision(6) << cons->w_absorb << endl;
	cout << "Exited:   " << fixed << setprecision(6) << cons->w_exit << endl;
	cout << "Roulette: " << fixed << setprecision(6) << cons->w_die << endl;
	cout << "Launched: " << fixed << setprecision(6) << cons->w_launch << endl;

	SpatialMap2D<float>* abs = nullptr;
	BOOST_REQUIRE( (abs=dynamic_cast<SpatialMap2D<float>*>(DC->getByName("CylAbsorbedEnergy"))) );

	cout << "  Total absorption in map: " << abs->sum() << endl;
	cout << "    Writing to fullmonte_sample.A_rzQ.txt" << endl;

	TextFileMatrixWriter W;
	W.filename("fullmonte_sample.A_rzQ.txt");
	W.source(abs);
	W.write();
}

