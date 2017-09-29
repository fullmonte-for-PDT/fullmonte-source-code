/*
 * OutputData.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: jcassidy
 */

#include "OutputData.hpp"

using namespace std;

void OutputData::acceptVisitor(Visitor* v)
{
	v->doVisit(this);
}

void OutputData::name(std::string n)
{
	m_name = n;
}

const string& OutputData::name() const
{
	return m_name;
}
