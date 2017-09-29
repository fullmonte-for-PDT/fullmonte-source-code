/*
 * Test_FullMonte_approx.cpp
 *
 *  Created on: August 3, 2017
 *      Author: Siyun Li
 */

#define BOOST_TEST_MODULE "Test_FullMonte_Approx"

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <FullMonteSW/Examples/Helpers/fileReader.hpp>
#include <FullMonteSW/Examples/Helpers/chi2test.hpp>


BOOST_AUTO_TEST_SUITE(Test_FullMonte_Approx)

BOOST_AUTO_TEST_CASE(FullMonte_approx_ball_colin27)
{
	unsigned packets = 1e6;
	unsigned topNvalues = 1e5;
	float threshold = 1e-8;
	std::string FMFieldDataName = "Volume%20" "energy";
	
	FileReader fr1;
		fr1.packets = packets;
		for (unsigned i = 1; i < 29; ++i)
		{
			std::stringstream ss;
			ss << "./../Examples/FullMonte_approx/ExpectedData/colin27/FullMonte_energy_ball_point_" << i << ".vtk";
			fr1.filename_vtk.push_back(ss.str());
		}
		fr1.find_this = FMFieldDataName;
		fr1.next_field = "\n";
		fr1.parseData();

	// Add data at ball centre
	FileReader fr;
	fr.filename_vtk.push_back("./../Examples/FullMonte_approx/ExpectedData/colin27/FullMonte_energy_ball_point_0.vtk");
	fr.packets = packets/3.0;
	fr.find_this = FMFieldDataName;
	fr.next_field = "\n";
	fr.parseData();

	// Sum up all data from the approximated point sources
	std::vector<float> approx;
	for (unsigned k = 0; k < fr1.getVTKResults().size(); ++k)
		approx.push_back(fr1.getVTKResults()[k]/3.0 + fr.getVTKResults()[k]*2.0/3.0);

	FileReader fr2;
		fr2.packets = packets;
		fr2.filename_vtk.push_back("./../Examples/Data/colin27/FullMonte_energy_ball_0.vtk");
		fr2.find_this = FMFieldDataName;
		fr2.next_field = "\n";
		fr2.parseData();
		fr2.filename_vtk.push_back("./../Examples/Data/colin27/FullMonte_energy_ball_0.vtk");

	Chi2Test chi2test;
		chi2test.topNvalues = topNvalues;
		chi2test.threshold = threshold;
		chi2test.expectedData = approx;
		chi2test.observedData = fr2.getVTKResults();
		chi2test.compute();

	BOOST_CHECK_EQUAL(chi2test.getPvalue(), 1.0);
	BOOST_CHECK_LT(chi2test.getChi2Stat(), 0.15);
	BOOST_CHECK_LT(chi2test.getAbsError()/chi2test.getExpectedPartial(), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
