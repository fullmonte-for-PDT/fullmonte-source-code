/*
 * Test_FullMonte_composite.cpp
 *
 *  Created on: August 3, 2017
 *      Author: Siyun Li
 */

#define BOOST_TEST_MODULE "Test_FullMonte_Composite"

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <FullMonteSW/Examples/Helpers/fileReader.hpp>
#include <FullMonteSW/Examples/Helpers/chi2test.hpp>


BOOST_AUTO_TEST_SUITE(Test_FullMonte_Composite)

BOOST_AUTO_TEST_CASE(FullMonte_composite_colin27)
{
        unsigned packets = 1e6;
        unsigned topNvalues = 1e5;
        float threshold = 1e-8;
        std::string FMFieldDataName = "Volume%20" "energy";

	// Read results from separate point source simulations
        FileReader fr1;
                fr1.packets = packets;
                for (unsigned i = 0; i < 2; ++i)
                {
                        std::stringstream ss;
                        ss << "./../Examples/FullMonte_composite/ExpectedData/colin27/FullMonte_energy_composite_point_" << i << ".vtk";
                        fr1.filename_vtk.push_back(ss.str());
                }
                fr1.find_this = FMFieldDataName;
                fr1.next_field = "\n";
                fr1.parseData();
	// Read results from a single composite source simulation
        FileReader fr2;
                fr2.packets = packets;
                fr2.filename_vtk.push_back("./../Examples/Data/colin27/FullMonte_energy_composite_0.vtk");
                fr2.find_this = FMFieldDataName;
                fr2.next_field = "\n";
                fr2.parseData();

	// Calculate statistical results
        Chi2Test chi2test;
                chi2test.topNvalues = topNvalues;
                chi2test.threshold = threshold;
                chi2test.expectedData = fr2.getVTKResults();
                chi2test.observedData = fr1.getVTKResults();
                chi2test.compute();

        BOOST_CHECK_EQUAL(chi2test.getPvalue(), 1.0);
        BOOST_CHECK_LT(chi2test.getChi2Stat(), 0.01);
        BOOST_CHECK_LT(chi2test.getAbsError()/chi2test.getExpectedPartial(), 0.05);
}

BOOST_AUTO_TEST_SUITE_END()
