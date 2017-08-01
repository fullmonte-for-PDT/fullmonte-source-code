#ifndef TETRAMESH_HPP_INCLUDED_
#define TETRAMESH_HPP_INCLUDED_

#include <vector>

#include <boost/range/algorithm.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/counting_range.hpp>

#include <type_traits>

#include <iterator>

#include "Geometry.hpp"

#ifndef SWIG

#include "Cells.hpp"

template<typename T,typename Index>class WrappedVector;
struct FaceTetraLink;
struct TetraFaceLink;
using FaceTetraLinks = WrappedVector<FaceTetraLink,unsigned>;
using TetraFaceLinks = WrappedVector<std::array<TetraFaceLink,4>,unsigned>;

template<std::size_t D,typename T>class Point;
class Points;
class Unit;

#ifndef SWIG

#define CREATE_GEOMETRY_PROPERTY_TAG(prop) struct prop##_tag { constexpr prop##_tag(){} }; constexpr prop##_tag prop;

template<typename PointProp>struct point_prop
{ constexpr point_prop(){} };

CREATE_GEOMETRY_PROPERTY_TAG(points);
CREATE_GEOMETRY_PROPERTY_TAG(area);
CREATE_GEOMETRY_PROPERTY_TAG(coords);
//CREATE_GEOMETRY_PROPERTY_TAG(point_coords);
CREATE_GEOMETRY_PROPERTY_TAG(num_faces);
CREATE_GEOMETRY_PROPERTY_TAG(num_directed_faces);
CREATE_GEOMETRY_PROPERTY_TAG(num_tetras);
CREATE_GEOMETRY_PROPERTY_TAG(num_points);
CREATE_GEOMETRY_PROPERTY_TAG(volume);
CREATE_GEOMETRY_PROPERTY_TAG(id);
CREATE_GEOMETRY_PROPERTY_TAG(face_link);
CREATE_GEOMETRY_PROPERTY_TAG(tetra_link);
CREATE_GEOMETRY_PROPERTY_TAG(region)
CREATE_GEOMETRY_PROPERTY_TAG(face_plane);

CREATE_GEOMETRY_PROPERTY_TAG(point_above_face)
CREATE_GEOMETRY_PROPERTY_TAG(point_below_face)
CREATE_GEOMETRY_PROPERTY_TAG(tetra_above_face)
CREATE_GEOMETRY_PROPERTY_TAG(tetra_below_face)
CREATE_GEOMETRY_PROPERTY_TAG(face)
CREATE_GEOMETRY_PROPERTY_TAG(directed_face)
CREATE_GEOMETRY_PROPERTY_TAG(faces)
CREATE_GEOMETRY_PROPERTY_TAG(directed_faces)

typedef point_prop<coords_tag> point_coords_tag;
constexpr point_prop<coords_tag> point_coords = point_prop<coords_tag>();


template<typename I>class WrappedInteger
{
public:
	WrappedInteger(){}
	explicit WrappedInteger(I v) : m_value(v){}

	explicit operator I() const { return m_value; }

	bool operator==(const WrappedInteger<I>& rhs) const { return rhs.m_value==m_value; }
	bool operator!=(const WrappedInteger<I>& rhs) const { return rhs.m_value!=m_value; }

	WrappedInteger& operator++(){ m_value++; return *this; }

	WrappedInteger& operator+=(std::ptrdiff_t rhs)					{ m_value += rhs; return *this; }
	WrappedInteger  operator+ (const WrappedInteger<I> rhs)	const 	{ return WrappedInteger(rhs.m_value + m_value); }
	std::ptrdiff_t	operator- (const WrappedInteger<I> rhs)			{ return m_value-rhs.m_value;}

	WrappedInteger& operator=(const WrappedInteger<I>&) = default;

	I value() const { return m_value; }

protected:
	I m_value=I();
};

#endif


typedef std::array<unsigned,3> FaceByPointID;
typedef std::array<unsigned,4> TetraByPointID;


/** In lexicographical order, gives the point indices of all four faces */
//
//constexpr std::array<std::array<unsigned char,3>,4> tetra_face_indices {{
//	std::array<unsigned char,3>{{ 0, 1, 2 }},
//	std::array<unsigned char,3>{{ 0, 1, 3 }},
//	std::array<unsigned char,3>{{ 0, 2, 3 }},
//	std::array<unsigned char,3>{{ 1, 2, 3 }}
//}};


/** TetraMesh contains the bare essentials of a tetrahedral mesh: points, tetras (by 4 point indices) and a region code for
 * each tetra. Generally tetra 0 and point 0 are dummy entries, to facilitate conversion between 0-based and 1-based indexing.
 * Likewise region 0 is a special code to indicate the exterior of the mesh.
 *
 */

class Points;
class Partition;




/** Gives the point indices of all four faces, and the point opposite them.
 * Will maintain the orientation of the tetra it's applied to, ie. ccw tetra -> ccw faces & point above
 *
 * If the input tetra is given in minimum oriented position (ie. the lowest lexicographical ordering consistent with
 * the orientation), the resulting faces will have the same property
 * */

struct face_opposite {
	std::array<unsigned char,3> faceidx;
	unsigned 					oppidx;
};

constexpr std::array<face_opposite,4> tetra_face_opposite_point_indices {{
	face_opposite{std::array<unsigned char,3>{{ 0, 1, 2 }},3},
	face_opposite{std::array<unsigned char,3>{{ 0, 3, 1 }},2},
	face_opposite{std::array<unsigned char,3>{{ 0, 2, 3 }},1},
	face_opposite{std::array<unsigned char,3>{{ 1, 3, 2 }},0}
}};



/** In lexicographical order, gives the point indices of all six edge pairs of a tetra */

constexpr std::array<std::array<unsigned char,2>,6> tetra_edge_indices {{
	std::array<unsigned char,2>{{ 0, 1}},
	std::array<unsigned char,2>{{ 0, 2}},
	std::array<unsigned char,2>{{ 0, 3}},
	std::array<unsigned char,2>{{ 1, 2}},
	std::array<unsigned char,2>{{ 1, 3}},
	std::array<unsigned char,2>{{ 2, 3}}
}};


using namespace std;

#endif // SWIG


//typedef std::array<int,4> TetraByFaceID;



/** TetraMesh represents a tetrahedral mesh geometry.
 *
 *
 * Point 0, tetra 0, and face 0 are special dummy entries meant to represent not-a-point, not-a-tetra, not-a-face.
 *
 *
 *
 */

class TetraMesh : public Geometry
{
public:
	TetraMesh();
	virtual ~TetraMesh();

	/// Get/set the region partition (assignment of tetras to regions)
	void 			regions(Partition* p);
	Partition* 		regions() const;

	/// Get/set the points used
	void			points(Points* p);
	Points*			points() const;

	/// Get/set the tetras
	void			tetraCells(TetraCells* T);
	TetraCells*		tetraCells() const;

    /// Get/set the face-tetra links
    void			faceTetraLinks(FaceTetraLinks* f);
	FaceTetraLinks*	faceTetraLinks() const;

	/// Get/set the tetra-face links
	void 			tetraFaceLinks(TetraFaceLinks* t);
	TetraFaceLinks*	tetraFaceLinks() const;

	void			internalFacesForFluenceCounting(Partition* p);
	Partition*		internalFacesForFluenceCounting() const;

    class PointDescriptor;

    typedef boost::counting_iterator<
    		PointDescriptor,
			std::random_access_iterator_tag,
			std::ptrdiff_t>	PointIterator;

    typedef boost::iterator_range<PointIterator> 			PointRange;


    class FaceDescriptor;

    typedef boost::counting_iterator<
    		FaceDescriptor,
			std::random_access_iterator_tag,
			std::ptrdiff_t>	FaceIterator;

    typedef  boost::iterator_range<FaceIterator> FaceRange;



    class DirectedFaceDescriptor;

    typedef boost::counting_iterator<
    		DirectedFaceDescriptor,
			std::random_access_iterator_tag,
			std::ptrdiff_t>	DirectedFaceIterator;

    typedef  boost::iterator_range<DirectedFaceIterator> DirectedFaceRange;

    class TetraDescriptor;

    typedef boost::counting_iterator<
        			TetraDescriptor,
        			std::random_access_iterator_tag,
        			std::ptrdiff_t> TetraIterator;

    typedef  boost::iterator_range<TetraIterator> TetraRange;

    TetraRange 			tetras() const;
    FaceRange 			faces() const;
    DirectedFaceRange 	directedFaces() const;
    //        PointRange	points() const;


private:
	Partition*		m_regions=nullptr;
	Partition*		m_internalFacesForFluenceCounting=nullptr;
	Points*			m_points=nullptr;
	TetraCells*		m_tetraCells=nullptr;

	FaceTetraLinks*	m_faceTetraLinks=nullptr;
	TetraFaceLinks* m_tetraFaceLinks=nullptr;
};


#ifndef SWIG

class TetraMesh::PointDescriptor : public WrappedInteger<unsigned>
{
public:
	explicit PointDescriptor(unsigned i=0) : WrappedInteger<unsigned>(i){}

};

class TetraMesh::TetraDescriptor : public WrappedInteger<unsigned>
{
public:
	explicit TetraDescriptor(unsigned i=0) : WrappedInteger<unsigned>(i){}
};

double get(volume_tag,const TetraMesh& M,TetraMesh::TetraDescriptor T);




/** Get a point property for all points of a tetra */


inline TetraByPointID get(points_tag,const TetraMesh& M,TetraMesh::TetraDescriptor IDt)
{
	return M.tetraCells()->get(IDt.value());
}


template<typename PointProp>
	std::array< decltype(get(PointProp(),declval<const TetraMesh&>(),TetraMesh::PointDescriptor())),4>
	get(
		point_prop<PointProp> p,
		const TetraMesh& M,
		typename TetraMesh::TetraDescriptor IDt)
	{
	typedef decltype(get(PointProp(),M,std::declval<TetraMesh::PointDescriptor>())) result_type;
	std::array<result_type,4> res;
	TetraByPointID IDps = get(points,M,IDt);

	for(unsigned i=0;i<4;++i)
		res[i] = get(PointProp(),M,TetraMesh::PointDescriptor(IDps[i]));

	return res;
	}

//template<typename PointProp>
//	std::array< decltype(get(PointProp(),declval<const TetraMesh&>(),TetraMesh::PointDescriptor())),4>
//	get(
//		PointProp p,
//		const TetraMesh& M,
//		typename TetraMesh::TetraDescriptor IDt)
//	{
//	typedef decltype(get(PointProp(),M,std::declval<TetraMesh::PointDescriptor>())) result_type;
//	std::array<result_type,4> res;
//	TetraByPointID IDps = get(points,M,IDt);
//
//	for(unsigned i=0;i<4;++i)
//		res[i] = get(PointProp(),M,TetraMesh::PointDescriptor(IDps[i]));
//
//	return res;
//	}

//template<>std::array<TetraMesh::PointDescriptor,4> get(point_prop<points_tag>,const TetraMesh& M,TetraMesh::TetraDescriptor T)
//{
//	TetraByPointID IDps = M.tetraCells()->get(IDt.value());
//	std::array<TetraMesh::PointDescriptor,4> a;
//	for(unsigned i=0;i<4;++i)
//		a[i] = TetraMesh::PointDescriptor(IDps[i]);
//	return a;
//}

#ifndef SWIG

class TetraMesh::FaceDescriptor : public WrappedInteger<unsigned>
{
public:
	explicit FaceDescriptor(unsigned i) : WrappedInteger<unsigned>(i){}
	FaceDescriptor(){}

private:
	friend class DirectedFaceDescriptor;
};

class TetraMesh::DirectedFaceDescriptor : public WrappedInteger<unsigned>
{
public:
	/// Allow implicit conversion of FaceDescriptor into DirectedFaceDescriptor
	explicit DirectedFaceDescriptor(unsigned i) : WrappedInteger<unsigned>(i){}

	explicit DirectedFaceDescriptor(const FaceDescriptor& F,bool invert=false) :
		WrappedInteger<unsigned>((F.m_value << 1) | invert){}

	DirectedFaceDescriptor(){}
};





#endif

#include "Cells.hpp"


Point<3,double> get(coords_tag,const TetraMesh& M,TetraMesh::PointDescriptor IDp);

array<TetraMesh::PointDescriptor,3> get(points_tag,const TetraMesh&M,TetraMesh::FaceDescriptor);

TetraMesh::PointDescriptor get(point_above_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor);
TetraMesh::PointDescriptor get(point_below_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor);

TetraMesh::TetraDescriptor get(tetra_above_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor);
TetraMesh::TetraDescriptor get(tetra_below_face_tag,const TetraMesh& M,TetraMesh::FaceDescriptor);

TetraMesh::TetraDescriptor get(tetra_below_face_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor);
TetraMesh::TetraDescriptor get(tetra_above_face_tag,const TetraMesh& M,TetraMesh::DirectedFaceDescriptor);

inline unsigned get(id_tag,const TetraMesh&,TetraMesh::TetraDescriptor IDt){ return IDt.value(); }

unsigned get(region_tag,const TetraMesh&,TetraMesh::TetraDescriptor IDt);

double get(area_tag,const TetraMesh&,TetraMesh::FaceDescriptor);
double get(area_tag,const TetraMesh&,TetraMesh::DirectedFaceDescriptor);

TetraMesh::FaceDescriptor 			get(face_tag,const TetraMesh&,TetraMesh::TetraDescriptor,unsigned i);
TetraMesh::DirectedFaceDescriptor 	get(directed_face_tag,const TetraMesh&,TetraMesh::TetraDescriptor,unsigned i);

TetraMesh::FaceDescriptor get(face_tag,const TetraMesh&,TetraMesh::DirectedFaceDescriptor);

inline unsigned get(num_tetras_tag,const TetraMesh& M){ return M.tetraCells()->size(); }
unsigned get(num_points_tag,const TetraMesh& M);



#endif

#endif
