/*
 * SerialPointTetraLocator.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_QUERIES_SERIALPOINTTETRALOCATOR_HPP_
#define GEOMETRY_QUERIES_SERIALPOINTTETRALOCATOR_HPP_

/** Locates the tetra containing a given point by traversing all tetras serially and evaluating the
 */


/** Finds the surface face closest to the ray (p,d)
 * If predicate F is provided, only faces matching the predicate F are considered
 *
 * Returns the intersection point, distance to intersection, the tetra just after the interface,
 * and the face oriented to point along the ray
 */


RTIntersection TetraMesh::findSurfaceFace(array<float,3> p,array<float,3> d,const FilterBase<int>* F) const
{
	float dMin = numeric_limits<float>::infinity();
	int fMin = 0;

	if (!F)
		throw std::logic_error("TetraMesh::findSurfaceFace(p,d,F) given null filter (not supported)");

	for(unsigned i=1;i<m_faces.size(); ++i)
	{
		int IDf;

		if ((*F)(i))
			IDf = i;
		else if ((*F)(-i))
			IDf = -i;
		else
			continue;

		Face f = m_faces[i];

		const auto nd = f.getNormal();
		array<float,3> n{{float(nd[0]),float(nd[1]),float(nd[2])}};
		float c = f.getConstant();

		Point<3,double> T[3] = {
				Point<3,double>(m_points[m_facePoints[i][0]]),
				Point<3,double>(m_points[m_facePoints[i][1]]),
				Point<3,double>(m_points[m_facePoints[i][2]])
		};

		unsigned IDt_from 	= m_faceTetras[i][0];
		unsigned IDt_to 	= m_faceTetras[i][1];

		if (IDf < 0)
		{
			c=-c;
			n=-n;

			std::swap(IDt_from,IDt_to);
		}
		else
			std::swap(T[1],T[2]);

		float costheta = dot(d,n);

		float h = dot(p,n) - c;
		float dist = std::abs(h/costheta);

		//cout << "    Face " << IDf << " distance " << dist << " from tet " << IDt_from << " (mat " << m_tetraMaterials[IDt_from] << ") into tet " << IDt_to << " (material " << m_tetraMaterials[IDt_to] << ")" << endl;

		PointIntersectionResult res = RayTriangleIntersection(Point<3,double>(p), UnitVector<3,double>(d), T);
//		if (res.intersects)
//			cout << "      has intersection with ray, distance " << dist << endl;

		if (h >= 0 && dist >= 0 && dist < dMin && res.intersects)
		{
			dMin = dist;
			fMin = IDf;
		}
	}

	//unsigned IDt_from = fMin < 0 ? m_faceTetras[-fMin][1] : m_faceTetras[fMin][0]			// return the tetra from which ray enters

	unsigned IDt_from = m_faceTetras[std::abs(fMin)][fMin < 0];
	unsigned IDt_to   = m_faceTetras[std::abs(fMin)][fMin > 0];

	return RTIntersection {
		p + dMin*d,
		dMin,
		fMin,
		IDt_from,
		IDt_to
	};
}



// check linear combination of points to verify we're within tetra
//   used only for testing, very slow
bool TetraMesh::isWithinByPoints(int tID,const Point<3,double>& p) const
{
    float M[3][4];
    const Point<3,double> &A=m_points[m_tetraPoints[tID][0]], &B=m_points[m_tetraPoints[tID][1]], &C=m_points[m_tetraPoints[tID][2]], &D=m_points[m_tetraPoints[tID][3]];
    Vector<3,double> e[3];

    // calculate edge vectors
    e[0]=(Vector<3,double>)(B)-(Vector<3,double>)(A);
    e[1]=(Vector<3,double>)(C)-(Vector<3,double>)(A);
    e[2]=(Vector<3,double>)(D)-(Vector<3,double>)(A);

    // build basis matrix for tetra
    M[0][0] = e[0][0];
    M[1][0] = e[0][1];
    M[2][0] = e[0][2];
    M[0][3] = p[0]-A[0];

    M[0][1] = e[1][0];
    M[1][1] = e[1][1];
    M[2][1] = e[1][2];
    M[1][3] = p[1]-A[1];

    M[0][2] = e[2][0];
    M[1][2] = e[2][1];
    M[2][2] = e[2][2];
    M[2][3] = p[2]-A[2];

    double c;

    // eliminate
    for(unsigned i=0;i<3;++i)
    {
        // subtract rows above
        for(unsigned j=0;j<i;++j)
        {
            c=M[i][j];
            for(unsigned k=0;k<4;++k)
                M[i][k] -= c*M[j][k];
        }

        // normalize the row
        c=M[i][i];
        for(unsigned j=i;j<4;++j)
            M[i][j] /= c;
    }

    // backsub
    for(int i=1;i>=0;--i)
        for(unsigned j=i+1;j<3;++j)
        {
            c=M[i][j];
            for(unsigned k=0;k<4;++k)
                M[i][k] -= c*M[j][k];
        }

    float coeff[4];
    coeff[0]=1.0;

    for(unsigned i=1;i<4;++i)
    {
        coeff[i]=M[i-1][3];
        coeff[0] -= coeff[i];
    }

    printf("Coeffs are: %9.5f %9.5f %9.5f %9.5f ",coeff[0],coeff[1],coeff[2],coeff[3]);

    bool within=true,onedge=false;

    for(unsigned i=0;i<4;++i)
    {
        within &= (coeff[i] > -1e-4) & (coeff[i] < 1+1e-4);
        onedge |= (abs(coeff[i]) < 1e-4);
    }

    cout << " - " << (within ? (onedge ? "on edge" : "within") : "OUTSIDE") << endl;

    return within;
}


/** Finds the next face along the given ray
 *
 * An optional parameter IDf_exclude allows exclusion of one face (eg. if the ray starts at such face)
 *
 * TODO: Should include a tolerance on the >= 0 calc, in case the point is ever-so-slightly on the wrong side of the face,
 * 			eg. when trying to find the bounding face
 */

std::tuple<PointIntersectionResult,int> TetraMesh::findNextFaceAlongRay(Point<3,double> p,UnitVector<3,double> dir,int IDf_exclude) const
{
	unsigned IDf=0;
	double dMin = std::numeric_limits<double>::infinity();
	PointIntersectionResult best;

	for(unsigned i=0;i<m_faces.size();++i)
	{
		if (i == abs(IDf_exclude))
			continue;

		FaceByPointID IDps = m_facePoints[i];
		Point<3,double> T[3]{
			m_points[IDps[0]],
			m_points[IDps[1]],
			m_points[IDps[2]]
		};

		PointIntersectionResult res = RayTriangleIntersection(p,dir,T);

		if (res.intersects && res.t < dMin)
		{
			best = res;
			dMin = res.t;
			IDf = i;
		}
	}

	return make_tuple(best,IDf);
}
//
//unsigned TetraMesh::getTetraFromFace(int IDf) const
//{
//    return IDf > 0 ? m_faceTetras[IDf][0] : m_faceTetras[-IDf][1];
//}

double TetraMesh::getFaceArea(const FaceByPointID& f) const
{
    Vector<3,double> AB(m_points[f[0]],m_points[f[1]]);
    Vector<3,double> AC(m_points[f[0]],m_points[f[2]]);

    return cross(AB,AC).norm_l2()/2;
}

/** returns a vector containing (face ID, tetra ID) for each face on the boundary of r0; if r1 is specified, then
 * it also requires that the face bound r1 (ie. is an r0-r1 boundary)
 *
 * The tetra ID returned is the tetra in region r0.
 */

// TODO: return signed int indicating face orientation?

vector<pair<unsigned,unsigned>> TetraMesh::getRegionBoundaryTrisAndTetras(unsigned r0,unsigned r1) const
{
	vector<pair<unsigned,unsigned>> v;

	for(unsigned i=0;i<m_faceTetras.size();++i)
	{
		unsigned Ta = m_faceTetras[i][0], Tb = m_faceTetras[i][1];
		assert(Ta < m_tetraMaterials.size() && Tb < m_tetraMaterials.size());

		unsigned ma = m_tetraMaterials[Ta], mb = m_tetraMaterials[Tb];

		if (ma == mb)														// no boundary
			continue;
		else if (ma == r0 && (r1==-1U || mb==r1))							// boundary; Ta is within region
			v.push_back(make_pair(i,Ta));
		else if (mb == r0 && (r1 ==-1U || ma == r1))						// boundary; Tb is within region
			v.push_back(make_pair(i,Tb));
	}

	return v;
}


#endif /* GEOMETRY_QUERIES_SERIALPOINTTETRALOCATOR_HPP_ */
