/*
 * AbsorptionSum.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: jcassidy
 */

#include "AbsorptionSum.hpp"

#include <boost/range/counting_range.hpp>
#include <boost/range/adaptor/filtered.hpp>

#include <iomanip>

AbsorptionSum::AbsorptionSum()
{

}

AbsorptionSum::~AbsorptionSum()
{

}

void AbsorptionSum::partition(Partition* p)
{
	m_partition=p;
}

void AbsorptionSum::update()
{
	if (!m_partition)
	{
		cout << "AbsorptionSum::update() called with null partition" << endl;
		return;
	}

	if (!m_E)
	{
		cout << "AbsorptionSum::update() called with null data" << endl;
		return;
	}


	m_regionSum.resize(m_partition->count());
	boost::fill(m_regionSum, 0.0f);

	for(unsigned i=0;i<(*m_E)->dim(); ++i)
		sum.at(m_partition->get(i)) += (*m_E)[i];
}

float AbsorptionSum::total() const
{
	float total=0.0f;
	for(const auto s : regionSum)
		total += s;
	return total;
}

VolumeAbsorbedEnergyMap* AbsorptionSum::result() const
{
	return new VolumeAbsorbedEnergyMap(m_regionSum);
}
