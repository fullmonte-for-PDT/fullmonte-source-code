/*
 * Point.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_POINT_HPP_
#define GEOMETRY_POINT_HPP_

#include <array>
#include <boost/range/algorithm.hpp>

template<size_t D,class T>class Vector;

template<size_t D,class T>class Point : public std::array<T,D>
{
public:
	using std::array<T,D>::operator=;

	Point()                { std::array<T,D>::fill(T()); };
	Point(T x,T y,T z)		: std::array<T,D>{{x,y,z}} 	{}
	Point(const Point& P_) : std::array<T,D>(P_){};
	Point(std::array<float,D> a){ boost::copy(a,std::array<T,D>::begin()); }

	static Point nan(){ Point P; boost::fill(P,std::numeric_limits<T>::quiet_NaN()); return P; }

	template<typename U>explicit Point(const std::array<U,D> a){ boost::copy(a,std::array<T,D>::begin()); }
	template<typename U>explicit Point(const U* p){ std::copy(p,p+D,std::array<T,D>::begin()); }

	Point operator+(const Vector<D,T>& v) const { Point t; for(unsigned i=0;i<D;++i){ t[i]=(*this)[i]+v[i]; } return t; }


	template<typename FT=float>std::array<FT,D> as_array_of() const
		{
			std::array<FT,D> o;
			std::copy(std::array<T,D>::begin(), std::array<T,D>::end(), o.begin());
			return o;
		}
};

// Computes the scalar triple product for points A,B,C,D = DA dot (DB cross DC)
template<class T>T scalartriple(const Point<3,T>& A,const Point<3,T>& B,const Point<3,T>& C,const Point<3,T>& D)
{
    return scalartriple(Vector<3,T>(D,A),Vector<3,T>(D,B),Vector<3,T>(D,C));
}




#endif /* GEOMETRY_POINT_HPP_ */
