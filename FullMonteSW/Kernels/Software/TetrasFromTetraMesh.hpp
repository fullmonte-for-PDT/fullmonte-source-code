/*
 * TetrasFromTetraMesh.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_TETRASFROMTETRAMESH_HPP_
#define KERNELS_SOFTWARE_TETRASFROMTETRAMESH_HPP_


/** Creates kernel tetras from a TetraMesh description
 *
 *
 */

#include "Tetra.hpp"
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <vector>

class Partition;
class TetraMesh;

class TetrasFromTetraMesh
{
public:
	TetrasFromTetraMesh();
	~TetrasFromTetraMesh();

	void mesh(const TetraMesh* M);
	void update();

	/// Checks that the kernel faces make sense
	bool checkKernelFaces() const;

	/// Mark faces for fluence counting. Face i is counted if p[i] != 0
	void setFacesForFluenceCounting(const Partition* p);

	const std::vector<Tetra>& tetras() const;

private:

	void makeKernelTetras();
	void updateFaceCountingPartition();

	static Tetra convert(const TetraMesh& M,TetraMesh::TetraDescriptor T);

	/** Packed and aligned tetra representation for the kernel, holding face normals, constants, adjacent tetras,
	 * bounding faces, material ID, and face flags (for logging).
	 */

	const TetraMesh*				m_mesh=nullptr;
    std::vector<Tetra>               m_tetras;

    const Partition*				m_faceCountingPartition=nullptr;
};

#endif /* KERNELS_SOFTWARE_TETRASFROMTETRAMESH_HPP_ */
