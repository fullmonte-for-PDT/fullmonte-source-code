/*
 * TextFileDoseHistogramWriter.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: jcassidy
 */

#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

#include "TextFileDoseHistogramWriter.hpp"
#include <FullMonteSW/Queries/DoseHistogramCollection.hpp>
#include <FullMonteSW/Queries/DoseHistogram.hpp>


TextFileDoseHistogramWriter::TextFileDoseHistogramWriter()
{

}

TextFileDoseHistogramWriter::~TextFileDoseHistogramWriter()
{
}

void TextFileDoseHistogramWriter::filename(std::string fn)
{
	m_filename=fn;
}

const std::string& TextFileDoseHistogramWriter::filename() const
{
	return m_filename;
}

DoseHistogramCollection* TextFileDoseHistogramWriter::collection(DoseHistogramCollection* C)
{
	swap(C,m_collection);
	return C;
}

DoseHistogramCollection* TextFileDoseHistogramWriter::collection() const
{
	return m_collection;
}

void TextFileDoseHistogramWriter::write()
{
	ofstream os(filename());

	os << m_collection->count() << endl;

	for(unsigned h=0;h<m_collection->count();++h)
	{
		const DoseHistogram* H = m_collection->get(h);
		os << H->dim() << fixed << endl;

		for(const auto e : *H)
			os <<
				setprecision(2) << setw(6) << e.cmeasure << ' ' <<
				setprecision(4) << setw(6) << e.cdf << ' ' <<
				setprecision(3) << e.dose << endl;
	}
}

//void TextFileDoseHistogramWriter::
