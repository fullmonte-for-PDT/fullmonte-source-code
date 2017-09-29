/* 
 * visualize.hpp
 *
 *  Created on: Jun 20, 2017
 *      Author: Siyun Li
 */

#ifndef VISUALIZE_HPP_
#define VISUALIZE_HPP_

#include <string>
#include <cmath>
// modify vtk files
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkFloatArray.h"
#include "vtkSmartPointer.h"
// render
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellData.h"
#include "vtkClipDataSet.h"
#include "vtkColorTransferFunction.h"
#include "vtkDiscretizableColorTransferFunction.h"
#include "vtkDataSetMapper.h"
#include "vtkPlane.h"
#include "vtkPNGWriter.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkScalarBarActor.h"
#include "vtkSmartPointer.h"
#include "vtkWindowToImageFilter.h"

class Visualize
{
public:
	Visualize();
	virtual ~Visualize();

	bool logScale;
	// name of the data field(s)
	std::string *scalarName;
	// name of the .vtk file that contains the data field(s)
	std::string fileName;
	// name of the saved image
	std::string pngName;

	// set the number of views in a window
	void setViewPortGrid (unsigned row, unsigned column);
	// start visualization
	void visualization();

private:
	double range[2];
	unsigned row;
	unsigned column;
	
	// read the vtkUnstructuredGrid from the input file
	vtkSmartPointer<vtkUnstructuredGrid> readUnstructuredGridFromFile();

	// get the range of the data field to be mapped to colour
	void getScalarRange(std::string name, vtkSmartPointer<vtkUnstructuredGrid> ug);

	// create a red-white-blue colour transfer function as the default in ParaView
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> setColorTransferFunctionToBlueWhiteRed();

	// create a renderer in a view port
	vtkSmartPointer<vtkRenderer> createRenderInViewPort(unsigned portNum, std::string name, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkDiscretizableColorTransferFunction> ctf, vtkSmartPointer<vtkCamera> camera);
};
#endif /*_VISUALIZE_HPP_ */

