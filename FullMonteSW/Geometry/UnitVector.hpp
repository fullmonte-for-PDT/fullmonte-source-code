/*
 * UnitVector.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_UNITVECTOR_HPP_
#define GEOMETRY_UNITVECTOR_HPP_

#include "Vector.hpp"

// A UnitVector is a Vector that is guaranteed to always have L2 norm 1
template<size_t D,class T>class UnitVector : public Vector<D,T>
{
	public:
	using Vector<D,T>::dot;
	using Vector<D,T>::norm_l2;
	using Vector<D,T>::norm2_l2;
    using Vector<D,T>::operator-;
	using Vector<D,T>::operator*;
	using Vector<D,T>::operator/;

	explicit UnitVector(Vector<D,T> v) : Vector<D,T>(v){}

	UnitVector()                    { (*this)[0]=1;          for(unsigned i=1;i<D;++i){ (*this)[i]=0;      } };
	UnitVector(std::array<float,D> a){ boost::copy(a,std::array<T,D>::begin()); }
    UnitVector(const T* v_,bool alreadyUnit=false){
        for (unsigned i=0;i<D;++i)
        	(*this)[i]=v_[i];
        if (!alreadyUnit)
        {
            T L = norm_l2();
            for (unsigned i=0;i<D;++i)
            	(*this)[i] /= L;
        }
    }
    UnitVector(const UnitVector<D,T>& v){
        for(unsigned i=0;i<D;++i)
        	(*this)[i]=v[i];
    };
    UnitVector(const std::initializer_list<T>& il){ copy(il.begin(),il.end(),Vector<D,T>::begin()); }

    UnitVector operator-() const { UnitVector t(*this); for(unsigned i=0;i<D;++i) t[i] = -t[i]; return t; }
};

template<class T,std::size_t D>UnitVector<D,T> normalize(Vector<D,T> v)
{
	return UnitVector<D,T>(v * (T(1)/norm_l2(v)));
}




#endif /* GEOMETRY_UNITVECTOR_HPP_ */
