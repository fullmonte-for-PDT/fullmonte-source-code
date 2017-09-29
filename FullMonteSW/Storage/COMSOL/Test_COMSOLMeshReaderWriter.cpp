/*
 * Test_COMSOLMeshReaderWriter.cpp
 *
 *  Created on: July 24, 2017
 *      Author: Siyun Li
 */

#include <string>
#include <boost/range/adaptor/indexed.hpp>
#include "COMSOLMeshReader.hpp"
#include "COMSOLMeshWriter.hpp"
#include "../TIMOS/TIMOSMeshReader.hpp"
#include "../TIMOS/TIMOSMeshWriter.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Points.hpp>
#include <FullMonteSW/Geometry/Cells.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>

#include <FullMonteSW/Geometry/PTTetraMeshBuilder.hpp>

#include <iostream>
#include <array>
#include <algorithm>

#include <FullMonteSW/Config.h>

using namespace std;


// This test checks that mesh read by COMSOLMeshReader can be written using both COMSOLMeshWriter and TIMOSMeshWriter. The example mesh used is HeadAndNeckModel.mphtxt.
BOOST_AUTO_TEST_CASE(COMSOLReader)
{
	string fn=FULLMONTE_DATA_DIR "/COMSOL/HeadAndNeckModel.mphtxt";

	COMSOLMeshReader R;
	R.filename(fn.c_str());
	R.read();

	// check that tetras were provided oriented as would be expected from COMSOL format
	BOOST_CHECK_EQUAL(R.builder()->orientedTetras(), 817921U);

	TetraMesh* M = R.mesh();
	BOOST_TEST_MESSAGE("Read mesh with " << M->points()->size() << " points and " << M->tetraCells()->size() << " tetras");

	// check mesh stats (Np, Nt, Nf)
	BOOST_CHECK_EQUAL(M->tetraCells()->size(),		817921U);
	BOOST_CHECK_EQUAL(M->points()->size(), 			150714U);

	// check first point is dummy
	BOOST_CHECK_EQUAL(M->points()->get(0)[0], 0.0);
	BOOST_CHECK_EQUAL(M->points()->get(0)[1], 0.0);
	BOOST_CHECK_EQUAL(M->points()->get(0)[2], 0.0);

	// check first actual data point
	BOOST_CHECK_EQUAL(M->points()->get(1)[0], 0.010864994449698172);
	BOOST_CHECK_EQUAL(M->points()->get(1)[1], -0.0265017779392984);
	BOOST_CHECK_EQUAL(M->points()->get(1)[2], 0.0047499999999999981);

	BOOST_CHECK_CLOSE(M->points()->get(150713)[0], -0.013501137814002561, 1e-3);
	BOOST_CHECK_CLOSE(M->points()->get(150713)[1], -0.076956617249456849, 1e-3);
	BOOST_CHECK_CLOSE(M->points()->get(150713)[2], 0.023275786054624333, 1e-3);


	// check first tetra is dummy
	BOOST_CHECK_EQUAL(M->regions()->get(0), 0U);
	BOOST_CHECK( (M->tetraCells()->get(0) == TetraByPointID{{0U,0U,0U,0U}}) );
	BOOST_CHECK( (M->tetraCells()->get(1) == TetraByPointID{{1U, 13U, 15U, 14U}} ));


	for(unsigned i=1;i<M->tetraCells()->size();++i)
	{
		const auto T = M->tetraCells()->get(i);
		BOOST_CHECK( T[0] < T[1] );
		BOOST_CHECK( T[1] < T[2] );
		BOOST_CHECK( T[1] < T[3] );
	}

	COMSOLMeshWriter W;
	W.filename("headneck.FullMonte.mphtxt");
	W.mesh(M);
	W.setElementTypeCount(1);
	W.setElementDetails(0, "3 tet", 4);
	W.write();
	TIMOSMeshWriter TW;
	TW.filename("headneck.mesh");
	TW.mesh(M);
	TW.write();
}


// This test uses a simple mesh defined manually and checks for COMSOLMeshWriter
BOOST_AUTO_TEST_CASE(COMSOLWriterSimple)
{
        string ofn = "COMSOLMeshWriterTest.mesh";

        Points* P = new Points();
        TetraCells* T = new TetraCells();
        Partition* R = new Partition();

        P->resize(5);
        P->set(0,Point<3,double>{ .0, .0, .0} );
        P->set(1,Point<3,double>{ 101.0, 102.0, 103.0 });
        P->set(2,Point<3,double>{ 220.0, 221.0, 222.0 });
        P->set(3,Point<3,double>{ 330.0, 331.0, 332.0 });
        P->set(4,Point<3,double>{ -400.0, -401.000001, 402.22222 });


        R->resize(3);
        R->assign(0,0);
        R->assign(1,1);
        R->assign(2,1);

        T->resize(3);
        T->set(0,TetraByPointID {{0U,0U,0U,0U}});
        T->set(1,TetraByPointID {{1U, 2U, 3U, 4U}});
        T->set(2,TetraByPointID {{1U, 2U, 3U, 5U}});

        TetraMesh M;

        M.points(P);
        M.tetraCells(T);
        M.regions(R);

        COMSOLMeshWriter W;

        W.filename(ofn);
        W.mesh(&M);
	W.setElementTypeCount(1);
	W.setElementDetails(0, "3 tet", 4);
        W.write();
}

// This test checks for writing a COMSOL mesh from TIMOS format, which is compatible with FullMonte.
BOOST_AUTO_TEST_CASE(COMSOLWriterFull)
{
	string ofn = "Colin27.mphtxt";
	
	string fn = FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.mesh";
	TIMOSMeshReader R;
	R.filename(fn);
	R.read();
	TetraMesh* M = R.mesh();

	COMSOLMeshWriter W;
	W.filename(ofn);
	W.mesh(M);
	W.setElementTypeCount(1);
	W.setElementDetails(0, "3 tet", 4);
	W.write();
}
