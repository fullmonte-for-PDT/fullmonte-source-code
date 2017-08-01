/*
 * ErrorChecker.cpp
 *
 *  Created on: May 18, 2017
 *      Author: jcassidy
 */

#include "ErrorChecker.hpp"

void ErrorChecker::clear()
{
	for(const auto p : m_errors)
		delete p;
	m_currentError=0;
	m_tested=0;
}

void ErrorChecker::skipToError(unsigned i)
{
	if (i >= m_errors.size() && i != 0)
		throw std::out_of_range("ErrorChecker::skipToError(i)");
	m_currentError=i;
}

bool ErrorChecker::nextError()
{
	m_currentError++;
	return m_currentError == m_errors.size();
}

bool ErrorChecker::prevError()
{
	if (m_currentError == 0)
		return false;
	m_currentError--;
	return true;
}

void MeshErrorChecker::mesh(const TetraMesh* M)
{
	m_mesh=M;
	clear();
}




