/**
 * TetraMeshBaseVTK.hpp
 *
 *  Created on: Mar 7, 2015
 *      Author: jcassidy
 */

#ifndef VTK_TETRAMESHVTK_HPP_
#define VTK_TETRAMESHVTK_HPP_

#include <vtkObject.h>

#include "vtkHelperFunctions.hpp"


/** Non-owning wrapper for a FullMonte TetraMesh object.
 */

class vtkFullMonteTetraMeshWrapper : public vtkObject
{
public:
	vtkTypeMacro(vtkFullMonteTetraMeshWrapper,vtkObject);
	static vtkFullMonteTetraMeshWrapper* New();
	virtual ~vtkFullMonteTetraMeshWrapper();

	/// Get/set the underlying mesh
	void 					mesh(const TetraMesh* m);
	void					mesh(const char* mptr);
	const TetraMesh* 		mesh() 							const;

	/// Update the VTK arrays from the mesh
	void update();

	vtkUnstructuredGrid* blankMesh() const;
	vtkUnstructuredGrid* regionMesh() const;

	vtkPoints*				points() const;
	vtkUnsignedShortArray* 	regions() const;

	vtkPolyData*			faces() const;

protected:
	vtkFullMonteTetraMeshWrapper();

private:
	const TetraMesh* 		m_mesh=nullptr;

	vtkCellArray*			m_faces=nullptr;
	vtkCellArray*			tetras() const;

	vtkPoints*				m_points=nullptr;
	vtkUnsignedShortArray*	m_regions=nullptr;
	vtkCellArray*			m_tetras=nullptr;
};


#endif /* VTK_TETRAMESHVTK_HPP_ */
