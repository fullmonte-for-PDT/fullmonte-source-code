/*
 * PointTetraTetraMeshBuilder.cpp
 *
 *  Created on: Dec 16, 2016
 *      Author: jcassidy
 */

#include <FullMonteSW/Geometry/PTTetraMeshBuilder.hpp>

#include <iostream>

#include <boost/range/algorithm_ext/is_sorted.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <FullMonteSW/take_drop.hpp>
#include <unordered_map>

#include <cassert>

#include <boost/functional/hash.hpp>

PTTetraMeshBuilder::PTTetraMeshBuilder()
{
}

PTTetraMeshBuilder::~PTTetraMeshBuilder()
{
}

void PTTetraMeshBuilder::build()
{
	// sort all tetras into ascending index order
	sortTetraPoints();

	// swap IDp[2] and IDp[3] if needed to get ccw ordering for all tetras
	orientTetras();

	// build the tetra-face links
	buildFacesFromTetras();
}


/** Uses the tetra points to construct faces and a bidirectional mapping between faces and tetras.
 *
 * Tetra point indices must already be in minimum oriented order (ie. index sorted then IDps[2], IDps[3] swapped to get ccw order)
 */

void PTTetraMeshBuilder::buildFacesFromTetras()
{
	struct BuildFaceInfo
	{
		unsigned								IDfu;			///< Undirected face index
		std::array<FaceInfo::FaceTetraInfo,2>	tetras;			///< up [0] and down [1] tetra links
	};


	// maps a tuple of points to a face ID plus up/down incident tetra info
	// Each face has 3! possible permutations; use the one such that  IDp0 < IDp1 < IDp2 for uniqueness

	typedef std::unordered_map<
			FaceByPointID,
			BuildFaceInfo,
			boost::hash<std::array<unsigned,3>>> PointFaceMap;
	PointFaceMap pointIDsToFaceMap;


	// Nf <= 4Nt for sure (== if no tetras sharing faces)
	// Nf = 2Nt in the limit of no boundary (every face impinges on 2 tetras, every tetra has 4 faces)
	unsigned Nf_est = 4*m_tetraPoints.size();

	pointIDsToFaceMap.reserve(Nf_est);

	m_faces.clear();

	m_tetraFaces.resize(m_tetraPoints.size());
	m_tetraFaces[0] = array<unsigned,4>{{0U,0U,0U,0U}};

	cout << "Building faces with Np=" << m_points.size() << " Nt=" << m_tetraPoints.size() << endl;

	for(auto T : m_tetraPoints | drop(1) | boost::adaptors::indexed(1U)) // remove the first (dummy) tetra
	{
		// loop over all face combinations of the tetra points
		for(const auto& perm : tetra_face_opposite_point_indices | boost::adaptors::indexed(0U))
		{
			FaceByPointID Ft{{
					T.value()[perm.value().faceidx[0]],
					T.value()[perm.value().faceidx[1]],
					T.value()[perm.value().faceidx[2]]}};

			// faces are all stored in ascending index order so if this face not so ordered --> tetra is on the down side
			bool downFace = Ft[1] > Ft[2];

			FaceByPointID Ft_sorted = Ft;
			if (downFace)
				swap(Ft_sorted[1],Ft_sorted[2]);

			assert(boost::is_sorted(Ft_sorted));

			// Must use sorted indices as key
			typename PointFaceMap::iterator it;
			bool inserted;

			tie(it,inserted) = pointIDsToFaceMap.insert(
					make_pair(
							Ft_sorted,
							BuildFaceInfo()));

			// new face tuple inserted --> give it an ID and copy point IDs
			if(inserted)
				it->second.IDfu = pointIDsToFaceMap.size();

			// add reference to up or down tet as appropriate
			it->second.tetras[downFace].IDt = T.index();
			it->second.tetras[downFace].IDp_opposite = T.value()[perm.value().oppidx];

			// add directed face reference to the tetra
			m_tetraFaces[T.index()][perm.index()] = (it->second.IDfu << 1) | downFace;
		}
	}

	// create faces vector and add the zero face
	m_faces.resize(pointIDsToFaceMap.size()+1);

	m_faces[0].IDps = array<unsigned,3>{{0U,0U,0U}};
	m_faces[0].tetras[0].IDt=m_faces[0].tetras[0].IDp_opposite=0;
	m_faces[0].tetras[1]=m_faces[0].tetras[0];

	// copy faces from hash to vector and count surface faces
	unsigned Ns=0;
	for(const auto f : pointIDsToFaceMap)
	{
		m_faces[f.second.IDfu].tetras = f.second.tetras;
		m_faces[f.second.IDfu].IDps = f.first;

		// surface face is incident on tetra 0
		Ns += (f.second.tetras[0].IDt == 0) | (f.second.tetras[1].IDt==0);
	}

	cout << "  Done (" << m_faces.size() << " faces, " << Ns << " on the surface)" << endl;

//	for(unsigned i=1;i<m_faceVertices.size(); ++i)
//		if (m_faceVertices[i][0] != 0 && !isCCW(m_facePoints[i],m_points[m_faceVertices[i][0]]))
//			cout << "Whoa! Face " << i << " is still off!" << endl;
//
//	for(unsigned i=1;i<m_faceVertices.size(); ++i)
//		if (m_faceVertices[i][1] != 0 && isCCW(m_facePoints[i],m_points[m_faceVertices[i][1]]))
//			cout << "Whoa! Face " << i << " is down tetra is still off!" << endl;

//
//	// check tetra integrity
//	for(unsigned i=1;i<m_facePoints[i].size();++i)
//	{
//		// check the up-tetra
//		bool err=false;
//		array<unsigned,4> face_IDps;
//		for(unsigned j=0;j<3;++j)
//			face_IDps[j] = m_facePoints[i][j];
//		face_IDps[3] = m_faceVertices[i][0];
//		boost::sort(face_IDps);
//
//		array<unsigned,4> upTet_IDps = m_tetraPoints[m_faceTetras[i][0]];
//		boost::sort(upTet_IDps);
//
//		if (face_IDps != upTet_IDps)
//		{
//			err=true;
//			cout << "ERROR: Face " << i << " mismatch on up-tetra" << endl;
//		}
//
//		// check the down-tetra
//		for(unsigned j=0;j<3;++j)
//			face_IDps[j] = m_facePoints[i][j];
//		face_IDps[3] = m_faceVertices[i][1];
//		boost::sort(face_IDps);
//
//		array<unsigned,4> downTet_IDps = m_tetraPoints[m_faceTetras[i][1]];
//		boost::sort(downTet_IDps);
//
//		if (face_IDps != downTet_IDps)
//		{
//			err=true;
//			cout << "ERROR: Face " << i << " mismatched on down-tetra" << endl;
//		}
//
//		if (err)
//		{
//			cout << "Face (" << m_facePoints[i][0] << ',' << m_facePoints[i][1] << ',' << m_facePoints[i][2] << ") " <<
//					" with up-tetra " << m_faceTetras[i][0] << " (" << upTet_IDps[0] << ',' << upTet_IDps[1] << ',' << upTet_IDps[2] << ',' << upTet_IDps[3] << ") " <<
//					" and down-tetra " << m_faceTetras[i][1] << " (" << downTet_IDps[0] << ',' << downTet_IDps[1] << ',' << downTet_IDps[2] << ',' << downTet_IDps[3] << ") " << endl;
//		}
//	}
}

void PTTetraMeshBuilder::setTetra(size_t i,array<unsigned,4> IDps,unsigned region)
{
	if (region != -1U)
		m_tetraMaterials[i] = region;
	m_tetraPoints[i]=IDps;
}
