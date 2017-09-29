/*
 * Test_FullMonte_MMC_Comparison.cpp
 *
 *  Created on: July 21, 2017
 *      Author: Siyun Li
 */


#define BOOST_TEST_MODULE "Test_FullMonte_MMC_Comparison"

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <FullMonteSW/Examples/Helpers/fileReader.hpp>
#include <FullMonteSW/Examples/Helpers/chi2test.hpp>

BOOST_AUTO_TEST_SUITE(Test_FullMonte_MMC_Comparison)

BOOST_AUTO_TEST_CASE(FullMonte_MMC_mouse)
{
	unsigned packets = 1e6;
	unsigned topNvalues = 1e4;
	float threshold = 1e-8;
	std::string FMFieldDataName = "Volume%20" "energy";

	// Read data from FullMonte and MMC results
	FileReader fr;
		fr.packets = packets;
		fr.filename_vtk.push_back("./../Examples/Data/mouse/FullMonte_energy_point_0.vtk");
		fr.filename_txt.push_back("./../Examples/FullMonte_MMC_Comparison/ExpectedData/mouse/MMC_energy_point.dat");
		fr.find_this = FMFieldDataName;
		fr.next_field = "\n";
		fr.data_index = 1;
		fr.parseData();

	// Perform statistical tests on the results
	Chi2Test chi2test;
		chi2test.topNvalues = topNvalues;
		chi2test.threshold = threshold;
		chi2test.expectedData = fr.getTxtResults();
		chi2test.observedData = fr.getVTKResults();
		chi2test.compute();
	
	// Check if the test results are within a reasonable bound
	BOOST_CHECK_EQUAL(chi2test.getPvalue(), 1.0);
	BOOST_CHECK_CLOSE(chi2test.getChi2Stat(), 0.0003, 10);
}


BOOST_AUTO_TEST_CASE(FullMonte_MMC_colin27)
{
        unsigned packets = 1e6;
        unsigned topNvalues = 1e5;
        float threshold = 1e-8;
        std::string FMFieldDataName = "Volume%20" "energy";

        // Read data from FullMonte and MMC results
        FileReader fr;
                fr.packets = packets;
                fr.filename_vtk.push_back("./../Examples/Data/colin27/FullMonte_energy_point_0.vtk");
                fr.filename_txt.push_back("../Examples/FullMonte_MMC_Comparison/ExpectedData/colin27/MMC_energy_point.dat");
                fr.find_this = FMFieldDataName;
                fr.next_field = "\n";
                fr.data_index = 1;
                fr.parseData();

        // Perform statistical tests on the results
        Chi2Test chi2test;
                chi2test.topNvalues = topNvalues;
                chi2test.threshold = threshold;
                chi2test.expectedData = fr.getTxtResults();
                chi2test.observedData = fr.getVTKResults();
                chi2test.compute();

        // Check if the test results are within a reasonable bound
        BOOST_CHECK_EQUAL(chi2test.getPvalue(), 1.0);
        BOOST_CHECK_CLOSE(chi2test.getChi2Stat(), 0.006, 10);
}


BOOST_AUTO_TEST_SUITE_END()
