/*
 * FaceLinks.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_FACELINKS_HPP_
#define GEOMETRY_FACELINKS_HPP_

#include "WrappedVector.hpp"
#include "TetraMesh.hpp"

#include <FullMonteSW/Geometry/UnitVector.hpp>

/** FaceTetraLink specifies a face that links two tetras.
 *
 * The point IDs (A,B,C) should always be given in ascending index order.
 * The normal can be computed by normalizing AB x AC.
 *
 * upTet/downTet should be specified so that the point of upTet that is not in the face is above the face.
 */

struct FaceTetraLink
{
	struct TetraLink
	{
		TetraMesh::TetraDescriptor T;
		TetraMesh::PointDescriptor vertexPoint;
	};

	std::array<TetraMesh::PointDescriptor,3>		pointIDs;

	TetraLink&		upTet(){ return tets[0]; }
	TetraLink& 		downTet(){ return tets[1]; }

	array<TetraLink,2>	tets;
};

struct TetraFaceLink
{
	TetraMesh::DirectedFaceDescriptor 	faceID;
	TetraMesh::TetraDescriptor			tetraID;
};

typedef WrappedVector<FaceTetraLink,unsigned> FaceTetraLinks;
typedef WrappedVector<array<TetraFaceLink,4>,unsigned> TetraFaceLinks;


inline std::array<TetraMesh::PointDescriptor,3> get(points_tag,const TetraMesh& M,TetraMesh::FaceDescriptor F)
{
	return M.faceTetraLinks()->get(F.value()).pointIDs;
}

inline std::array<TetraMesh::PointDescriptor,3> get(points_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor F)
{
	std::array<TetraMesh::PointDescriptor,3> Ps = M.faceTetraLinks()->get(F.value() >> 1).pointIDs;
	if (F.value() & 1)
		std::swap(Ps[1],Ps[2]);
	return Ps;
}

inline std::array<TetraMesh::FaceDescriptor,4> get(faces_tag,const TetraMesh& M,TetraMesh::TetraDescriptor T)
{
	const auto L = M.tetraFaceLinks()->get(T.value());
	std::array<TetraMesh::FaceDescriptor,4> Fu;
	for(unsigned i=0;i<4;++i)
		Fu[i] = get(face,M,L[i].faceID);
	return Fu;
}

inline std::array<TetraMesh::DirectedFaceDescriptor,4> get(directed_faces_tag,const TetraMesh& M,TetraMesh::TetraDescriptor T)
{
	const auto L = M.tetraFaceLinks()->get(T.value());
	std::array<TetraMesh::DirectedFaceDescriptor,4> F;
	for(unsigned i=0;i<4;++i)
		F[i] = L[i].faceID;
	return F;
}

template<typename FaceProp>struct face_prop { constexpr face_prop(){} };

/** Get a point property for all point on a face
 */


template<typename PointProp>
	std::array<decltype(get(PointProp(),declval<const TetraMesh&>(),TetraMesh::PointDescriptor())),3>
	get(
		point_prop<PointProp>,
		const TetraMesh& M,
		typename TetraMesh::FaceDescriptor IDf)
	{
	typedef decltype(get(PointProp(),M,std::declval<TetraMesh::PointDescriptor>())) result_type;
	std::array<result_type,3> res;
	std::array<TetraMesh::PointDescriptor,3> IDps = get(points,M,IDf);

	for(unsigned i=0;i<3;++i)
		res[i] = get(PointProp(),M,TetraMesh::PointDescriptor(IDps[i]));

	return res;
	}

template<typename PointProp>
	std::array<decltype(get(PointProp(),declval<const TetraMesh&>(),TetraMesh::PointDescriptor())),3>
	get(
		point_prop<PointProp>,
		const TetraMesh& M,
		typename TetraMesh::DirectedFaceDescriptor IDfd)
	{
	typedef decltype(get(PointProp(),M,std::declval<TetraMesh::PointDescriptor>())) result_type;
	std::array<result_type,3> res;
	std::array<TetraMesh::PointDescriptor,3> IDps = get(points,M,IDfd);

	for(unsigned i=0;i<3;++i)
		res[i] = get(PointProp(),M,TetraMesh::PointDescriptor(IDps[i]));

	return res;
	}

template<typename FaceProp>
	std::array<decltype(get(FaceProp(),declval<const TetraMesh&>(),TetraMesh::FaceDescriptor())),4>
	get(
			face_prop<FaceProp>,
			const TetraMesh& M,
			typename TetraMesh::TetraDescriptor IDt)
	{
		typedef decltype(get(FaceProp(),M,std::declval<TetraMesh::FaceDescriptor>())) result_type;
		std::array<result_type,4> res;
		std::array<TetraMesh::FaceDescriptor,4> IDfus = get(faces,M,IDt);

		for(unsigned i=0;i<4;++i)
			res[i] = get(FaceProp(),M,IDfus[i]);

		return res;
	}
//
//template<typename Prop>
//	std::array<decltype(get(Prop(),declval<const TetraMesh&>(),TetraMesh::PointDescriptor())),3>
//	get(
//		Prop p,
//		const TetraMesh& M,
//		typename TetraMesh::FaceDescriptor IDf)
//	{
//	typedef decltype(get(p,M,std::declval<TetraMesh::PointDescriptor>())) result_type;
//	std::array<result_type,3> res;
//	FaceByPointID IDps = get(points,M,IDf);
//
//	for(unsigned i=0;i<3;++i)
//		res[i] = get(p,M,TetraMesh::PointDescriptor(IDps[i]));
//
//	return res;
//	}

inline TetraMesh::DirectedFaceDescriptor get(directed_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor F,bool down=false)
{
	return TetraMesh::DirectedFaceDescriptor((F.value() << 1) | down);
}

FaceTetraLink get(face_link_tag,const TetraMesh& M,TetraMesh::FaceDescriptor IDf);
inline unsigned get(num_faces_tag,const TetraMesh& M){ return M.faceTetraLinks()->size(); }
inline unsigned get(num_directed_faces_tag,const TetraMesh& M){ return M.faceTetraLinks()->size()*2; }

inline unsigned get(id_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor Fd)
{ return Fd.value(); }

inline unsigned get(id_tag,const TetraMesh& M,TetraMesh::FaceDescriptor Fu)
{ return Fu.value(); }

inline std::pair<UnitVector<3,double>,double> get(face_plane_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor IDfd)
{
	std::array<Point<3,double>,3> P = get(point_coords,M,IDfd);
	UnitVector<3,double> n = normalize(cross(P[0],P[1],P[2]));

	double c = dot(n,Vector<3,double>(P[0]));
	return std::make_pair(n,c);
}

inline std::pair<UnitVector<3,double>,double> get(face_plane_tag,const TetraMesh& M,TetraMesh::FaceDescriptor IDfu)
{
	std::array<Point<3,double>,3> P = get(point_coords,M,IDfu);
	UnitVector<3,double> n = normalize(cross(P[0],P[1],P[2]));

	double c = dot(n,Vector<3,double>(P[0]));
	return std::make_pair(n,c);
}

#endif /* GEOMETRY_FACELINKS_HPP_ */
