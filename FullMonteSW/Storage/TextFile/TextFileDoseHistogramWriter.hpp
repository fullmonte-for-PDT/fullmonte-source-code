/*
 * TextFileDoseHistogramWriter.hpp
 *
 *  Created on: Apr 28, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TEXTFILE_TEXTFILEDOSEHISTOGRAMWRITER_HPP_
#define STORAGE_TEXTFILE_TEXTFILEDOSEHISTOGRAMWRITER_HPP_

#include <string>
class DoseHistogramCollection;
class DoseHistogram;

/** Writes a collection of dose histograms to a text file
 *
 * The histogram is represented as a standard CDF, ie. the percentage of target receiving a dose <= the specified dose.
 *
 * Format:
 *
 * <Nh>
 * {
 * 		<Ns>
 * 		{
 * 			<cmeasure> <cmeasure%> <dose>
 * 		}Ns
 * }Nh
 *
 *
 * Nh			Number of histograms
 * Ns			Number of samples in each histogram
 * cmeasure		Cumulative measure (sum of area or volume)
 * cmeasure%	Cumulative measure as a % of total histogram
 * dose			Dose
 */

class TextFileDoseHistogramWriter
{
public:
	TextFileDoseHistogramWriter();
	virtual ~TextFileDoseHistogramWriter();

	void						filename(std::string fn);
	const std::string& 			filename() const;

	void 						write();

	DoseHistogramCollection*	collection(DoseHistogramCollection* C);
	DoseHistogramCollection*	collection() const;

private:
	std::string					m_filename;
	DoseHistogramCollection		*m_collection=nullptr;
};




#endif /* STORAGE_TEXTFILE_TEXTFILEDOSEHISTOGRAMWRITER_HPP_ */
