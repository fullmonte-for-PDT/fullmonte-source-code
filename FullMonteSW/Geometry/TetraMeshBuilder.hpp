/*
 * TetraMeshBuilder.hpp
 *
 *  Created on: Nov 10, 2016
 *      Author: jcassidy
 */

#ifndef GEOMETRY_TETRAMESHBUILDER_HPP_
#define GEOMETRY_TETRAMESHBUILDER_HPP_

#include <array>
#include <vector>

#include "Point.hpp"
#include "TetraMesh.hpp"
#include "Cells.hpp"

/** Provides facilities for building a TetraMesh from various input information
 *
 * All TetraMeshes must have points, tetras, and tetra materials specified so do handle those here.
 * Derived classes will have means of building those items.
 *
 *
 * Face construction:
 *
 * 	(1)		Provide face point IDs into m_facePoints
 * 	(2)		Call buildFaces() to generate the data structures
 * 	(3)		Call orientFaceNormals() if needed to flip the faces into the right orientation+
 *
 * 	Notes:
 *
 * 	+ some representations don't require this step since they guarantee faces in the correct orientation
 */

class TetraMeshBuilder
{
public:
	TetraMeshBuilder();
	virtual ~TetraMeshBuilder();

	/// Point manipulation
	void setNumberOfPoints(std::size_t);
	void setPoint(std::size_t,Point<3,double> P);
	void setPoint(std::size_t,std::array<double,3> P);
	void setPoint(std::size_t,std::array<float,3> P);
	void setPoint(std::size_t,const float* P);
	void setPoint(std::size_t,const double* P);

	void setNumberOfFaces(std::size_t);

	void setNumberOfTetras(std::size_t);
	std::size_t getNumberOfTetras() const;

	/// Assign a tetra to a material
	void setTetraMaterial(std::size_t IDt,unsigned IDm);

	/// Sort all tetra point definitions into ascending order of index
	void sortTetraPoints();

	/// Flip (IDps[2], IDps[3]) as needed to get ccw orientation
	void orientTetras();

	virtual void build()=0;


	TetraMesh* mesh();


	/// Checks
	unsigned orientedTetras() const;
	unsigned sortedTetras() const;


	/// Check faces for appropriate orientation. Return the number having problems.

	unsigned orientedFaces() const;




	/// Build Face structures

	/** Construct face data structures from the provided triples of point IDs
	 *
	 * Preconditions:
	 * 		m_points populated
	 * 		m_facePoints populated (max index < m_points.size())
	 *
	 * Postconditions:
	 *
	 */
	//void buildFaces();
	//void orientNormals();

	/// Checks (non-modifying)
	//bool checkFaces() const;


	/// Get the resulting mesh
	TetraMesh*	mesh() const;



	/// Check counterclockwise orientation of the provided entity
	bool 			isCCW(TetraByPointID T) const;
	bool			isCCW(FaceByPointID F,Point<3,double> pUp) const;
	static bool		isCCW(array<Point<3,double>,4> P);

protected:
	struct FaceInfo
	{
		array<unsigned,3> 	IDps;			///< Point IDs in ascending index order

		struct FaceTetraInfo
		{
			unsigned IDt;					///< The tetra
			unsigned IDp_opposite;			///< The tetra point opposite the face
		};

		// tetras[0] is tetra above face, tetras[1] is below
		std::array<FaceTetraInfo,2>			tetras;
	};

	std::vector<Point<3,double>>			m_points;
	std::vector<TetraByPointID>				m_tetraPoints;
	std::vector<unsigned>					m_tetraMaterials;

	std::vector<
		std::array<unsigned,4>>				m_tetraFaces;


	/** Information on all faces. Up orientation is defined by CCW face with points in ascending order.
	 * A face is a surface face if one of its two attached tetras is 0.
	 */

	std::vector<FaceInfo>					m_faces;

	unsigned								m_materialCount=0U;

	/// Permutes vertices into ascending index order
	FaceByPointID	sortIndices(FaceByPointID F);
	TetraByPointID 	sortIndices(TetraByPointID T);

	/// Permutes vertices into minimum lexicographical order consistent with counterclockwise ordering
	/// (ie. ascending order for first N-1, then last 2 ordered as needed for ccw)
	TetraByPointID 	orient(TetraByPointID T);
	FaceByPointID	orient(FaceByPointID F,Point<3,double> pUp);

};

#endif /* GEOMETRY_TETRAMESHBUILDER_HPP_ */
