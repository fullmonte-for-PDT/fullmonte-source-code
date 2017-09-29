/*
 * PartitionRelabel.cpp
 *
 *  Created on: Jun 6, 2017
 *      Author: jcassidy
 */

#include "PartitionRelabel.hpp"
#include "../Partition.hpp"

PartitionRelabel::PartitionRelabel()
{
}

PartitionRelabel::~PartitionRelabel()
{
}

void PartitionRelabel::passthroughAll()
{
	m_newRegion.clear();
	m_defaultRegion = -1U;
}

void PartitionRelabel::relabelAllTo(unsigned i)
{
	m_newRegion.clear();
	m_defaultRegion = i;
}

void PartitionRelabel::relabel(unsigned from,unsigned to)
{
	if (from >= m_newRegion.size())
		m_newRegion.resize(from+1,-1U);

	m_newRegion[from] = to;
}

unsigned PartitionRelabel::newRegionFor(unsigned r) const
{
	if (r < m_newRegion.size() && m_newRegion[r] != -1U)
		return m_newRegion[r];
	else if (m_defaultRegion == -1U)
		return r;
	else
		return m_defaultRegion;
}

void PartitionRelabel::update()
{
}

Partition* PartitionRelabel::result() const
{
	Partition* P = new Partition();
	P->resize(m_partition->size());

	for(unsigned i=0;i<P->size();++i)
		P->assign(i,newRegionFor(m_partition->get(i)));

	P->recountRegions();

	return P;
}
