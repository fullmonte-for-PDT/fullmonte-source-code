/*
 * Points.cpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#include "Points.hpp"
using namespace std;

Points::Points()
{
}

Points::~Points()
{
}

unsigned Points::size() const
{
	return m_points.size();
}

void Points::resize(unsigned N)
{
	m_points.resize(N,Point<3,double>{ .0,.0,.0 });
}

void Points::set(unsigned i,Point<3,double> p)
{
	m_points.at(i) = p;
}

Point<3,double> Points::get(unsigned i)
{
	return m_points.at(i);
}

