/*
 * Test_FullMonte_MMC_self.cpp
 *
 *  Created on: July 21, 2017
 *      Author: Siyun Li
 */

#define BOOST_TEST_MODULE "Test_FullMonte_Self"

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <FullMonteSW/Examples/Helpers/fileReader.hpp>
#include <FullMonteSW/Examples/Helpers/chi2test.hpp>

BOOST_AUTO_TEST_SUITE(Test_FullMonte_Self)

BOOST_AUTO_TEST_CASE(FullMonte_Self_colin27)
{
	unsigned packets = 1e6;
	unsigned topNvalues = 1e5;
	float threshold = 1e-8;
	std::string FMFieldDataName = "Volume%20" "energy";

	// Read data from the new simulation	
	FileReader fr1;
		fr1.packets = packets;
		fr1.filename_vtk.push_back("./../Examples/Data/colin27/FullMonte_energy_point_0.vtk");
		fr1.find_this = FMFieldDataName;
		fr1.next_field = "\n";
		fr1.parseData();
	// Read data from expected results
	FileReader fr2;
		fr2.packets = packets;
		fr2.filename_vtk.push_back("./../Examples/FullMonte_self/ExpectedData/colin27/FullMonte_energy_point.vtk");
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
	BOOST_CHECK_SMALL((double)chi2test.getChi2Stat(), 0.01);
}

BOOST_AUTO_TEST_SUITE_END()

