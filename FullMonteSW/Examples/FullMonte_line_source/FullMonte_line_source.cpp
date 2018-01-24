/*
 * FullMonte_line_source.cpp
 *
 *  Created on: October 26, 2017
 *      Author: Abed Yassine
 */

#include "../Helpers/fileReader.hpp"
#include "../Helpers/fileWriter.hpp"
#include "../Helpers/visualize.hpp"
#include "../Helpers/chi2test.hpp"


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
	float data[4];
	for (unsigned i = 0; i < 7; i++)
	{
		getline(inFile, line);
		if (i < 4)
		{
			std::stringstream subline(line.substr(0, line.find("\t")));
			subline >> data[i];
		}
		else if (i == 4)
			dataType = line.substr(0, line.find(" "));
		else if (i == 5)
			sourceType = line.substr(0, line.find(" "));
		else if (i == 6)
			meshName = line.substr(0, line.find("\t"));
	}

	// Number of experiments run and packets launched
	unsigned N = data[0];
	unsigned packets = data[1];
	// chi2test parameters
	unsigned topNvalues = data[2];
	float threshold = data[3];

        std::string FMFieldDataName = dataType + "0";

	// chi2test summary file
	std::stringstream schi;
	schi << "./Result/" << meshName << "/chi2test_results.txt";
	std::ofstream fchi(schi.str().c_str());
	fchi << std::fixed << std::setw(10) << "Trial\tStatistic\tp value\tabsolute error\tObserved part\tExpected part\tObserved total\tExpected total" << std::endl;

	for (unsigned i = 0; i < N; i++)
	{
		std::stringstream ss1, ss2, sso, svtk;

		// Define input file directories
		ss1 << "./../Data/" << meshName << "/FullMonte_" << dataType << "_" << sourceType << "_" << i << ".vtk";
		ss2 << "./ExpectedData/" << meshName << "/FullMonte_" << dataType << "_" << sourceType << ".vtk";
		
		// Set up output file
		sso << "./Result/" << meshName << "/" << sourceType << "_" << dataType << "_data_summary_" << i << ".txt";
		svtk << "./Result/" << meshName << "/" << sourceType << "_" << dataType << "_data_summary_" << i << ".vtk";
		
		// Read data from vtk files
		FileReader fr1;
			fr1.packets = packets;
			fr1.filename_vtk.push_back(ss1.str());
			fr1.find_this = FMFieldDataName;
			fr1.next_field = "\n";
			fr1.parseData();
		FileReader fr2;
			fr2.packets = packets;
			fr2.filename_vtk.push_back(ss2.str());
			fr2.find_this = FMFieldDataName;
			fr2.next_field = "\n";
			fr2.parseData();
		
		// Write datasets in vtk or text format
		FileWriter fw;
			fw.filein_vtk = ss1.str();
			fw.fileout_vtk = svtk.str();
			fw.fileout_txt = sso.str();
			fw.data_lists.push_back(fr1.getVTKResults());
			fw.arrayName.push_back("FullMonteObserved");
			fw.data_lists.push_back(fr2.getVTKResults());
			fw.arrayName.push_back("FullMonteExpected");
			fw.writeVTKSummary();
			fw.writeTxtSummary();

		// Perform chi_squared test on output data
		Chi2Test chi2test;
			chi2test.topNvalues = topNvalues;
			chi2test.threshold = threshold;
			chi2test.expectedData = fr2.getVTKResults();
			chi2test.observedData = fr1.getVTKResults();
       			chi2test.compute();
                        fchi << i << "\t" << chi2test.getResultSummary();


		// Run MatLab executable (energy comparison plot auto-saved)
		std::stringstream commandline;
		commandline << "./../Helpers/run_comparePlot.sh /usr/local/MATLAB/MATLAB_Runtime/v92 " << sso.str() << " " << i;
		int plotEnergy = system(commandline.str().c_str());

		Visualize visualMMC;
			visualMMC.setViewPortGrid(1, 2);
			visualMMC.logScale = true;
			visualMMC.fileName = svtk.str();
			visualMMC.scalarName[0] = "FullMonteObserved";
			visualMMC.scalarName[1] = "FullMonteExpected";
			visualMMC.pngName = "./Results/FullMonte_MMC_point.png";
			visualMMC.visualization();
	}
	
	fchi.close();
	return 0;
}
