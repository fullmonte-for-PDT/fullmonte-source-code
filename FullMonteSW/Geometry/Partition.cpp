/*
 * Partition.cpp
 *
 *  Created on: May 20, 2017
 *      Author: jcassidy
 */

#include "Partition.hpp"
using namespace std;



Partition::Partition()
{
}

Partition::~Partition()
{
}

void Partition::assign(unsigned el,unsigned region)
{
	set(el,region);
	if (region >= m_regionCount)
		m_regionCount = region+1;
}

unsigned Partition::count() const
{
	return m_regionCount;
}

void Partition::recountRegions()
{
	m_regionCount=0U;

	// find the max region code and add one (regions 0..N-1 -> N regions)
	for(unsigned i=0;i<size();++i)
		m_regionCount = max(m_regionCount,get(i));
	++m_regionCount;
}
