/*
 * find_pos.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: Siyun Li
 */

#include "spherePos.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

int main()
{
	std::ifstream inFile;
	inFile.open("find_pos.in");
	
	std::string line;
	getline(inFile, line);
	std::stringstream subline;
	subline.str(line);
	unsigned rad;
	subline >> rad;

	std::array<float,3> p;
	for (unsigned i = 0; i < 3; ++i)
	{
		getline(inFile, line);
		subline.str(line);
		subline >> p[i];
	}

	SpherePos sp;
		sp.position(p);
		sp.radius(rad);
		sp.writePoints();
	return 0;
}
