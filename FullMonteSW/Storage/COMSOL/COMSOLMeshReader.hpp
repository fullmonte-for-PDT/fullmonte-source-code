/*
 * COMSOLMeshReader.hpp
 *
 *  Created on: Jul 24, 2017
 *      Author: Siyun Li
 */

#ifndef COMSOLMESHREADER_HPP_
#define COMSOLMESHREADER_HPP_

class TetraMesh;

class PTTetraMeshBuilder;

#include <string>
#include <sstream>

class COMSOLMeshReader
{
public:
	COMSOLMeshReader();
	virtual ~COMSOLMeshReader();

	void filename(std::string fn);

	/// Read from the file
	void read();

	/// Call after read() to do the conversion
	TetraMesh*			mesh() const;

	PTTetraMeshBuilder*		builder() const;

private:
	PTTetraMeshBuilder* 		m_builder;
	std::string 			m_filename;
};

#endif /* COMSOLMESHREADER_HPP_ */

