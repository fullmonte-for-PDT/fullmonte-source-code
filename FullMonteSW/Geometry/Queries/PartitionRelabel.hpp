/*
 * PartitionRemap.hpp
 *
 *  Created on: Jun 6, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_QUERIES_PARTITIONRELABEL_HPP_
#define GEOMETRY_QUERIES_PARTITIONRELABEL_HPP_

class Partition;
#include <vector>

/** Relabels partition members based solely on the input membership.
 */

class PartitionRelabel
{
public:
	PartitionRelabel();
	~PartitionRelabel();

	void			passthroughAll();						///< Passes everything through as-is
	void			relabelAllTo(unsigned i);				///< Relabels everything to the specified label

	void			relabel(unsigned from,unsigned to);		///< Relabels the specified 'from' region to the new 'to' region

	void			partition(Partition* i);				///< Change the input partition
	void			update();								///< Calculate new output

	Partition*		result() const;

private:
	unsigned		newRegionFor(unsigned r) const;

	Partition*					m_partition=nullptr;

	std::vector<unsigned>		m_newRegion;				///< New region map. -1U or index out of bounds -> use default
	unsigned					m_defaultRegion=-1U;		///< value to assign by default. If -1U, then leave input unchanges
};

#endif /* GEOMETRY_QUERIES_PARTITIONRELABEL_HPP_ */
