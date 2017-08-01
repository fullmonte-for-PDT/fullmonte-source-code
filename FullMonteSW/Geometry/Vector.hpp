/*
 * Vector.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_VECTOR_HPP_
#define GEOMETRY_VECTOR_HPP_

#include "Point.hpp"

#include <cmath>

// A Vector extends the Point class with a norm, dot product, cross product, add/sub and scalar multiply/divide
//    vector can be defined as going between two points, or implicitly as the origin (0,0,0) to a point
template<size_t D,class T>class Vector : public Point<D,T>
{
public:
	using Point<D,T>::operator=;
	using Point<D,T>::operator[];
	using Point<D,T>::operator+;

	Vector()                                        : Point<D,T>()  {}
	Vector(const std::array<T,D>& a)				: Point<D,T>(a)	{}
	Vector(const Point<D,T>& P)                     : Point<D,T>(P) {};
	Vector(const Point<D,T>& A,const Point<D,T>& B){
		for(size_t i=0;i<D;++i)
			(*this)[i]=B[i]-A[i];
	}

	// norms and dots
	T norm_l2()                 const { T s=0; for(size_t i=0;i<D;++i){ s += (*this)[i] * (*this)[i]; } return(std::sqrt(s)); }
	T norm_l1()                 const { T s=0; for(size_t i=0;i<D;++i){ s += abs((*this)[i]); } return abs(s);   }
	T norm2_l2()                const { T s=0; for(size_t i=0;i<D;++i){ s += (*this)[i]*(*this)[i]; } return s;        }
	T dot(const Vector<D,T>& a) const { T s=0; for(size_t i=0;i<D;++i){ s += a[i]*(*this)[i]; } return s;        }

	// unary negate
	Vector operator-()           		{ Vector v; for(size_t i=0; i<D; ++i){ v[i]=-(*this)[i]; } return v; }


	// vector += / -= operations
	const Vector& operator+=(const Vector& k) { for(int i=0; i<D; ++i){ (*this)[i]+=k[i]; } return *this; }
	const Vector& operator-=(const Vector& k) { for(int i=0; i<D; ++i){ (*this)[i]-=k[i]; } return *this; }

	// scalar operations
	const Vector& operator*=(const T& k) { for(size_t i=0; i<D; ++i){ (*this)[i]*=k; } return *this; }
	const Vector& operator/=(const T& k) { for(size_t i=0; i<D; ++i){ (*this)[i]/=k; } return *this; }
	Vector operator* (T k)        const { Vector<D,T> t(*this); return t *= k; }
	Vector operator/ (const T& k) const { Vector<D,T> t(*this); return t /= k; }

	// vector cross product
	Vector cross(const Vector<D,T>&) const;
	template<size_t D_,class U>friend Vector<D_,U> cross(const Vector<D_,U>&,const Vector<D_,U>&);
};

template<size_t D,class T>Vector<D,T> operator-(const Point<D,T> lhs, const Point<D,T> rhs)
{
	Vector<D,T> v;
	for(size_t i=0;i<D; ++i)
		v[i]=lhs[i]-rhs[i];
	return v;
}

// helpers
template<size_t D,class T>T dot(const Vector<D,T>& a,const Vector<D,T>& b) { return a.dot(b); }
template<size_t D,class T>T norm_l2(const Vector<D,T>& a) { return a.norm_l2(); }
template<size_t D,class T>T norm_l1(const Vector<D,T>& a) { return a.norm_l1(); }
template<size_t D,class T>T norm2_l2(const Vector<D,T>& a){ return a.norm2_l2(); }

template<size_t D,class T>T norm2_l2(const Point<D,T>& a,const Point<D,T>& b){ T s=0; for(unsigned i=0;i<D;++i){ s += (a[i]-b[i])*(a[i]-b[i]); } return s; }




/** cross(O,A,B) computes the cross product of A and B around origin O
 * ie. OA x OB
 *
 * Useful when working with a triangle that is not located at the origin
 */

template<class T> Vector<3,T> cross(const Point<3,T> O,const Point<3,T> A,const Point<3,T> B)
{
	return cross(A-O,B-O);
}





/** cross(A,B) computes A x B
 *
 */

template<class T> Vector<3,T> cross(const Vector<3,T>& a,const Vector<3,T>& b)
{
	return a.cross(b);
}

template<size_t D,class T>Vector<D,T> Vector<D,T>::cross(const Vector<D,T>& x) const
{
	std::array<T,D> cp{{ (*this)[1]*x[2]-(*this)[2]*x[1], (*this)[2]*x[0]-x[2]*(*this)[0], (*this)[0]*x[1]-(*this)[1]*x[0] }};
	return Vector<D,T>(cp);
}

// Computes the scalar triple product for vectors a,b,c = a dot (b cross c)
template<class T>T scalartriple(const Vector<3,T>& a,const Vector<3,T>& b,const Vector<3,T>& c)
{
    return dot(a,cross(b,c));
}


#endif /* GEOMETRY_VECTOR_HPP_ */
