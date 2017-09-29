/*
 * LinearTetraEnclosingPointQuery.hpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_QUERIES_TETRAENCLOSINGPOINTBYLINEARSEARCH_HPP_
#define GEOMETRY_QUERIES_TETRAENCLOSINGPOINTBYLINEARSEARCH_HPP_

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/Point.hpp>

/** O(Nt) location of tetra enclosing a given point by traversing all tetras and testing if it encloses the point
 *
 *
 * Fairly foolproof, as it makes no assumptions about the mesh aside from being well-formed.
 */



class TetraEnclosingPointByLinearSearch
{
public:
	TetraEnclosingPointByLinearSearch();
	~TetraEnclosingPointByLinearSearch();

	void 						mesh(const TetraMesh* m);
	void 						point(Point<3,double> p);

	void 						update();

	TetraMesh::TetraDescriptor	tetra() const;


private:
	const TetraMesh*			m_mesh=nullptr;
	Point<3,double>				m_point=Point<3,double>::nan();
	double						m_heightEpsilon=1e-5;

	bool						m_verbose=true;

	std::vector<TetraMesh::TetraDescriptor>		m_inside;
	std::vector<TetraMesh::TetraDescriptor>		m_maybe;

};

#endif /* GEOMETRY_QUERIES_TETRAENCLOSINGPOINTBYLINEARSEARCH_HPP_ */
