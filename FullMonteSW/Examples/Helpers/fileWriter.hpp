/*
 * fileWriter.hpp
 *
 *  Created on: Aug 21, 2017
 *      Author: Siyun Li
 */

#ifndef FILEWRITER_HPP_
#define FILEWRITER_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridWriter.h"
#include "vtkFloatArray.h"
#include "vtkCellData.h"
#include "vtkSmartPointer.h"

class FileWriter
{
public:
	FileWriter();
	virtual ~FileWriter();

	// Name of the VTK file to which the new data fields are appended
	std::string filein_vtk;
	// Name of the output VTK file that holds all the information from "filein_vtk" and the new fields
	std::string fileout_vtk;
	// Name of the text file in which new data fields are written in columns
	std::string fileout_txt;

	// List of data to be written
	std::list<std::vector<float>> data_lists;
	// Names of the data fields to be used in the VTK file. Please use the same order as "data_lists"
	std::list<std::string> arrayName;

	// Output text file
	void writeTxtSummary();
	// Output VTK file
	void writeVTKSummary();

private:
	// Create a new VTK float array to hold the dataset and append
	vtkSmartPointer<vtkFloatArray> createNewFloatArray(std::string name, std::vector<float>& data);

};

#endif /* FILEWRITER_HPP_ */
