/*
 * PartitionFacesOnRegionBounds.hpp
 *
 *  Created on: Jun 6, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_QUERIES_PARTITIONFACESONREGIONBOUNDS_HPP_
#define GEOMETRY_QUERIES_PARTITIONFACESONREGIONBOUNDS_HPP_

#include <vector>

class TetraMesh;
class Partition;

class PartitionFacesOnRegionBounds
{
public:
	PartitionFacesOnRegionBounds();
	~PartitionFacesOnRegionBounds();

	void			regions(Partition* p);
	void			mesh(TetraMesh* m);

	void 			update();

	Partition*		result() const;


private:
	Partition*				m_partition=nullptr;
	TetraMesh*				m_mesh=nullptr;

	std::vector<unsigned>	m_result;
};



#endif /* GEOMETRY_QUERIES_PARTITIONFACESONREGIONBOUNDS_HPP_ */
