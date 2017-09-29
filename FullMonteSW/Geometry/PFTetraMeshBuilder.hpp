/*
 * PointFaceTetraMeshBuilder.hpp
 *
 *  Created on: Dec 16, 2016
 *      Author: jcassidy
 */

#ifndef GEOMETRY_PFTETRAMESHBUILDER_HPP_
#define GEOMETRY_PFTETRAMESHBUILDER_HPP_

#include "TetraMeshBuilder.hpp"

/** Builds a mesh from a list of points and faces.
 * Each face must specify its points and connected tetras (up & down; 0 if none).
 *
 * Points:
 * 		(x,y,z) coordinates
 *
 * Faces:
 * 		IDp[0..2]		point indices (ccw order)
 * 		IDt_up
 * 		IDt_down
 *
 * Tetras:
 * 		material
 *
 * Total storage is 3Np coordinates, 5Nf indices, Nt material IDs
 */

class PFTetraMeshBuilder : public TetraMeshBuilder
{
public:
	PFTetraMeshBuilder();
	~PFTetraMeshBuilder();

	/** Incrementally constructs the mesh from faces consisting of 3 points, with the neighbouring tetras specified
	 *
	 * Preconditions:
	 * 		setNumberOfFaces called to pre-allocate
	 * 		setNumberOfTetras called to pre-allocate
	 *
	 * Postconditions:
	 */
	void setFace(std::size_t IDf,std::array<unsigned,3> IDps,std::array<unsigned,2> IDts);

private:
	void addFaceToTetra(std::size_t IDf,std::size_t IDt,std::array<unsigned,3> IDps,bool up);
};



#endif /* GEOMETRY_PFTETRAMESHBUILDER_HPP_ */
