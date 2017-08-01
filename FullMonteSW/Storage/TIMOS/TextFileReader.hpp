/*
 * TextFileReader.hpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TIMOS_TEXTFILEREADER_HPP_
#define STORAGE_TIMOS_TEXTFILEREADER_HPP_

#include <string>

class TextFileReader
{
public:
	TextFileReader();
	~TextFileReader();

	void filename(std::string fn);
	const std::string& filename() const;

private:
	std::string		m_filename;
};



#endif /* STORAGE_TIMOS_TEXTFILEREADER_HPP_ */
