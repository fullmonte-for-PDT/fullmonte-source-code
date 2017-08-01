/*
 * TIMOS.hpp
 *
 *  Created on: Mar 5, 2015
 *      Author: jcassidy
 */

#ifndef TIMOSREADER_HPP_
#define TIMOSREADER_HPP_

class TetraMesh;

class PTTetraMeshBuilder;

#include <string>

class TIMOSMeshReader
{
public:
	TIMOSMeshReader();
	virtual ~TIMOSMeshReader();

	void filename(std::string fn);

	/// Read from the file
	void read();

	/// Call after read() to do the conversion
	TetraMesh*						mesh() const;

	PTTetraMeshBuilder*				builder() const;

private:
	PTTetraMeshBuilder* 	m_builder;
	std::string 			m_filename;
};

#endif /* TIMOS_HPP_ */

