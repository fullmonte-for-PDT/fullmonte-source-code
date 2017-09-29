/*
 * MaterialSet.cpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#include "MaterialSet.hpp"

MaterialSet::MaterialSet()
{

}

MaterialSet::~MaterialSet()
{
}

bool MaterialSet::matchedBoundary() const
{
	return m_matched;
}

void MaterialSet::matchedBoundary(bool m)
{
	m_matched=m;
}

unsigned MaterialSet::append(Material* m)
{
	unsigned i=size();
	resize(i+1);

	WrappedVector<Material*,unsigned>::set(i,m);
	return i;
}

void MaterialSet::set(unsigned i,Material* m)
{
	if (i >= size())
		resize(i+1);

	WrappedVector<Material*,unsigned>::set(i,m);
}
