/*
 * Partition.hpp
 *
 *  Created on: Apr 13, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_PARTITION_HPP_
#define GEOMETRY_PARTITION_HPP_

#include "WrappedVector.hpp"

/** A Partition divides a set of elements into disjoint subsets
 *
 * Can be used to represent a predicate using false(0) and true (1)
 */

class Partition : public WrappedVector<unsigned,unsigned>
{
public:
	Partition();
	~Partition();

	void assign(unsigned el,unsigned region);	///< Requires el < N, but region may be any value and count will be updated

	unsigned count() const;						///< Number of subsets

	void recountRegions();						///< Update the region count (automatically grows as elements are added to subset,
												///< but does not automatically shrink as elements are assigned out of a set)

private:
	using WrappedVector<unsigned,unsigned>::set;	///< Hide the set method; should use assign(i,region) instead

	unsigned				m_regionCount=0;
};

#endif /* GEOMETRY_PARTITION_HPP_ */
