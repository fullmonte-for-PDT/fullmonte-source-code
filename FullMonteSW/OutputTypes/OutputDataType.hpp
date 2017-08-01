/*
 * OutputDataType.hpp
 *
 *  Created on: May 31, 2017
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_OUTPUTDATATYPE_HPP_
#define OUTPUTTYPES_OUTPUTDATATYPE_HPP_

#include <string>

class OutputDataType
{
public:
	OutputDataType(std::string typeName) : m_name(typeName){};
	virtual ~OutputDataType(){}

	virtual const std::string& name() const { return m_name; }



private:

	std::string		m_name;
};



#endif /* OUTPUTTYPES_OUTPUTDATATYPE_HPP_ */
