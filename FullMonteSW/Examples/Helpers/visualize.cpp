/*
 * visualize.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: siyunli
 */

#include "visualize.hpp"


Visualize::Visualize()
{
}

Visualize::~Visualize()
{
	delete[] scalarName;
}

// create a grid with given rows and columns for view port(s)
void Visualize::setViewPortGrid (unsigned row, unsigned column)
{
	this->row = row;
	this->column = column;
	scalarName = new std::string[row*column];
}

void Visualize::visualization()
{
	// Create an unstructured grid from the input file
	vtkSmartPointer<vtkUnstructuredGrid> ug;
		ug = readUnstructuredGridFromFile();
	
	// Find the range of the scalar to visualize
	getScalarRange(scalarName[0], ug);

	// Create a render window and an interactor
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
		renderWindow->SetSize(1000, 500);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		renderWindowInteractor->SetRenderWindow(renderWindow);
	// Create a camera for the selected view angle
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
		camera->SetViewUp(0, 0, 1);
		camera->SetPosition(-400, 100, 100);
		camera->SetFocalPoint(70, 100, 100);

	// Set up each renderer
	for (unsigned i = 0; i < row * column; ++i)
		renderWindow->AddRenderer(createRenderInViewPort(i, scalarName[i], ug, setColorTransferFunctionToBlueWhiteRed(), camera));
	
	renderWindow->Render();
	
	// Create a snapshot of the render window
	vtkSmartPointer<vtkWindowToImageFilter> wif = vtkSmartPointer<vtkWindowToImageFilter>::New();
		wif->SetInput(renderWindow);
		wif->ReadFrontBufferOff();
		wif->Update();

	// Save a .png file of the snapshot
	vtkSmartPointer<vtkPNGWriter> png = vtkSmartPointer<vtkPNGWriter>::New();
		png->SetFileName(pngName.c_str());
		png->SetInputConnection(wif->GetOutputPort());
		png->Write();

	// Start the render window
	renderWindow->Render();
	renderWindowInteractor->Start();
}

// Read the vtk unstructured grid from the given VTK file
vtkSmartPointer<vtkUnstructuredGrid> Visualize::readUnstructuredGridFromFile()
{
	vtkSmartPointer<vtkUnstructuredGridReader> reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
		reader->SetFileName(fileName.c_str());
		reader->Update();
	return reader->GetOutput();
}

// Calculate the range of the scalar of interest
void Visualize::getScalarRange(std::string name, vtkSmartPointer<vtkUnstructuredGrid> ug)
{
	vtkSmartPointer<vtkFloatArray> scalar;
		scalar = scalar->SafeDownCast((ug->GetCellData())->GetScalars(name.c_str()));
	scalar->GetRange(range);

	// Adjust scalar range for logarithmic scale
	if (logScale == true && range[0] == 0)
		range[0] = range[1]/1e4;
}

// Create a standard colour transfer function like the default one in ParaView
vtkSmartPointer<vtkDiscretizableColorTransferFunction> Visualize::setColorTransferFunctionToBlueWhiteRed()
{
	vtkSmartPointer<vtkDiscretizableColorTransferFunction> ctf = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
	double midpoint;

	// find the midpoint of the scale (value to be mapped to colour white)
	if (logScale)
	{
		ctf->SetUseLogScale(1);
		midpoint = range[1]/sqrt(range[1]/range[0]);
	}
	else
    		midpoint = 1/2 * (range[0]+range[1]);
	
	// create the colour transfer function
	ctf->SetColorSpaceToRGB();
	ctf->AddRGBPoint(range[0], 0.231373, 0.298039, 0.752941);			// blue
	ctf->AddRGBPoint(midpoint, 0.865003, 0.865003, 0.865003);			// white
   	ctf->AddRGBPoint(range[1], 0.705882, 0.0156863, 0.14902);			// red
	ctf->SetColorSpaceToDiverging();
   	ctf->AdjustRange(range);
   	ctf->Build();

   	return ctf;
}


vtkSmartPointer<vtkRenderer> Visualize::createRenderInViewPort(unsigned portNum, std::string name, vtkSmartPointer<vtkUnstructuredGrid> ug, vtkSmartPointer<vtkDiscretizableColorTransferFunction> ctf, vtkSmartPointer<vtkCamera> camera)
{
	// Create a renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
		renderer->SetBackground(0.32, 0.34, 0.43);
		renderer->SetViewport((portNum/row)/(double)column, (portNum%row)/(double)row, (portNum/row+1)/(double)column, (portNum%row+1)/(double)row);

	// Create a copy of ug
	vtkSmartPointer<vtkUnstructuredGrid> ug2 = vtkSmartPointer<vtkUnstructuredGrid>::New();
		ug2->ShallowCopy(ug);
		ug2->GetCellData()->SetActiveScalars(name.c_str());

	// Create a mapper to map scalar to colours
	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
		mapper->SetInputData(ug2);	
		mapper->SetLookupTable(ctf);
		mapper->SetScalarRange(range[0], range[1]);
		mapper->SetScalarModeToUseCellData();
		mapper->SetColorModeToMapScalars();
		mapper->ScalarVisibilityOn();
		mapper->InterpolateScalarsBeforeMappingOn();

	// Create an actor for the mapper
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

	// Create a scalar bar
   	vtkSmartPointer<vtkScalarBarActor> legend = vtkSmartPointer<vtkScalarBarActor>::New();
   		legend->SetLookupTable(ctf);
   		legend->SetTitle(name.c_str());
   		legend->SetOrientationToVertical();
   		legend->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
   		legend->GetPositionCoordinate()->SetValue(0.9, 0.1);
   		legend->SetWidth(0.1);
   		legend->SetHeight(0.6);

	// Add the scalar bar, mapper, and camera to the scene
		renderer->AddActor(actor);
	   	renderer->AddActor2D(legend);
	   	renderer->SetActiveCamera(camera);

	// Create a clip of the rendered image
	vtkSmartPointer<vtkPlane> clip = vtkSmartPointer<vtkPlane>::New();
		clip->SetNormal(1.0, 0.0, 0.0);
		clip->SetOrigin(90.7709078434855, 110.366635084152, 86.9421596825123);

	vtkSmartPointer<vtkClipDataSet> clipper = vtkSmartPointer<vtkClipDataSet>::New();
		clipper->SetInputData(ug2);
		clipper->SetClipFunction(clip);
		mapper->SetInputConnection(clipper->GetOutputPort());
		mapper->Update();

	return renderer;
}
