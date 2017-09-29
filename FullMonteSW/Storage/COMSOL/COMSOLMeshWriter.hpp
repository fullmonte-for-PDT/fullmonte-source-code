/*
 * COMSOLMeshWriter.hpp
 *
 *  Created on: July 26, 2017
 *      Author: Siyun Li
 */

#ifndef COMSOLMESHWRITER_HPP_
#define COMSOLMESHWRITER_HPP_


#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Sources/ForwardDecl.hpp>

#include <string>
#include <vector>

#include <iostream>

/** Writes a mesh out in COMSOL format.
 *
 * Indices are zero-based, so the first tetra and point need to be dropped.
 * and all other tetra and points need to decrement by 1
 *
 * Format:
 *
 * #####
 * # ------- Object 0 ---------
 * #####
 * Np
 * #####
 * #Mesh point coordinates
 * { px py pz }
 * #####
 * # Type #0
 * #####
 * Nt
 * Elements
 * { P0 P1 P2 P3 }
 *
 *
 * Np			Number of points
 * Nt			Number of tetras
 * px..pz		Point coordinates
 * P0..P3		Point references (first point in vector is 1)
 */

class COMSOLMeshWriter
{
public:
	COMSOLMeshWriter();
	~COMSOLMeshWriter();

	void filename(std::string fn);
	void mesh(const TetraMesh* m);

	void write() const;
	void setElementTypeCount(unsigned types);
	void setElementDetails(unsigned count, std::string name, unsigned nodeCount);

private:
	std::string			m_filename;
	const TetraMesh*	m_mesh=nullptr;

	unsigned			m_coordinatePrecision=6;
	unsigned			m_coordinateWidth=10;

	unsigned			m_regionWidth=2;

	unsigned			m_indexWidth=6;

	unsigned 			elementType;
	string*				typeName;
	unsigned*			node;
	unsigned*			element;
};

#endif /* COMSOLMESHWRITER_HPP_ */

