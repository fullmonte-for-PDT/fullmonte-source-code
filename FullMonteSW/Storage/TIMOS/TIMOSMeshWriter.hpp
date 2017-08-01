/*
 * TIMOSWriter.hpp
 *
 *  Created on: Mar 7, 2015
 *      Author: jcassidy
 */

#ifndef TIMOSWRITER_HPP_
#define TIMOSWRITER_HPP_


#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Sources/ForwardDecl.hpp>

#include <string>
#include <vector>

#include <iostream>

/** Writes a mesh out in TIMOS format.
 *
 * Indices are one-based, so the first tetra and point are dropped when writing
 *
 * Format:
 *
 * Np
 * Nt
 * { px py pz }Np
 * { P0 P1 P2 P3 region}Nt
 *
 *
 * Np			Number of points
 * Nt			Number of tetras
 * px..pz		Point coordinates
 * P0..P4		Point references (first point in vector is 1)
 * region		Region code 1..Nr
 */

class TIMOSMeshWriter
{
public:
	TIMOSMeshWriter();
	~TIMOSMeshWriter();

	void filename(std::string fn);
	void mesh(const TetraMesh* m);

	void write() const;

private:
	std::string			m_filename;
	const TetraMesh*	m_mesh=nullptr;

	unsigned			m_coordinatePrecision=6;
	unsigned			m_coordinateWidth=10;

	unsigned			m_regionWidth=2;

	unsigned			m_indexWidth=6;
};

#endif /* TIMOSWRITER_HPP_ */

