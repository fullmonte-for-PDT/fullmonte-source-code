/*
 * Test_FullMonte_line_source.cpp
 *
 *  Created on: October 26, 2017
 *      Author: Abed Yassine
 */

#define BOOST_TEST_MODULE "Test_FullMonte_Line_Source"

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <FullMonteSW/Examples/Helpers/fileReader.hpp>
#include <FullMonteSW/Examples/Helpers/chi2test.hpp>

BOOST_AUTO_TEST_SUITE(Test_FullMonte_Line_Source)

BOOST_AUTO_TEST_CASE(FullMonte_Line_Source_colin27)
{
	unsigned packets = 1e7;
	unsigned topNvalues = 4e4;
	float threshold = 1e-8;
	std::string FMFieldDataName = "fluence0";

	// Read data from the new simulation	
	FileReader fr1;
		fr1.packets = packets;
		fr1.filename_vtk.push_back("./../Examples/Data/colin27/FullMonte_fluence_line_0.vtk");
		fr1.find_this = FMFieldDataName;
		fr1.next_field = "\n";
		fr1.parseData();
	// Read data from expected results
	FileReader fr2;
		fr2.packets = packets;
		fr2.filename_vtk.push_back("./../Examples/FullMonte_line_source/ExpectedData/colin27/FullMonte_fluence_line.vtk");
		fr2.find_this = FMFieldDataName;
		fr2.next_field = "\n";
		fr2.parseData();
	
	// Perform statistical tests on the two datasets
	Chi2Test chi2test;
		chi2test.topNvalues = topNvalues;
		chi2test.threshold = threshold;
		chi2test.expectedData = fr2.getVTKResults();
		chi2test.observedData = fr1.getVTKResults();
		chi2test.compute();

	// Check if the test results are within a reasonable bound
	BOOST_CHECK_EQUAL(chi2test.getPvalue(), 1.0);

//     BOOST_CHECK_MESSAGE(chi2test.getPvalue() == 1.0, "\nP value is not 1\n");
	BOOST_CHECK_SMALL((double)chi2test.getChi2Stat(), 0.01);

//     BOOST_CHECK_MESSAGE((double)chi2test.getChi2Stat() < 0.01, "Chi2Stat is greater than 0.01");
}

BOOST_AUTO_TEST_SUITE_END()

