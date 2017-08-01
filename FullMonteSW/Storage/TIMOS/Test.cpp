/*
 * Test.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: jcassidy
 */

#include <string>
#include <boost/range/adaptor/indexed.hpp>
#include "TIMOSMeshReader.hpp"
#include "TIMOSMeshWriter.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Points.hpp>
#include <FullMonteSW/Geometry/Cells.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>

#include <FullMonteSW/Geometry/PTTetraMeshBuilder.hpp>

#include "TIMOSSourceReader.hpp"
#include <FullMonteSW/Geometry/Sources/Composite.hpp>
#include <FullMonteSW/Geometry/Sources/Point.hpp>
#include <FullMonteSW/Geometry/Sources/Volume.hpp>

#include <iostream>
#include <array>
#include <algorithm>

#include <FullMonteSW/Config.h>

using namespace std;

BOOST_AUTO_TEST_CASE(TIMOSMeshReaderDigimouse)
{
	string fn=FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.mesh";

	TIMOSMeshReader R;

	R.filename(fn);
	R.read();

	// check that tetras were provided oriented as would be expected from TIMOS format
	BOOST_CHECK_EQUAL(R.builder()->orientedTetras(), 306774U);

	TetraMesh* M = R.mesh();

	BOOST_TEST_MESSAGE("Read mesh with " << M->points()->size() << " points and " << M->tetraCells()->size() << " tetras");

	// check mesh stats (Np, Nt, Nf)
	BOOST_CHECK_EQUAL(M->tetraCells()->size(),		306774U);
	BOOST_CHECK_EQUAL(M->points()->size(), 			58245U);
	BOOST_CHECK_EQUAL(M->faceTetraLinks()->size(), 	629709U);

	// check first point is dummy
	BOOST_CHECK_EQUAL(M->points()->get(0)[0], 0.0);
	BOOST_CHECK_EQUAL(M->points()->get(0)[1], 0.0);
	BOOST_CHECK_EQUAL(M->points()->get(0)[2], 0.0);

	// check first actual data point
	BOOST_CHECK_EQUAL(M->points()->get(1)[0], 18.4);
	BOOST_CHECK_EQUAL(M->points()->get(1)[1], 10.4);
	BOOST_CHECK_EQUAL(M->points()->get(1)[2], 1.2);

	BOOST_CHECK_CLOSE(M->points()->get(58243)[0], 14.799419, 1e-1);
	BOOST_CHECK_CLOSE(M->points()->get(58243)[1], 37.590242, 1e-1);
	BOOST_CHECK_CLOSE(M->points()->get(58243)[2], 13.199609, 1e-1);


	// check first tetra is dummy
	BOOST_CHECK_EQUAL(M->regions()->get(0), 0U);
	BOOST_CHECK( (M->tetraCells()->get(0) == TetraByPointID{{0U,0U,0U,0U}}) );

	BOOST_CHECK( (M->tetraCells()->get(1) == TetraByPointID{{ 90U, 36551U, 42510U, 43895U }} ));
	BOOST_CHECK_EQUAL(M->regions()->get(1), 1U);

	// note loader will reorder tetras to ascending order of point ID then orient ccw by swapping IDps[2], IDps[3]
	BOOST_CHECK_EQUAL(M->regions()->get(306773),1U);
	BOOST_CHECK( ( M->tetraCells()->get(306773) == TetraByPointID{{ 57070U, 57657U, 58244U, 58239U }}) );

	// check it's not just assigning 1's to everything
	BOOST_CHECK_EQUAL(M->regions()->get(306759), 10U);

	for(unsigned i=1;i<M->tetraCells()->size();++i)
	{
		const auto T = M->tetraCells()->get(i);
		BOOST_CHECK( T[0] < T[1] );
		BOOST_CHECK( T[1] < T[2] );
		BOOST_CHECK( T[1] < T[3] );
	}


	TIMOSMeshWriter W;
	W.mesh(M);
	W.filename("mouse.out.mesh");
	W.write();

	cout << "Done" << endl;
}

BOOST_AUTO_TEST_CASE(TIMOSMeshWriterSimple)
{
	string ofn = "TIMOSMeshWriterTest.mesh";

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

	TIMOSMeshWriter W;

	W.filename(ofn);
	W.mesh(&M);

	W.write();
}

BOOST_AUTO_TEST_CASE(TIMOSSourceReaderDigimouseVolume)
{
	string fn = FULLMONTE_DATA_DIR "/TIM-OS/mouse/mouse.source";

	TIMOSSourceReader R;

	R.filename(fn);
	R.read();

	Source::Abstract* src = R.source();
	BOOST_REQUIRE(src);

	const Source::Composite* s;
	BOOST_REQUIRE( (s = dynamic_cast<const Source::Composite*>(src)) );

	// check source count and total power
	BOOST_CHECK_EQUAL(s->count(), 436U);

	double totalWeight=0.0;
	for(const Source::Abstract* src : s->elements())
		totalWeight += src->power();

	BOOST_CHECK_CLOSE(totalWeight, 1e7,1e-2);

	const Source::Volume* sFirstTetra;

	Source::Composite::const_range srcRange = s->elements();

	// check cirst tetra ID & power
	auto srcIt = begin(srcRange);

	BOOST_REQUIRE( (sFirstTetra = dynamic_cast<const Source::Volume*>(*srcIt)) );
	BOOST_CHECK_EQUAL(sFirstTetra->elementID(),4460U);
	BOOST_CHECK_CLOSE(sFirstTetra->power(), 62597.0f,1e-2f);


	// check last tetra ID & power
	advance(srcIt,s->count()-1);
	const Source::Volume* sLastTetra;

	BOOST_REQUIRE( (sLastTetra  = dynamic_cast<const Source::Volume*>(*srcIt)) );
	BOOST_CHECK_EQUAL(sLastTetra->elementID(),280315U);
	BOOST_CHECK_CLOSE(sLastTetra->power(), 3094.0f,1e-2f);
}

//R.filename(fn);
//
//cout << "Total " << mat.size() << " materials" << endl;
//cout << "Total " << src.size() << " source" << endl;
//
//const auto& __reg = reg | boost::adaptors::indexed(0U);
//for(auto r = begin(__reg); r != end(__reg); ++r)
//	cout << setw(2) << r.index() << ": " << r->label << "    (" << r->colour[0] << ',' << r->colour[1] << ',' << r->colour[2] << ')' << endl;
//
////	for(const TIMOS::Material& m : opt.mat)
////		cout << "  " <<
//
//for(const SourceDescription* s : src)
//	cout << *s << endl;
//
//for(const Material& m : mat)
//	cout << m << endl;
//
////	for(const TIMOS::Source& s : src)
////		cout << "  " << s << endl;
////
////	cout << "Coding by region? " << opt.by_region << endl;
////	cout << "Matched boundary? " << opt.matched << endl;
////	cout << "  External n=" << opt.n_ext << endl;
//
//
//vector<SourceDescription*> src = R.sources();

//
//BOOST_AUTO_TEST_CASE(TIMOSSourceWriter)
//{
//	string fn;
//
//	TIMOSSourceWriter W;
//	W.filename();
//
//	TIMOSSourceReader R;
//	R.filename(fn);
//
//}
