/*
 * vtkHelperFunctions.hpp
 *
 *  Created on: Jun 6, 2017
 *      Author: jcassidy
 */

#ifndef VTK_VTKHELPERFUNCTIONS_HPP_
#define VTK_VTKHELPERFUNCTIONS_HPP_

class vtkPoints;
class vtkCellArray;
class vtkUnstructuredGrid;
class vtkUnsignedShortArray;
class vtkPolyData;

class TetraMesh;

void getVTKPoints(const TetraMesh& M,vtkPoints* P);
void getVTKTetraCells(const TetraMesh& M,vtkCellArray* ca);
void getVTKTetraRegions(const TetraMesh& M,vtkUnsignedShortArray* R);
void getVTKTriangleCells(const TetraMesh& M,vtkCellArray* ca);
void getVTKDirectedTriangleCells(const TetraMesh& M,vtkCellArray* ca);

#endif /* VTK_VTKHELPERFUNCTIONS_HPP_ */
