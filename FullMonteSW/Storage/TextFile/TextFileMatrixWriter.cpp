/*
 * TextFileMatrixWriter.cpp
 *
 *  Created on: Jun 5, 2017
 *      Author: jcassidy
 */

#include "TextFileMatrixWriter.hpp"
#include <FullMonteSW/OutputTypes/OutputData.hpp>

#include <fstream>
#include <iomanip>
#include <FullMonteSW/OutputTypes/SpatialMap.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap2D.hpp>

using namespace std;

TextFileMatrixWriter::TextFileMatrixWriter()
{

}

TextFileMatrixWriter::~TextFileMatrixWriter()
{
}

void TextFileMatrixWriter::source(OutputData* d)
{
	m_data=d;
}

OutputData* TextFileMatrixWriter::source() const
{
	return m_data;
}

void TextFileMatrixWriter::filename(string fn)
{
	m_filename=fn;
}

void TextFileMatrixWriter::precision(unsigned p)
{
	m_precision=p;
}

void TextFileMatrixWriter::width(unsigned w)
{
	m_width=w;
}

void TextFileMatrixWriter::write()
{
	ofstream os(m_filename.c_str());

	if (m_precision != 0)
		os << scientific << setprecision(m_precision);

	if(!m_data)
		cout << "TextFileMatrixWriter::write() provided null data" << endl;
	else if (const SpatialMap<float>* vf = dynamic_cast<const SpatialMap<float>*>(m_data))
	{
		if (const SpatialMap2D<float>* vf2 = dynamic_cast<const SpatialMap2D<float>*>(vf))
			os << "2 " << vf2->dims()[0] << ' ' << vf2->dims()[1];
		else
			os << "1 " << vf->dim();
		os << endl;

		for(unsigned i=0;i < vf->dim(); ++i)
			os << setw(m_width) << vf->get(i) << ((i % m_columns) == m_columns-1 ? '\n' : ' ');
	}
	else
		cout << "TextFileMatrixWriter::write() unrecognized data type '" << m_data->type()->name() << "'" << endl;

}

