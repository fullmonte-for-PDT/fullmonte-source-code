/*
 * vtkFullMonteMeshFacesToPolyData.h
 *
 *  Created on: Jun 6, 2017
 *      Author: jcassidy
 */

#ifndef VTK_VTKFULLMONTEMESHFACESTOPOLYDATA_H_
#define VTK_VTKFULLMONTEMESHFACESTOPOLYDATA_H_

#include <vtkObject.h>

class vtkPolyData;
class TetraMesh;
class Partition;

class vtkFullMonteMeshFacesToPolyData : public vtkObject
{
public:
	vtkFullMonteMeshFacesToPolyData();
	~vtkFullMonteMeshFacesToPolyData();

	static vtkFullMonteMeshFacesToPolyData* New();

	void				mesh(TetraMesh* );
	//void				partition(Partition*);

	//void				showZeroElements(bool s);

	void				update();
	vtkPolyData*		output() const;

	void				outputDirectedFaces(bool st);		///< If true, output directed faces (2x as many as undirected)


private:
	TetraMesh*			m_mesh=nullptr;
	//Partition*			m_partition=nullptr;
	//bool				m_showZeroElements=false;

	bool				m_outputDirectedFaces=false;

	vtkPolyData*		m_vtkPD=nullptr;
};


#endif /* VTK_VTKFULLMONTEMESHFACESTOPOLYDATA_H_ */
