/*
 * DoseHistogram.hpp
 *
 *  Created on: Oct 21, 2016
 *      Author: jcassidy
 */

#ifndef QUERIES_DOSEHISTOGRAM_HPP_
#define QUERIES_DOSEHISTOGRAM_HPP_

#include <FullMonteSW/OutputTypes/OutputData.hpp>

#include <vector>

struct ordered_t { constexpr ordered_t(){} };
extern const ordered_t ordered;


/** OutputData class for a DoseHistogram (dose-volume or dose-surface)
 *
 */

class DoseHistogram : public OutputData
{
public:
	/** Build from iterator range of sorted values.
	 * For Iterator it, measure(*it) and dose(*it) must be valid expressions.
	 */

	DoseHistogram(){}

	template<class Iterator>DoseHistogram(Iterator begin,Iterator end,const ordered_t);

	struct Element
	{
		float measure;		// measure (area/volume)
		float cmeasure;		// cumulative measure (area/volume with dose <= specified)
		float cdf;			// cmeasure/total measure
		float dose;			// dose for this element
	};



	typedef std::vector<Element>::const_iterator const_iterator;

#ifndef SWIG
	CLONE_METHOD(OutputData,DoseHistogram)
	//virtual const char* typeString() const override { return "DoseHistogram"; }
	virtual const OutputDataType* type() const override;
#endif

	const_iterator begin() 	const;
	const_iterator end() 	const;

	const_iterator percentileOfDose(float dose) const;
	const_iterator doseAtPercentile(float p) 	const;

	/// Number of elements in the CDF
	std::size_t		dim() const;

	/// Total weight in the histogram
	float			totalMeasure() const;

private:
	std::vector<Element>		m_histogram;
};

template<class Iterator>DoseHistogram::DoseHistogram(Iterator begin,Iterator end,const ordered_t)
{
	std::size_t N = end-begin;

	m_histogram.resize(N);

	m_histogram[0].cmeasure = m_histogram[0].measure = begin->measure;

	// Copy dose & measure, and accumulate cmeasure
	for(std::vector<Element>::iterator it = m_histogram.begin();begin != end; ++begin,++it)
	{
		it->measure = begin->measure;
		it->cmeasure = (it-1)->cmeasure + it->measure;

		it->dose = begin->dose;
	}

	// scale measures to get CDF as % of total measure
	for(std::vector<Element>::iterator it = m_histogram.begin(); it != m_histogram.end(); ++it)
		it->cdf = it->cmeasure/m_histogram.back().cmeasure;
}

#endif /* QUERIES_DOSEHISTOGRAM_HPP_ */
