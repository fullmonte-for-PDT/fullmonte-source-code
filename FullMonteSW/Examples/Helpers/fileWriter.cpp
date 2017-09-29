/*
 * fileWriter.cpp
 *
 *  Created on: Aug 21, 2017
 *      Author: Siyun Li
 */

#include "fileWriter.hpp"

FileWriter::FileWriter()
{
}

FileWriter::~FileWriter()
{
}

// Write a list of data in a column in the text file
void FileWriter::writeTxtSummary()
{
	std::ofstream fout(fileout_txt.c_str());
	for (unsigned i = 0; i < data_lists.front().size(); ++i)
	{
		fout << i << "\t";
		for (auto &v : data_lists)
			fout << v[i] << "\t";
		fout << std::endl;
	}
	fout.close();
}

// Write a list of data in VTK legacy format
void FileWriter::writeVTKSummary()
{
	// Read the existing information in the input file
	vtkSmartPointer<vtkUnstructuredGridReader> reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
		reader->SetFileName(filein_vtk.c_str());
		reader->ReadAllFieldsOn();
		reader->ReadAllScalarsOn();
		reader->ReadAllVectorsOn();
		reader->Update();

	// Add the new data fields
	vtkSmartPointer<vtkUnstructuredGrid> ug = reader->GetOutput();
	std::list<std::string>::iterator it1;
	std::list<std::vector<float>>::iterator it2;
		for (it1 = arrayName.begin(), it2 = data_lists.begin(); it1 != arrayName.end() && it2 != data_lists.end(); ++it1, ++it2)
		{
			vtkSmartPointer<vtkFloatArray> temp = createNewFloatArray(*it1, *it2);
			ug->GetCellData()->AddArray(temp);
		}

	// Write all the datasets into the output file
	vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
		writer->SetFileName(fileout_vtk.c_str());
		writer->SetInputData(ug);
		writer->Update();
}

// Create a VTK float array to be appended
vtkSmartPointer<vtkFloatArray> FileWriter::createNewFloatArray(std::string name, std::vector<float>& data)
{
	vtkSmartPointer<vtkFloatArray> temp = vtkSmartPointer<vtkFloatArray>::New();
		temp->SetName(name.c_str());
		temp->SetNumberOfTuples(data.size());
		temp->SetTuple1(0, 0);
		for (unsigned i = 0; i < data.size(); ++i)
			temp->SetTuple1(i, data[i]);
	return temp;
}
