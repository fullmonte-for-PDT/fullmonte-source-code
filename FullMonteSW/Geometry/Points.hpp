/*
 * Points.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_POINTS_HPP_
#define GEOMETRY_POINTS_HPP_

#include <vector>
#include <array>

#include "Point.hpp"

class Points
{
public:
	Points();
	~Points();

	void resize(unsigned N);

	unsigned size() const;

	Point<3,double> get(unsigned i);
	void 			set(unsigned i,Point<3,double> p);

private:
	std::vector<Point<3,double>>	m_points;
};


#endif /* GEOMETRY_POINTS_HPP_ */
