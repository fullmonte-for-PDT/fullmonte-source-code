/*
 * Test_Layered.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: jcassidy
 */

#include <boost/test/unit_test.hpp>

#include <FullMonteSW/Geometry/Layered.hpp>

// check that (ri,zi) maps to i and vice-versa
#define BOOST_CHECK_CYL_MAPPING(ri,zi,i) BOOST_CHECK_EQUAL( L.binFromIndex(i), (CylIndex { ri, zi }) ); \
	BOOST_CHECK_EQUAL( L.indexForBin(CylIndex { ri,zi }), i );

BOOST_AUTO_TEST_CASE(Layered_bins)
{
	const unsigned Nr=101U;
	const unsigned Nz=41U;
	Layered L;
	L.dims(Nr, Nz);

	BOOST_CHECK_CYL_MAPPING(0U,0U,0U);
	BOOST_CHECK_CYL_MAPPING(1U,1U,1U);
	BOOST_CHECK_CYL_MAPPING(Nr,1U,Nr);
	BOOST_CHECK_CYL_MAPPING(1U,2U,Nr+1);
	BOOST_CHECK_CYL_MAPPING(2U,2U,Nr+2);
	BOOST_CHECK_CYL_MAPPING(Nr,Nz,Nr*Nz);
}
