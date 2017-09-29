/*
 * Test_TclShell.cpp
 *
 *  Created on: July 20, 2017
 *      Author: Siyun Li
 */

//#include "cpptcl.h"
#include <boost/test/unit_test.hpp>
#include <string>
using namespace std;


void func (int i)
{
	if (i != 0)
		throw std::runtime_error("Error detected");
}

BOOST_AUTO_TEST_CASE(tclShell)
{
	string commandline = "./tclmonte.sh ../Examples/tclShell.tcl";
	BOOST_REQUIRE_NO_THROW(func(system(commandline.c_str())));
}
