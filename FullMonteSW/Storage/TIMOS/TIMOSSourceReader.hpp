/*
 * TIMOSSourceReader.hpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TIMOS_TIMOSSOURCEREADER_HPP_
#define STORAGE_TIMOS_TIMOSSOURCEREADER_HPP_

#include <vector>

#include "TIMOSSource.hpp"
#include "TextFileReader.hpp"

class TIMOSSourceReader : public TextFileReader
{
public:
	TIMOSSourceReader();
	~TIMOSSourceReader();

	void filename(std::string fn);

	void read();

	Source::Abstract* 	source() const;

private:
	void read(std::istream& is,TIMOS::GenericSource& s);
	void read(std::istream& is,TIMOS::PointSource& p);
	void read(std::istream& is,TIMOS::FaceSource& p);
	void read(std::istream& is,TIMOS::PencilBeamSource& p);
	void read(std::istream& is,TIMOS::TetraSource& p);

	static Source::Abstract* 	convertToSource(TIMOS::GenericSource);
	static TIMOS::GenericSource convertFromSource(const Source::Abstract*);

	Source::Abstract*					m_source;
	std::string							m_filename;
	std::vector<TIMOS::GenericSource>	m_sources;

};



#endif /* STORAGE_TIMOS_TIMOSSOURCEREADER_HPP_ */
