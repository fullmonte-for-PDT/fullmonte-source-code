/*
 * spherePos.hpp
 *
 *  Created on: Aug 22, 2017
 *      Author: Siyun Li
 */

#ifndef SPHEREPOS_HPP_
#define SPHEREPOS_HPP_

#include <array>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <iomanip>

class SpherePos
{
public:
	SpherePos();
	virtual ~SpherePos();

	void position(std::array<float,3> p) {	centre = p;	}
	std::array<float,3> position() {	return centre;	}

	void radius(float r) {	rad = r;	}
	float radius() {	return rad;	}

	void writePoints();

private:
	std::array<float,3> centre;
	float rad;
	std::array<std::array<float,3>,14> unitDist = {{
							{{0, 0, 1}}, {{0, 0, -1}}, {{0, 1, 0}}, {{0, -1, 0}}, {{1, 0, 0}}, {{-1, 0, 0}},
							{{0.5, 0.5, 0.7071067812}}, {{0.5, 0.5, 0.7071067812}}, 
							{{-0.5, 0.5, 0.7071067812}}, {{-0.5, -0.5, 0.7071067812}},
							{{0.5, 0.5, -0.7071067812}}, {{0.5, 0.5, -0.7071067812}}, 
							{{-0.5, 0.5, -0.7071067812}}, {{-0.5, -0.5, -0.7071067812}}
						      }};

	std::list<std::array<float,3>> points;

	std::array<std::array<float,3>, 14> temp_pts = {{
							{{0, 0, 1}}, {{0, 0, -1}}, {{0, 1, 0}}, {{0, -1, 0}}, {{1, 0, 0}}, {{-1, 0, 0}},
							{{0.57735026919, 0.57735026919, 0.57735026919}}, {{0.57735026919, -0.57735026919, 0.57735026919}},
							{{0.57735026919, 0.57735026919, -0.57735026919}}, {{0.57735026919, -0.57735026919, -0.57735026919}},
							{{-0.57735026919, 0.57735026919, 0.57735026919}}, {{-0.57735026919, -0.57735026919, 0.57735026919}},
							{{-0.57735026919, 0.57735026919, -0.57735026919}}, {{-0.57735026919, -0.57735026919, -0.57735026919}}
						      }};

	std::list<std::array<float,3>> points_temp;

	void findPoints();
};

#endif /* SPHEREPOS_HPP_ */
