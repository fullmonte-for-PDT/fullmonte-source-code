/*
 * TextFileReader.cpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#include "TextFileReader.hpp"

TextFileReader::TextFileReader()
{
}

TextFileReader::~TextFileReader()
{
}

void TextFileReader::filename(std::string fn)
{
	m_filename = fn;
}

const std::string& TextFileReader::filename() const
{
	return m_filename;
}

