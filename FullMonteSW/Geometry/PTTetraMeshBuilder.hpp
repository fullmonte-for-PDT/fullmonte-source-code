/*
 * PointTetraTetraMeshBuilder.hpp
 *
 *  Created on: Dec 16, 2016
 *      Author: jcassidy
 */

#ifndef GEOMETRY_PTTETRAMESHBUILDER_HPP_
#define GEOMETRY_PTTETRAMESHBUILDER_HPP_

#include "TetraMeshBuilder.hpp"

/** Builds a TetraMesh from points and tetras.
 *
 * Each tetra may have a material assigned.
 */

class PTTetraMeshBuilder : public TetraMeshBuilder
{
public:
	PTTetraMeshBuilder();
	virtual ~PTTetraMeshBuilder();

	/// Add the tetra representations
	void setTetra(std::size_t i,std::array<unsigned,4> IDps,unsigned IDm=-1U);

	virtual void build() override;



private:

	/** Creates faces from tetra points
	 * Requires: tetra point IDs, CCW oriented
	 * Produces: face info (face ID, point IDs, ()
	 */

	void buildFacesFromTetras();
};

#endif /* GEOMETRY_PTTETRAMESHBUILDER_HPP_ */
