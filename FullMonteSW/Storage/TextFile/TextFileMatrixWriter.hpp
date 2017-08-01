/*
 * TextFileMatrixWriter.hpp
 *
 *  Created on: Jun 5, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TEXTFILE_TEXTFILEMATRIXWRITER_HPP_
#define STORAGE_TEXTFILE_TEXTFILEMATRIXWRITER_HPP_

class OutputData;
#include <string>

class TextFileMatrixWriter
{
public:
	TextFileMatrixWriter();
	~TextFileMatrixWriter();

	void 			source(OutputData* d);
	OutputData*		source() const;

	void 			filename(std::string fn);

	void			precision(unsigned p);
	void			width(unsigned w);

	void 			write();

private:
	std::string		m_filename;
	unsigned		m_precision=0;
	unsigned		m_width=0;
	unsigned		m_columns=10;

	OutputData*		m_data=nullptr;
};

#endif /* STORAGE_TEXTFILE_TEXTFILEMATRIXWRITER_HPP_ */
