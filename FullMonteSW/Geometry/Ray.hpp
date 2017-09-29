/*
 * Ray.hpp
 *
 *  Created on: Mar 9, 2016
 *      Author: jcassidy
 */

#ifndef GEOMETRY_RAY_HPP_
#define GEOMETRY_RAY_HPP_

#include "Point.hpp"
#include "UnitVector.hpp"

// A Ray represents the geometric idea of a ray, a semi-infinite line starting from a point and extending along a unit vector
template<size_t D,class T>class Ray
{
	Point<D,T>      P;
	UnitVector<D,T> d;

	public:
	// construct from an origin point and a direction unit vector
    Ray(){};
	Ray(const Point<D,T>& P_,const UnitVector<D,T>& d_) : P(P_),d(d_){};

	// returns the point that is T units along the ray
	Point<D,T> operator()(T t) const { return Point<D,T>(P+d*t); };

	const Point<D,T>&      getOrigin()    const { return P; }
	const UnitVector<D,T>& getDirection() const { return d; }

    void setOrigin(Point<3,double> p_){ P=p_; }
};


struct RTIntersection
{
	std::array<float,3>		q;
	float					d;
	int						IDf;
	unsigned				IDt_from;
	unsigned				IDt_to;
};

//
//template<class Kernel>class Ray
//{
//public:
//	Ray(Kernel::Point p0,Kernel::UnitVector direction);
//
//	Kernel::Point		origin()						const 	{ return m_origin;		}
//	void				origin(Kernel::Point p)					{ m_origin=p;			}
//
//	void 				direction(Kernel::UnitVector d)			{ m_direction=d;		}
//	Kernel::UnitVector 	direction()						const	{ return m_direction;	}
//
//	Kernel::Point		operator()(Kernel::Scalar x)	const
//	{
//		return m_origin + x*m_direction;
//	}
//
//private:
//	Kernel::Point		m_origin;
//	Kernel::UnitVector	m_direction;
//};
//
//template<class Kernel>Ray<Kernel>::Ray(Kernel::Point p0,Kernel::UnitVector direction)
//	m_origin(p0),
//	m_direction(direction){}
//{
//}


struct PointIntersectionResult
{
	bool intersects=false;
	Point<3,double> q = Point<3,double>::nan();
	double t=std::numeric_limits<double>::quiet_NaN();
};

// returns true if P+td falls within triangle defined by points T for some non-negative t
PointIntersectionResult RayTriangleIntersection(Point<3,double> p,UnitVector<3,double> d,Point<3,double> T[3]);


/** Determines if ray defined by (P,d) intersects the triangle defined by three points T[]
 * If so, returns the point of intersection q and the distance along the ray, t
 *
 * Let AB = e1 = T[1]-T[0], AC = e2=T[2]-T[0]
 *
 * Solves P + td = A + u*AB + v*AC for (t,u,v) and checks if 0 <= u,v,u+v <= 1 and t>=0
 *
 */

inline PointIntersectionResult RayTriangleIntersection(Point<3,double> P,UnitVector<3,double> d,Point<3,double> T[3])
{
    double a[3] = { T[1][0]-T[0][0], T[1][1]-T[0][1], T[1][2]-T[0][2] };    // vector from point T[0] to T[1]
    double b[3] = { T[2][0]-T[0][0], T[2][1]-T[0][1], T[2][2]-T[0][2] };    // vector from T[0] to T[2]
    double M[3][3] = { { a[0],b[0],-d[0] }, {a[1],b[1],-d[1]}, {a[2],b[2],-d[2]} };

    // make determinant
    double det = M[0][0]*(M[2][2]*M[1][1]-M[2][1]*M[1][2])
        -M[1][0]*(M[2][2]*M[0][1]-M[2][1]*M[0][2])
        +M[2][0]*(M[1][2]*M[0][1]-M[1][1]*M[0][2]);

    // make inverse (needs to multiplied by 1/det)
    double inv[3][3] = { { M[2][2]*M[1][1]-M[2][1]*M[1][2], -(M[2][2]*M[0][1]-M[2][1]*M[0][2]), M[1][2]*M[0][1]-M[1][1]*M[0][2] },
        { -(M[2][2]*M[1][0]-M[2][0]*M[1][2]), M[2][2]*M[0][0]-M[2][0]*M[0][2], -(M[1][2]*M[0][0]-M[1][0]*M[0][2]) },
        {   M[2][1]*M[1][0]-M[2][0]*M[1][1],-(M[2][1]*M[0][0]-M[2][0]*M[0][1]),  M[1][1]*M[0][0]-M[1][0]*M[0][1] } };

    double A[3] = { P[0]-T[0][0], P[1]-T[0][1],P[2]-T[0][2] };

    double c[3] = { (inv[0][0]*A[0]+inv[0][1]*A[1]+inv[0][2]*A[2])/det,
        (inv[1][0]*A[0]+inv[1][1]*A[1]+inv[1][2]*A[2])/det,
        (inv[2][0]*A[0]+inv[2][1]*A[1]+inv[2][2]*A[2])/det };

    // u=c[0], v=c[1], t=c[2]

    PointIntersectionResult res;

    res.intersects = c[2] >= 0 && (c[0] <= 1.0) & (c[0] >= 0.0) & (c[1] <= 1.0) & (c[1] >= 0.0) && (c[0]+c[1] <= 1.0);

    if(res.intersects)
    {
        res.t=c[2];
        res.q=Point<3,double>{
        	(1-c[0]-c[1])*T[0][0] + c[0]*T[1][0] + c[1]*T[2][0],
            (1-c[0]-c[1])*T[0][1] + c[0]*T[1][1] + c[1]*T[2][1],
            (1-c[0]-c[1])*T[0][2] + c[0]*T[1][2] + c[1]*T[2][2] };
    }

    return res;
}




#endif /* GEOMETRY_RAY_HPP_ */
