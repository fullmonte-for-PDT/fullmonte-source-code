#include <limits>

#include "StandardArrayKernel.hpp"
#include "BoundingBox.hpp"
#include "Point.hpp"
#include "Vector.hpp"
#include "Points.hpp"
#include "Partition.hpp"

#include <iomanip>

#include "TetraMesh.hpp"

#include "FaceLinks.hpp"

#include <array>

using namespace std;

TetraMesh::TetraMesh()
{
}

TetraMesh::~TetraMesh()
{
}

void TetraMesh::regions(Partition* p)
{
	m_regions = p;
}

Partition* TetraMesh::regions() const
{
	return m_regions;
}

void TetraMesh::points(Points* p)
{
	m_points = p;
}

Points* TetraMesh::points() const
{
	return m_points;
}

void TetraMesh::tetraCells(TetraCells* T)
{
	m_tetraCells=T;
}

TetraCells* TetraMesh::tetraCells() const
{
	return m_tetraCells;
}

void TetraMesh::internalFacesForFluenceCounting(Partition* p)
{
	m_internalFacesForFluenceCounting=p;
}

Partition* TetraMesh::internalFacesForFluenceCounting() const
{
	return m_internalFacesForFluenceCounting;
}


/** get tetra volume: Calculate directly from point coordinates
 * 1/6 * AB x BC . AD
 */

double get(volume_tag,const TetraMesh& M,TetraMesh::TetraDescriptor T)
{
	std::array<Point<3,double>,4> Ps = get(point_coords,M,T);

	return dot(
		Ps[3]-Ps[0],
		cross(Ps[0],Ps[1],Ps[2])
		) /6.0;
}

TetraMesh::FaceDescriptor get(face_tag,const TetraMesh&,TetraMesh::DirectedFaceDescriptor F)
{
	return TetraMesh::FaceDescriptor(F.value()>>1);
}

double get(area_tag,const TetraMesh& M,TetraMesh::FaceDescriptor F)
{
	array<Point<3,double>,3> P=get(point_coords,M,F);
	return 0.5*norm_l2(cross(P[0],P[1],P[2]));
}

double get(area_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor F)
{
	return get(area,
			M,
			get(face,M,F));		// return area of the undirected face descriptor
}


FaceTetraLink get(face_link_tag,const TetraMesh& M,TetraMesh::FaceDescriptor IDf)
{
	return M.faceTetraLinks()->get(IDf.value());
}

Point<3,double> get(coords_tag,const TetraMesh& M,TetraMesh::PointDescriptor IDp)
{
	return M.points()->get(IDp.value());
}

TetraMesh::PointDescriptor get(point_above_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor IDf)
{
	FaceTetraLink F = get(face_link, M, IDf);
	return TetraMesh::PointDescriptor(F.upTet().vertexPoint);
}

TetraMesh::PointDescriptor get(point_below_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor IDf)
{
	FaceTetraLink F = get(face_link, M, IDf);
	return TetraMesh::PointDescriptor(F.downTet().vertexPoint);
}


TetraMesh::TetraDescriptor get(tetra_above_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor IDf)
{
	FaceTetraLink F = get(face_link, M, IDf);
	return F.tets[0].T;
}

TetraMesh::TetraDescriptor get(tetra_below_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor IDf)
{
	FaceTetraLink F = get(face_link, M, IDf);
	return F.tets[1].T;
}

TetraMesh::TetraDescriptor get(tetra_above_face_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor IDf)
{
	FaceTetraLink F = get(face_link, M, get(face,M,IDf));

	return F.tets[IDf.value() & 1].T;
}

TetraMesh::TetraDescriptor get(tetra_below_face_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor IDf)
{
	FaceTetraLink F = get(face_link, M, get(face,M,IDf));

	return F.tets[!(IDf.value() & 1)].T;
}

unsigned get(region_tag,const TetraMesh& M,TetraMesh::TetraDescriptor IDt)
{
	return M.regions()->get(IDt.value());
}

//TetraMesh::FaceDescriptor 			get(face_tag,const TetraMesh&,TetraMesh::TetraDescriptor,unsigned i)
//{
//	//return get(face,M,T,i);
//}
//
//TetraMesh::DirectedFaceDescriptor 	get(face_tag,const TetraMesh&,TetraMesh::TetraDescriptor,unsigned i)
//{
//	//return get(face,M,T,i);
//}



TetraMesh::FaceRange TetraMesh::faces() const
{
	FaceTetraLinks* faces = faceTetraLinks();

	if (!faces)
		throw std::logic_error("TetraMesh::faces() called with no faces supplied");

	return FaceRange(
		FaceIterator(FaceDescriptor(0)),
		FaceIterator(FaceDescriptor(faces->size())));
}

TetraMesh::DirectedFaceRange TetraMesh::directedFaces() const
{
	FaceTetraLinks* faces = faceTetraLinks();

	if (!faces)
		throw std::logic_error("TetraMesh::directedFaces() called with no faces supplied");

	return DirectedFaceRange(
			DirectedFaceIterator(DirectedFaceDescriptor(0U)),
			DirectedFaceIterator(DirectedFaceDescriptor(faces->size()*2U)));
}

TetraMesh::TetraRange TetraMesh::tetras() const
{
	if (!m_tetraCells)
		throw std::logic_error("TetraMesh::tetras() called without any tetra cells");

	return TetraRange(
			TetraIterator(TetraDescriptor(0U)),
			TetraIterator(TetraDescriptor(tetraCells()->size())));
}

FaceTetraLinks* TetraMesh::faceTetraLinks() const
{
	return m_faceTetraLinks;
}

void TetraMesh::faceTetraLinks(FaceTetraLinks* f)
{
	m_faceTetraLinks=f;
}

void TetraMesh::tetraFaceLinks(TetraFaceLinks* t)
{
	m_tetraFaceLinks = t;
}

TetraFaceLinks* TetraMesh::tetraFaceLinks() const
{
	return m_tetraFaceLinks;
}

unsigned get(num_points_tag,const TetraMesh& M){ return M.points()->size(); }


