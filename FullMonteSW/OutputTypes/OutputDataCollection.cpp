/*
 * OutputDataCollection.cpp
 *
 *  Created on: May 31, 2017
 *      Author: jcassidy
 */

#include "OutputDataCollection.hpp"
#include "OutputData.hpp"
#include "OutputDataType.hpp"

using namespace std;

OutputDataCollection::OutputDataCollection()
{

}

OutputDataCollection::~OutputDataCollection()
{
	clear();
}

void OutputDataCollection::clear()
{
	for(auto it = m_contents.begin(); it != m_contents.end(); ++it)
		delete *it;
	m_contents.clear();
}

unsigned OutputDataCollection::size() const
{
	return m_contents.size();
}

unsigned OutputDataCollection::add(OutputData* p)
{
	if (!p)
		return -1U;

	list<OutputData*>::iterator it;
	unsigned i=0;
	for(it = m_contents.begin(); it != m_contents.end(); ++it,++i)
	{
		if ((*it)->name() == p->name())
			cout << "WARNING - OutputDataCollection::add() name '" << p->name() << "' already present" << endl;
	}

	m_contents.insert(it,p);
	return i;
}

void OutputDataCollection::remove(std::string name)
{
	list<OutputData*>::iterator it;

	unsigned N=0;

	for(it = m_contents.begin(); it != m_contents.end(); ++it)
	{
		if ((*it)->name() == name)
		{
			m_contents.erase(it);
			++N;
		}
	}

	if (!N)
		cout << "WARNING - OutputDataCollection::remove() failed to find data named '" << name << "'" << endl;
}

void OutputDataCollection::remove(OutputData* p)
{
	list<OutputData*>::iterator it;

	unsigned N=0;

	for(it = m_contents.begin(); it != m_contents.end(); ++it)
	{
		if (*it == p)
		{
			m_contents.erase(it);
			++N;
		}
	}

	if (!N)
		cout << "WARNING - OutputDataCollection::remove() failed to find data " << p << endl;
}

void OutputDataCollection::start()
{
	m_iterator = m_contents.begin();
}

void OutputDataCollection::next()
{
	if (m_iterator != m_contents.end())
		++m_iterator;
}

void OutputDataCollection::prev()
{
	if (m_iterator != m_contents.begin())
		--m_iterator;
}

bool OutputDataCollection::done() const
{
	return m_iterator == m_contents.end();
}

OutputData* OutputDataCollection::current() const
{
	return m_iterator == m_contents.end() ? nullptr : *m_iterator;
}


OutputData* OutputDataCollection::getByIndex(unsigned i)
{
	list<OutputData*>::iterator it;
	for(it = m_contents.begin(); it != m_contents.end() && i; --i, ++it)
		{}
	return it == m_contents.end() ? nullptr : *it;
}

OutputData* OutputDataCollection::getByName(std::string s,unsigned i)
{
	list<OutputData*>::iterator it;
	for(it = m_contents.begin(); it != m_contents.end() && ((*it)->name() != s || i); ++it)
		i -= ((*it)->name() == s);

	return it == m_contents.end() ? nullptr : *it;
}

OutputData* OutputDataCollection::getByType(std::string s,unsigned i)
{
	list<OutputData*>::iterator it;
	i++;
	for(it = m_contents.begin(); it != m_contents.end() && i; ++it)
		i -= ((*it)->type()->name() == s);

	return it == m_contents.end() ? nullptr : *it;
}
