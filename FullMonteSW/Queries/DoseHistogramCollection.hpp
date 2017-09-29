/*
 * DoseHistogramCollection.hpp
 *
 *  Created on: Apr 25, 2017
 *      Author: jcassidy
 */

#ifndef QUERIES_DOSEHISTOGRAMCOLLECTION_HPP_
#define QUERIES_DOSEHISTOGRAMCOLLECTION_HPP_

#include <FullMonteSW/OutputTypes/OutputData.hpp>
#include <vector>

class DoseHistogram;

class DoseHistogramCollection : public OutputData
{
public:
	DoseHistogramCollection();
	DoseHistogramCollection(const DoseHistogramCollection&) = default;

	virtual ~DoseHistogramCollection();

	void add(DoseHistogram* h);
	void remove(DoseHistogram* h);

	DoseHistogram* get(unsigned i) const;

	unsigned count() const;

	//virtual const char* typeString() const override { return "DoseHistogramCollection"; }
	virtual DoseHistogramCollection* clone() const override;

	virtual const OutputDataType* type() const override;

private:
	std::vector<DoseHistogram*>		m_histograms;
};

#endif /* QUERIES_DOSEHISTOGRAMCOLLECTION_HPP_ */
