/*
 * fileReader.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: Siyun Li
 */

#include "fileReader.hpp"

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}
// Read a VTK data field and return the results in a vector
std::vector<float> FileReader::readVTKData(std::string inputFile)
{
	std::ifstream fin;
	fin.open(inputFile.c_str());
	std::vector<float> dataList;
	bool found_data = false;
	std::string line;
	while (getline(fin, line))
	{
		if (line.substr(0, next_field.size())==next_field)
			found_data = false;
		if (found_data)
		{
			std::stringstream str(line);
			float data;
			while (str >> data)
				dataList.push_back(data/packets);
		}
		if (line.substr(0, find_this.size())==find_this)
			found_data = true;
	}
	fin.close();
	return dataList;
}

// Read a column of a text file and return the results in a vector
std::vector<float> FileReader::readTxtData(std::string inputFile)
{
	std::ifstream fin;
	fin.open(inputFile.c_str());
	std::vector<float> dataList;
	std::string line;
	dataList.push_back(0);
	while (getline(fin, line))
	{
		std::stringstream str(line);
		float data;
		unsigned counter = 0;
		while (str >> data)
		{
			if (counter == data_index)
			{
				dataList.push_back(data);
				break;
			}
			++counter;
		}
	}
	fin.close();
	return dataList;
}

// Calculate the sum of multiple fields in different input files if necessary. 
//Otherwise, return the read dataset in a vector
void FileReader::parseData()
{
	if (filename_vtk.size() == 1)
		vtk_list = readVTKData(filename_vtk.front());
	else if (filename_vtk.size() > 1)
	{
		std::list<std::vector<float>> temp;
		for (auto &f : filename_vtk)
			temp.push_back(readVTKData(f));
		vtk_list = getSum(temp);
	}
	if (filename_txt.size() == 1)
		text_list = readTxtData(filename_txt.front());
	else if (filename_txt.size() > 1)
	{
		std::list<std::vector<float>> temp;
		for (auto &f : filename_txt)
			temp.push_back(readTxtData(f));
		text_list = getSum(temp);
	}
}

// Calculate the sum of the input data vectors. Normalized by dividing the total number
// of vectors summed.
std::vector<float> FileReader::getSum(std::list<std::vector<float>>& temp)
{
	std::vector<float> dataList;
	for (unsigned i = 0; i < temp.front().size(); ++i)
	{
		float sum = 0;
		for (auto &v : temp)
			sum += v[i];
		dataList.push_back(sum/(float)temp.size());
	}
	return dataList;
}


