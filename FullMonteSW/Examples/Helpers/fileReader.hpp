/*
 * fileReader.hpp
 *
 *  Created on: Aug 21, 2017
 *      Author: Siyun Li
 */

#ifndef FILEREADER_HPP_
#define FILEREADER_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

class FileReader
{
public:
	FileReader();
	virtual ~FileReader();
	
	unsigned packets;
	std::list<std::string> filename_vtk;
	std::list<std::string> filename_txt;
	// Name of the data field to be read
	std::string find_this;
	// Name of the data field following "find_this", used to tell when to stop reading input file
	std::string next_field;
	// Column of where the data is located. Index starts from 0.
	unsigned data_index;

	std::vector<float> getTxtResults() {	return this->text_list;	}
	std::vector<float> getVTKResults() {	return this->vtk_list;	}

	// Tell the FileReader to start reading the file and store the data in vectors.
	void parseData();

private:
	std::vector<float> text_list;
	std::vector<float> vtk_list;

	std::vector<float> readVTKData(std::string inputFile);

	std::vector<float> readTxtData(std::string inputFile);
	// Calculate the sum of all the input vectors, and return the sum.
	std::vector<float> getSum(std::list<std::vector<float>>& temp);
};

#endif /* FILEREADER_HPP_ */

