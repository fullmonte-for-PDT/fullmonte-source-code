/*
 * TetrasNearPoint.hpp
 *
 *  Created on: May 24, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_QUERIES_TETRASNEARPOINT_HPP_
#define GEOMETRY_QUERIES_TETRASNEARPOINT_HPP_

#include <FullMonteSW/Geometry/Point.hpp>

class TetraMesh;

/** Iterates through all tetras that are within a given radius.
 *
 * Simply iterates through all tetras, pausing where a tetra meets the criteria.
 *
 * Iteration is controlled by start()/next()/done().
 *
 */

class TetrasNearPoint
{
public:
	TetrasNearPoint();
	~TetrasNearPoint();

	void mesh(const TetraMesh* );

	void radius(float r);
	void centre(Point<3,double> P);

	unsigned containingTetra() const;

	void start();
	void next();
	bool done() const;

	unsigned currentTetraID() const;

private:
	const TetraMesh* 	m_mesh=nullptr;

	Point<3,double>		m_centre;

	float 				m_radius2=0.0f;
	std::vector<float>	m_squaredDistances;

	bool 				m_requireAllWithin=false;

	/// Iteration counter
	unsigned 			m_tetraID=1;
};



#endif /* GEOMETRY_QUERIES_TETRASNEARPOINT_HPP_ */

