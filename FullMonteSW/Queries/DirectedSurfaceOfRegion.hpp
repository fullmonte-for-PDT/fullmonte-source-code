/*
 * DirectedSurface.hpp
 *
 *  Created on: Jun 6, 2017
 *      Author: jcassidy
 */

#ifndef QUERIES_DIRECTEDSURFACEOFREGION_HPP_
#define QUERIES_DIRECTEDSURFACEOFREGION_HPP_


class OutputData;
class TetraMesh;
class Partition;
template<class T>class SpatialMap;

#include <vector>

/** Some kernels will return a map over directed faces. Those directed faces are oriented as in the original
 * geometry description, which may or may not conform to orientation with respect to a specific sub-object within that
 * geometry. This query flips the orientation from the original geometry into orientation with respect to the region
 * partition provided.
 *
 * Each face will appear twice in the input, in positive and negative orientation with respect to the original geometry.
 *
 * The output will appear in the same order as the input and each face will still appear twice, but the associated data will
 * be provided with respect to the provided region partition.
 *
 *
 * For a single face f with directed faces (u,d) for up/down orientation,
 *
 * 		entering/exiting/total[u]		The fluence entering/exiting region u
 * 		region[u]						The region that normal(u) points into
 *
 * 		entering/exiting/total[d]		The fluence entering/exiting region d (swap entering/exiting[u])
 * 		region[d]						The region that normal(d) = -normal(u) points into
 *
 * Inputs
 * 		partition		The region partition to use (if null, use mesh's existing partition)
 * 		mesh
 * 		source			The data field to process
 *
 * Outputs
 * 		entering		The total values entering the region
 * 		exiting
 * 		total			Entering + exiting
 * 		region			The region code for the face
 *
 *
 * TODO: Redundant data (most faces interior & probably not scored -> sparse)
 * TODO: Redundant data (any face can be both entering & exiting separate regions, probably not needed often)
 * TODO: Redundant data (total() gives a map over directed faces but it contains duplicates)
 * TODO: Does not (yet) work with interior faces.
 *
 * Consider providing N output sub-surfaces instead of maps over all directed faces?
 */


class DirectedSurfaceOfRegion
{
public:
	DirectedSurfaceOfRegion();
	~DirectedSurfaceOfRegion();

	void			source(OutputData*);			///< Map over directed faces
	void			mesh(TetraMesh*);				///< The tetra mesh (needed to link tetra regions with directed/undirected faces)
	void			partition(Partition*);			///< Partition defining in and out regions for the surface

	void 			update();

	OutputData*		entering() const;				///< Map over directed faces of fluence entering region
	OutputData*		exiting() const;				///< Map over directed faces of fluence exiting region
	OutputData*		total() const;					///< Map over directed faces of total (entering + exiting) fluence
	Partition*		region() const;					///< Partition map for entering/exiting/total

private:
	Partition*			m_partition=nullptr;
	TetraMesh*			m_mesh=nullptr;
	SpatialMap<float>*	m_directedData=nullptr;

	struct UndirectedFaceInfo
	{
		unsigned upRegion;
		unsigned dnRegion;
		unsigned upIDf;
		unsigned dnIDf;
	};

	std::vector<UndirectedFaceInfo>		m_faces;
};


#endif /* QUERIES_DIRECTEDSURFACEOFREGION_HPP_ */
