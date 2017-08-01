/*
 * SpatialMap2D2D.hpp
 *
 *  Created on: Oct 30, 2016
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_SPATIALMAP2D_HPP_
#define OUTPUTTYPES_SPATIALMAP2D_HPP_

#include "SpatialMap.hpp"

#include <array>
#include <boost/range.hpp>
#include <boost/range/adaptor/indexed.hpp>


#include "OutputDataType.hpp"

/** Represents a spatial map of some property over elements in a geometry
 *
 * Adds 2D indexing using operator()(i,j) to SpatialMap<Value>, with values stored in row-major order
 * (ie. M(0,0) M(0,1) .. M(0,N-1) M(1,0) .. )
 *
 */

template<typename Value>class SpatialMap2D : public SpatialMap<Value>
{
public:
	explicit SpatialMap2D(std::size_t w,std::size_t h,AbstractSpatialMap::SpaceType stype=AbstractSpatialMap::UnknownSpaceType,AbstractSpatialMap::ValueType vtype=AbstractSpatialMap::Scalar);

	SpatialMap2D(std::size_t w,std::size_t h,const std::vector<Value>&,AbstractSpatialMap::SpaceType stype=AbstractSpatialMap::UnknownSpaceType,AbstractSpatialMap::ValueType vtype=AbstractSpatialMap::Scalar);
	SpatialMap2D(std::size_t w,std::size_t h,std::vector<Value>&&,AbstractSpatialMap::SpaceType stype=AbstractSpatialMap::UnknownSpaceType,AbstractSpatialMap::ValueType vtype=AbstractSpatialMap::Scalar);

	virtual ~SpatialMap2D();

//#ifndef SWIG
//	CLONE_METHOD(OutputData,SpatialMap2D)
//	ACCEPT_VISITOR_METHOD(OutputData,SpatialMap2D)
//#endif

	void 				dims(std::size_t w,std::size_t h);
	array<std::size_t,2>	dims() const;

	const Value& 		operator()(std::size_t i,std::size_t j) const;
	Value& 				operator()(std::size_t i,std::size_t j);

	Value				get(std::size_t i,std::size_t j) const;
	void				set(std::size_t i,std::size_t j,const Value& v);

	virtual const OutputDataType* type() const;

private:
	static const OutputDataType s_type;
	std::array<std::size_t,2>		m_dims;
};

template<>const OutputDataType SpatialMap2D<float>::s_type{"SpatialMap2D<float>"};

template<class Value>const OutputDataType* SpatialMap2D<Value>::type() const
{
	return &s_type;
}

template<typename Value>SpatialMap2D<Value>::SpatialMap2D
	(std::size_t x,std::size_t y,std::vector<Value>&& src,AbstractSpatialMap::SpaceType stype,AbstractSpatialMap::ValueType vtype) :
		SpatialMap<float>(std::move(src),stype,vtype),
		m_dims{{x,y}}
{
	if (x*y != this->dim())
		throw std::logic_error("SpatialMap2D<Value>::SpatialMap2D dimension mismatch");
}

template<typename Value>SpatialMap2D<Value>::SpatialMap2D
	(std::size_t x,std::size_t y,const std::vector<Value>& src,AbstractSpatialMap::SpaceType stype,AbstractSpatialMap::ValueType vtype) :
		SpatialMap<float>(src,stype,vtype),
		m_dims{{x,y}}
{
	if (x*y != this->dim())
		throw std::logic_error("SpatialMap2D<Value>::SpatialMap2D dimension mismatch");
}

template<typename Value>SpatialMap2D<Value>::SpatialMap2D
	(std::size_t x,std::size_t y,AbstractSpatialMap::SpaceType stype,AbstractSpatialMap::ValueType vtype) :
		SpatialMap<float>(x*y,stype,vtype),
		m_dims{{x,y}}
{
}

template<typename Value>array<std::size_t,2> SpatialMap2D<Value>::dims() const
{
	return m_dims;
}

template<typename Value>SpatialMap2D<Value>::~SpatialMap2D()
{
}

template<typename Value>void SpatialMap2D<Value>::dims(std::size_t w,std::size_t h)
{
	m_dims = array<std::size_t,2>{{w,h}};
	SpatialMap<Value>::dim(w*h);
}

template<typename Value>Value& SpatialMap2D<Value>::operator()(std::size_t i,std::size_t j)
{
	return (*this)[i*m_dims[1]+j];
}

template<typename Value>const Value& SpatialMap2D<Value>::operator()(std::size_t i,std::size_t j) const
{
	return (*this)[i*m_dims[1]+j];
}

template<typename Value>void SpatialMap2D<Value>::set(std::size_t i,std::size_t j,const Value& v)
{
	operator()(i,j) = v;
}

template<typename Value>Value SpatialMap2D<Value>::get(std::size_t i,std::size_t j) const
{
	return operator()(i,j);
}

#endif /* OUTPUTTYPES_SpatialMap2D2D_HPP_ */
