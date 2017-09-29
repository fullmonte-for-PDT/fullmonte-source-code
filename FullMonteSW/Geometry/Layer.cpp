/*
 * Layer.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#include "Layer.hpp"

Layer::Layer()
{

}

Layer::~Layer()
{

}

void Layer::thickness(float t)
{
	m_thickness=t;
}

float Layer::thickness() const
{
	return m_thickness;
}
