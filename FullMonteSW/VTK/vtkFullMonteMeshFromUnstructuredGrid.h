/*
 * vtkFullMonteMeshFromUnstructuredGrid.h
 *
 *  Created on: May 24, 2017
 *      Author: jcassidy
 */

#ifndef VTK_VTKFULLMONTEMESHFROMUNSTRUCTUREDGRID_H_
#define VTK_VTKFULLMONTEMESHFROMUNSTRUCTUREDGRID_H_

#include <vtkObject.h>
#include <string>

class TetraMesh;
class vtkUnstructuredGrid;
class PTTetraMeshBuilder;

class vtkFullMonteMeshFromUnstructuredGrid : public vtkObject
{
public:
	vtkTypeMacro(vtkFullMonteMeshFromUnstructuredGrid,vtkObject);
	static vtkFullMonteMeshFromUnstructuredGrid* New();

	void unstructuredGrid(vtkUnstructuredGrid* ug);
	void regionLabelFieldName(const char* name);
	void regionLabelFieldName(std::string name);

	TetraMesh* mesh();

	const char* meshPtr() const;

	void update();

protected:
	vtkFullMonteMeshFromUnstructuredGrid();
	~vtkFullMonteMeshFromUnstructuredGrid();

private:
	vtkUnstructuredGrid*	m_ug=nullptr;

	std::string				m_regionLabelFieldName="Tissue Type";

	PTTetraMeshBuilder*		m_builder=nullptr;
};



#endif /* VTK_VTKFULLMONTEMESHFROMUNSTRUCTUREDGRID_H_ */
