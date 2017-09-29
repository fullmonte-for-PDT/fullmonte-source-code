/*
 * Layer.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_LAYER_HPP_
#define GEOMETRY_LAYER_HPP_

class Material;

class Layer
{
public:
	Layer();
	virtual ~Layer();

	void thickness(float t);
	float thickness() 					const;

private:

	float		m_thickness=0.0f;
};


#endif /* GEOMETRY_LAYER_HPP_ */
