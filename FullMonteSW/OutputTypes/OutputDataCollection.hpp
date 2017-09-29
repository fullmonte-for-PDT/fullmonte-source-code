/*
 * OutputDataCollection.hpp
 *
 *  Created on: May 31, 2017
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_OUTPUTDATACOLLECTION_HPP_
#define OUTPUTTYPES_OUTPUTDATACOLLECTION_HPP_

#include <list>
#include <string>

class OutputData;

class OutputDataCollection
{

public:
	OutputDataCollection();
	~OutputDataCollection();

	/// Clear
	void clear();

	/// Check number of elements
	unsigned 		size() const;

	/// Methods for iterating through the results
	void			start();
	void			next();
	void 			prev();

	bool			done() const;

	OutputData*		current() const;

	/// Result extraction
	OutputData*		getByType(std::string typeName,unsigned i=0);
	OutputData*		getByName(std::string name,unsigned i=0);
	OutputData*		getByIndex(unsigned i);

	/// Managing the result set
	unsigned		add(OutputData* d);
	void			remove(std::string name);
	void			remove(OutputData* d);

private:
	unsigned long long 					m_packetCount=0;
	std::list<OutputData*>				m_contents;
	std::list<OutputData*>::iterator 	m_iterator;
};


#endif /* OUTPUTTYPES_OUTPUTDATACOLLECTION_HPP_ */
