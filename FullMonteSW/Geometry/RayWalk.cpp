/*
 * RayWalk.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: jcassidy
 */

#include "TetraMesh.hpp"
#include "RayWalk.hpp"
#include "FaceLinks.hpp"

#include <FullMonteSW/Geometry/Queries/TetraEnclosingPointByLinearSearch.hpp>

#ifdef VERBOSE
#include <iostream>
#include <iomanip>
#endif

using namespace std;

LineQuery::LineQuery()
{
}

bool LineQuery::skipInitialZeros() const
{
	return m_skipInitialZeros;
}

void LineQuery::skipInitialZeros(bool skip)
{
	m_skipInitialZeros=skip;
}

const TetraMesh* LineQuery::mesh() const
{
	return m_mesh;
}

void LineQuery::mesh(const TetraMesh* m)
{
	m_mesh=m;
}

array<float,3> LineQuery::origin() const
{
	return m_origin;
}

void LineQuery::origin(array<float,3> p)
{
	m_origin=p;
}

void LineQuery::direction(array<float,3> d)
{
	m_direction=d;
}

array<float,3> LineQuery::direction() const
{
	return m_direction;
}

void LineQuery::destination(std::array<float,3> p1)
{
	array<float,3> delta{{ p1[0]-m_origin[0], p1[1]-m_origin[1], p1[2]-m_origin[2]}};
	float k = std::sqrt(delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2]);

	m_direction = array<float,3>{{ delta[0]/k, delta[1]/k, delta[2]/k }};
	m_length = k;
}

void LineQuery::length(float l)
{
	m_length=l;
}

float LineQuery::length() const
{
	return m_length;
}


boost::any_range<const WalkSegment,boost::forward_traversal_tag,const WalkSegment&,std::ptrdiff_t> LineQuery::result() const
{
	RayWalkIterator rwi = RayWalkIterator::init(*m_mesh,m_origin,m_direction);
	RayWalkIterator rwend;

	// inclusion filter for first elements (false -> skip)
	std::function<bool(const WalkSegment&)> f = std::function<bool(const WalkSegment&)>(
			[](const WalkSegment& seg){ return seg.matID != 0 && seg.IDt != 0; });

	std::function<bool(const WalkSegment&)>* m_initialFilter = m_skipInitialZeros ? &f : nullptr;

	for(; rwi != rwend && m_initialFilter && !(*m_initialFilter)(*rwi); ++rwi)
	{
#ifdef VERBOSE
		cout << "* tetra " << setw(7) << rwi->IDt << " material " << setw(2) << rwi->matID << " depth " << rwi->dToOrigin << endl;
#endif
	}

#ifdef VERBOSE
	cout << "Stepped through " << rwi->dToOrigin << " before starting walk" << endl;
#endif

	rwi = RayWalkIterator::restartFrom(rwi);

	return boost::any_range<
			const WalkSegment,
			boost::forward_traversal_tag,
			const WalkSegment&,
			std::ptrdiff_t>(rwi,rwend);
}



const WalkSegment& RayWalkIterator::dereference() const
{
	return m_currSeg;
}

/** Equality: either both in same tetra, or both at end */
bool RayWalkIterator::equal(const RayWalkIterator& rhs) const
{
	if (m_mesh == nullptr)				// if this == end, check if rhs == end too
		return rhs.m_mesh==nullptr || rhs.m_currSeg.dToOrigin > m_currSeg.dToOrigin;
	else							// if this != end, check if in same nonzero tetra (zero tetra is non-unique)
		return m_currSeg.IDt != 0 && m_currSeg.IDt == rhs.m_currSeg.IDt;
}

void RayWalkIterator::increment()
{
	m_currSeg.f0 = m_currSeg.f1;
	m_currSeg.f0.IDf = -m_currSeg.f0.IDf;
    m_currSeg.dToOrigin += m_currSeg.lSeg;
	m_currSeg.IDt = nextTet_;
    m_currSeg.matID = get(region, *m_mesh, TetraMesh::TetraDescriptor(nextTet_));//m_mesh->getMaterial(nextTet_);
	if (m_currSeg.IDt == 0)		// not currently in a tetra, need to check faces exhaustively
	{
		finishStepExterior();
		if (m_currSeg.f1.IDf == 0)		// no face found: return end iterator
			*this = RayWalkIterator();
	}
	else				// in a tetra, just four faces to check for next intersection (always will have, since we're in tetra)
		finishStepInTetra();
}

RayWalkIterator RayWalkIterator::init(const TetraMesh& M,const std::array<float,3> p0,const std::array<float,3> dir,float d0)
{
	RayWalkIterator rwi;

	rwi.m_mesh=&M;
	rwi.m_dir=dir;

	rwi.m_currSeg.f0.p 	 = p0;		// origin point
	rwi.m_currSeg.f0.IDf = 0;		// not-a-face

	rwi.m_currSeg.dToOrigin = d0;	// starts at ray origin

	// find the enclosing tetra
	TetraEnclosingPointByLinearSearch L;
	L.mesh(&M);
	L.point(::Point<3,double>(p0));
	L.update();

    //	if (!L.ok())
    //		cout << "Error locating tetra" << endl;

	TetraMesh::TetraDescriptor el = L.tetra();

	rwi.m_currSeg.IDt = el.value(); //M.findEnclosingTetra(p0);

	if (rwi.m_currSeg.IDt == 0)			// doesn't start in a tetra; search faces
	{
        cout << "Line 176 -- RayWalkIterator::init Unsupported external beam... Exiting!" << endl;
		exit(0);
        
        rwi.m_currSeg.matID=0;
		rwi.finishStepExterior();

		if (rwi.m_currSeg.f1.IDf==0)
			return RayWalkIterator();
	}
	else						// in a tetra, so search tetra's faces
	{
		rwi.m_currSeg.matID = get(region, M, TetraMesh::TetraDescriptor(rwi.m_currSeg.IDt)); // M.getMaterial(rwi.m_currSeg.IDt);
		rwi.finishStepInTetra();
	}


	return rwi;
}

RayWalkIterator RayWalkIterator::restartFrom(const RayWalkIterator& rwi)
{
	RayWalkIterator it=rwi;
	it.m_currSeg.dToOrigin=0.0f;
	return it;
}

RayWalkIterator RayWalkIterator::endAt(float d)
{
	RayWalkIterator it;
	it.m_currSeg.dToOrigin = d;
	return it;
}


/** Completes the current step within a tetra, starting at intersection defined by currSeg_.f0 in tetra currSeg_.IDt
* Requires f0, IDt, dir
* Updates f1, lSeg, and nextTet
*
*/
void RayWalkIterator::finishStepInTetra()
{
	__m128 p = _mm_setr_ps(m_currSeg.f0.p[0],m_currSeg.f0.p[1],m_currSeg.f0.p[2],0);
	__m128 d = _mm_setr_ps(m_dir[0],m_dir[1],m_dir[2],0);
	__m128 s = _mm_set1_ps(std::numeric_limits<float>::infinity());

    // Create a Tetra variable to be able tp call the getIntersection function
    /*
     * To do this, we get all the faces of the tetra, and then we get a nx, ny, nz and C of all the faces. 
     *
     */

    // get all the directed faces of the tetra
    std::array<TetraMesh::DirectedFaceDescriptor, 4> tetra_directed_faces = 
                            get(directed_faces, *m_mesh, TetraMesh::TetraDescriptor(m_currSeg.IDt));

    // get the components (normal vector and the offset) of the face f0 of the current segment
    float * nx_array = new float[4];
    float * ny_array = new float[4];
    float * nz_array = new float[4];
    float * C_array = new float[4];

    pair<UnitVector<3, double>, double> face_components;
    for (unsigned int f = 0; f < 4; f++)
    {
        face_components = get(face_plane, *m_mesh, tetra_directed_faces[f]);

        nx_array[f] = face_components.first[0];
        ny_array[f] = face_components.first[1];
        nz_array[f] = face_components.first[2];
        C_array[f] = face_components.second;
    }
    
    Tetra temp_t;
    
    // assign tetra components
    temp_t.nx = _mm_load_ps(nx_array);
    temp_t.ny = _mm_load_ps(ny_array);
    temp_t.nz = _mm_load_ps(nz_array);
    temp_t.C = _mm_load_ps(C_array);

    // assign IDs of tetra directed faces 
    temp_t.IDfds[0] = tetra_directed_faces[0].value();
    temp_t.IDfds[1] = tetra_directed_faces[1].value();
    temp_t.IDfds[2] = tetra_directed_faces[2].value();
    temp_t.IDfds[3] = tetra_directed_faces[3].value();
    
    // Assign IDs of adj tetra
    // For every face, check if the below tetra is the m_currSeg.IDt. If not, blow is adjacent. Otherwise, above is adjacent
    for (unsigned int f = 0; f < 4; f++)
    {
        auto adj_tet_IDt = get(tetra_below_face, *m_mesh, tetra_directed_faces[f]).value(); 
        if (m_currSeg.IDt == adj_tet_IDt)
            adj_tet_IDt = get(tetra_above_face, *m_mesh, tetra_directed_faces[f]).value();

        temp_t.adjTetras[f] = adj_tet_IDt;
    }
    

// 	StepResult sr = m_mesh->getTetra(m_currSeg.IDt).getIntersection(p,d,s);
	StepResult sr = temp_t.getIntersection(p,d,s);


#ifdef VERBOSE  
	////// DISPLAY ONLY
	cout << "RayWalkIterator::finishStepInTetra() pre-check: tetra " << m_currSeg.IDt << " heights: ";

	std::array<float,4> h;
	_mm_store_ps(h.data(), temp_t.heights(to_m128(m_currSeg.f0.p)));
	for(unsigned i=0;i<4;++i)
		cout << h[i] << ' ';
	cout << endl;

	cout << "    dots: ";
	std::array<float,4> dotf;
	_mm_store_ps(dotf.data(),temp_t.dots(to_m128(m_dir)));


	for(unsigned i=0;i<4;++i)
		cout << dotf[i] << ' ';
	cout << endl;


	__m128 h1 = _mm_load_ps(h.data());
	__m128 dot = _mm_load_ps(dotf.data());
	__m128 pcdist = _mm_div_ps(h1,dot);

	std::array<float,4> df;
	_mm_store_ps(df.data(),pcdist);

	cout << "    distances: ";
	for(unsigned i=0;i<4;++i)
		cout << df[i] << ' ';
	cout << endl;


//  selects dist where dist>0 and dot<0 (facing outwards), s otherwise
   // dist = _mm_blendv_ps(a,b,mask)
   //  dist_i = (mask_i & 0x80000000) ? b_i : a_i;
   __m128 dist = _mm_blendv_ps(
   			_mm_set1_ps(std::numeric_limits<float>::infinity()),
				pcdist,
				_mm_and_ps(
						h1,
						dot));

   std::array<float,4> distf;

   _mm_store_ps(distf.data(),dist);

	cout << "    distances post-clip: ";
	for(unsigned i=0;i<4;++i)
		cout << distf[i] << ' ';
	cout << endl;


	cout << "    tetras: ";
	for(unsigned i=0;i<4;++i)
		cout << temp_t.adjTetras[i] << ' ';
	cout << endl;
	////// END DISPLAY ONLY
#endif


	float t[4];
	_mm_store_ps(t,sr.Pe);

	m_currSeg.f1.p =  array<float,3>{{t[0],t[1],t[2]}};
	m_currSeg.f1.IDf= sr.IDfe;

	// there's always something to intersect when inside a tetra
	m_currSeg.lSeg = _mm_cvtss_f32(sr.distance);

#ifdef VERBOSE
	cout << "stepInTetra: distance " << m_currSeg.lSeg << " into " << sr.idx << "'th adjacent tet (" << sr.IDte << ")"<< endl;
#endif

	nextTet_ = sr.IDte; // the tetra hit by the ray

}



/** Completes the current step outside of a tetra, either ending in a tetra or no intersection (f1.ID==0)
* Requires f0, IDt, dir
* Updates f1, lSeg, nextTet_
*/

void RayWalkIterator::finishStepExterior()
{
	int IDf;
	PointIntersectionResult res;

	// find the next face along the ray, excluding the current face from consideration (may see hit due to tolerance errors)


    cout << "RayWalkIterator::finishStepExterior Unsupported external beam" << endl;
    //exit(0);
    m_currSeg.f1.IDf = 0;
    return;
    // Disabled: TODO: Need a new implementation for "findNextFaceAlongRay"
	//std::tie(res,IDf) = m_mesh->findNextFaceAlongRay(m_currSeg.f0.p, m_dir, m_currSeg.f0.IDf); // old command
    

	boost::copy(res.q,m_currSeg.f1.p.begin());
	m_currSeg.lSeg = res.t;

	unsigned IDt;

	// check if + orientation of this face points into the current tetra. if so, take other tetra and return -face.
    //  + orientation means above the tetra
    IDt = get(tetra_above_face, *m_mesh, TetraMesh::DirectedFaceDescriptor(IDf)).value();


	if (IDt == m_currSeg.IDt)
	{
		IDf=-IDf;
        IDt = get(tetra_above_face, *m_mesh, TetraMesh::FaceDescriptor(-IDf)).value();
	}

	m_currSeg.f1.IDf = IDf;
	nextTet_ = IDt;
}
