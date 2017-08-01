/*
 * Test_Histograms.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: jcassidy
 */

#include <boost/test/unit_test.hpp>

#include <FullMonteSW/Geometry/TetraMesh.hpp>

#include <FullMonteSW/OutputTypes/OutputDataType.hpp>
#include <FullMonteSW/OutputTypes/OutputData.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap.hpp>

#include "DoseHistogram.hpp"
#include "DoseVolumeHistogramGenerator.hpp"
#include "DoseHistogramCollection.hpp"

#include <FullMonteSW/Geometry/Point.hpp>
//#include <FullMonteSW/Storage/TextFile/TextFileDoseHistogramWriter.hpp>

#include <iostream>
#include <vector>
using namespace std;

BOOST_AUTO_TEST_CASE(constructedDVH)
{
	// construct a tetramesh cube
	vector<Point<3,double>> P{
		Point<3,double>{.0,.0,.0},	// 0
		Point<3,double>{1.,.0,.0},	// 1
		Point<3,double>{.0,1.,.0},	// 2
		Point<3,double>{.0,.0,1.},	// 3
		Point<3,double>{1.,1.,.0},	// 4
		Point<3,double>{1.,.0,1.},	// 5
		Point<3,double>{.0,1.,1.},	// 6
		Point<3,double>{1.,1.,1.}	// 7
	};


	// +y
	// ^ / +z
	// |/
	// +--> +x
	//
	//   6 ---- 7
	//  /|     /|
	// 2 ---- 4 |
 	// | |    | |
	// | |    | |
	// | 3 ---|-5
	// |/     |/
	// 0 ---- 1

	// total distinct edges: 8C2 = 8!/2!/6! = 28

	// cube outer edges (12): 		01 02 03 14 15 24 26 35 36 47 57 67   	differ in only 1 component
	// cube face diagonals (12): 	04 05 06 12 13 17 23 27 37 45 46 56		differ in only 2 components
	// cube interior diagonals (4):	07 16 25 34								differ in all 3  components

	// faces (6): 0142 0351 0362 7635 7426 7415
	//             XX   XX   XX   XX   XX   XX

	// outer edges all mutually compatible
	// face diagonals conflict in pairs: 04-12 05-13 06-23 17-45 27-46 37-56
	// interior diagonals all mutually conflict

	// outer edges (12) appear only once, interior diagonals don't appear at all (0), and rest appear twice (2x12) -> 36

	vector<TetraByPointID> T{
		// the dummy tetra as always (V=0)
		TetraByPointID{0,0,0,0},

		// four corner tetras anchored at 0, 4, 5, 6 (V=1/6)
		TetraByPointID{0,1,2,3},	// 01 02 03 12 13 23  --> !04 !05 !06
		TetraByPointID{1,2,4,7},	// 12 14 17 24 27 47  --> !04 !45 !46
		TetraByPointID{1,3,5,7},	// 13 15 17 35 37 57  --> !05 !45 !56
		TetraByPointID{2,3,6,7},	// 23 26 27 35 37 57  --> !06 !46 !56

		// interior wedge (V=2/6)
		TetraByPointID{1,2,3,7}
	};


	vector<unsigned> Tm{
		0,
		1,
		1,
		1,
		2,
		1
	};

	TetraMesh M;
	M.points();
	M.tetraCells();

	float V=.0f;

	for(const auto t : M.tetras())
	{
		V += get(volume,M,t);
		cout << "Tetra id " << get(id,M,t) << " has volume " << get(volume,M,t) << endl;
	}
	cout << "Total volume " << V << endl;


	SpatialMap<float> phi(
			vector<float>{
				10.0f,
				11.0f,
				22.0f,
				43.0f,
				14.0f,
				05.0f
			},
			AbstractSpatialMap::Volume,
			AbstractSpatialMap::Scalar);

	DoseVolumeHistogramGenerator dvhg;

	dvhg.mesh(&M);
	dvhg.dose(&phi);

	OutputData* o = dvhg.result();

	cout << o->type()->name() << endl;

	DoseHistogramCollection* C = static_cast<DoseHistogramCollection*>(o);



	if (!C)
		throw std::logic_error("Invalid output type");

	BOOST_CHECK_EQUAL(C->count(),2U);							// require 2 regions

	vector<DoseHistogram*> H;

	for(unsigned i=0;i<C->count();++i)
		H.push_back(C->get(i));

	BOOST_CHECK_CLOSE(H[0]->totalMeasure(),5.0f/6.0f,1e-4f);

	BOOST_CHECK_CLOSE(H[1]->totalMeasure(),1.0f/6.0f,1e-4f);


	for(unsigned i=0;i<H.size();++i)
	{
		cout << "Histogram " << i << endl;
		cout << "cmeas   dose" << endl;

		for(const auto e : *H[i])
		{
			cout << fixed << setprecision(1) << setw(6) << e.cmeasure << ' ' << setw(8) << e.dose << endl;
		}
	}

//	TextFileDoseHistogramWriter W;
//	W.fileName("histogram.txt");
//	W.collection(C);
//	W.update();
}
