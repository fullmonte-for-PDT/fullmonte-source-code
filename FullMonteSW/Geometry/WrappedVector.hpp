/*
 * WrappedVector.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_WRAPPEDVECTOR_HPP_
#define GEOMETRY_WRAPPEDVECTOR_HPP_

#include <vector>

/** Very simple wrapper for std::vector
 *
 * Useful because it renames the operators for script wrapping.
 * NOTE: Returns by value and does not have its own set(i,value) method in case a derived class wants to hook it.
 *
 */

template<class T,typename Index>class WrappedVector
{
public:
	WrappedVector(){};
	~WrappedVector(){};

	void 		resize(Index N,T value=T())
		{ m_vec.resize(N,value); 	}

	Index 		size() 			const
		{ return m_vec.size(); 		}

	T get(Index i) const
		{ return m_vec.at(i); 		}

	void set(Index i,T value)
		{ m_vec.at(i) = value; 		}

private:
	std::vector<T>		m_vec;
};



#endif /* GEOMETRY_WRAPPEDVECTOR_HPP_ */
