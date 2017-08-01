/*
 * AbsorptionSum.hpp
 *
 *  Created on: Mar 24, 2016
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_ABSORPTIONSUM_HPP_
#define OUTPUTTYPES_ABSORPTIONSUM_HPP_

#include <FullMonteSW/Geometry/Partition.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap.hpp>


/** Sums absorbed energy over a given Partition
 *
 */

class AbsorptionSum
{
public:
	AbsorptionSum();

	/// Set the partition to be used
	void							partition(const Partition* p);

	/// Get/set the data to be summed
	void 							data(const VolumeAbsorbedEnergyMap* E)			{ m_E=E;		}
	const VolumeAbsorbedEnergyMap*	data() 									const 	{ return m_E; 	}

	/// Compute
	void update();

	/// Get the result
	VolumeAbsorbedEnergyMap* 		result() const;

private:
	const Partition*				m_partition=nullptr;
	const VolumeAbsorbedEnergyMap* 	m_E=nullptr;

	std::vector<float>				m_regionSum;
};



#endif /* OUTPUTTYPES_ABSORPTIONSUM_HPP_ */
