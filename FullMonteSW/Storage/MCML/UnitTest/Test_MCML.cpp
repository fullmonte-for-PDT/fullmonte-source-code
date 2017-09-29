/*
 * Test_MCML.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#include <boost/test/unit_test.hpp>

#include "../MCMLInputReader.hpp"
#include "../MCMLOutputReader.hpp"

#include "../MCMLCase.hpp"

#include <FullMonteSW/Geometry/Layered.hpp>
#include <FullMonteSW/Geometry/Layer.hpp>
#include <FullMonteSW/Geometry/MaterialSet.hpp>
#include <FullMonteSW/Geometry/Material.hpp>

#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>
#include <FullMonteSW/OutputTypes/OutputData.hpp>

#include <FullMonteSW/Storage/TextFile/TextFileMatrixWriter.hpp>

BOOST_AUTO_TEST_CASE(ReadSample)
{
	MCMLInputReader R;

	R.filename("sample.mci");
	R.read();

	R.start();

	MCMLCase* c=nullptr;

	BOOST_CHECK_EQUAL( R.cases(), 2U );

	// check first case
	BOOST_REQUIRE( (c=R.current()) );
	BOOST_CHECK_EQUAL( c->geometry()->layerCount(), 3U );
	BOOST_CHECK_EQUAL( c->materials()->size(), 5U );
	BOOST_CHECK_EQUAL( c->outputFilename(), "sample.mco" );

	// advance to second case
	R.next();

	// advance to end and check that we're done
	R.next();
	BOOST_CHECK( R.done() );
}

BOOST_AUTO_TEST_CASE(ReadSampleOutput)
{
	MCMLOutputReader R;

	R.filename("sample.mco");
	R.read();



	OutputDataCollection * C = R.result();



	TextFileMatrixWriter W;

	W.width(11);
	W.precision(4);

	OutputData* d;

	if ((d = C->getByName("Absorption_rz")))
	{
		W.filename("sample.A_rz.txt");
		W.source(d);
		W.write();
	}
}

