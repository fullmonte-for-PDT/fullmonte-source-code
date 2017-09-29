/*
 * FullMonte_visualize.cpp
 *
 *  Created on: Aug 13, 2017
 *      Author: Siyun Li
 */

#include "../Helpers/visualize.hpp"
#include <sstream>

int main(int argc, char* argv[])
{
	const char* inFileName = 0;
	std::ifstream inFile;
	if (argc != 2)
	{
		fprintf(stderr, "Wrong number of arguments\n");
		return 1;
	}

	inFileName = argv[1];
	inFile.open(inFileName);
	if (!inFile)
	{
		fprintf(stderr, "Cannot open input file: \"%s\"\n", inFileName);
		return 1;
	}


        std::string dataType;
        std::string sourceType;
	std::string meshName;
	std::string line;
	getline(inFile, line);
	dataType = line.substr(0, line.find(" "));

	getline(inFile, line);
	sourceType = line.substr(0, line.find(" "));

	getline(inFile, line);
	meshName = line.substr(0, line.find("\t"));

        std::string FMFieldDataName = "Volume " + dataType + " (packet)";

	std::stringstream ss1;

	// Define input file directories
	ss1 <<"./../Data/" << meshName << "/FullMonte_" << dataType << "_" << sourceType << "_0.vtk";	

	// Visualization //
	Visualize visualMMC;
		visualMMC.setViewPortGrid(1, 1);
		visualMMC.logScale = true;
		visualMMC.fileName = ss1.str();
		visualMMC.scalarName[0] = FMFieldDataName;
		visualMMC.pngName = "FullMonte_visualize.png";
		visualMMC.visualization();
	
	return 0;
}
