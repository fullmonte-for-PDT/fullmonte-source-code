/*
 * chi2test.hpp
 *
 *  Created on: Jun 26, 2017
 *      Author: siyunli
 */

#ifndef CHI2TEST_HPP_
#define CHI2TEST_HPP_

#include <vector>
#include <string>
#include <sstream>
// for sorting
#include <tuple>
#include <algorithm>

// for chi2 test
#include <cmath>
#include <boost/math/distributions/chi_squared.hpp>

class Chi2Test
{
public:
	Chi2Test();
	virtual ~Chi2Test();

	// Top N values used to calculate chi-squared statistic and p-value
	unsigned topNvalues = 0;
	// Threshold for minimum value of the data to be included in chi-squared test
	float threshold = 0;

	std::vector<float> expectedData;
	std::vector<float> observedData;

	// Compute chi_squared statistic and p-value
	void compute();

	// Get chi_squared statistic
	float getChi2Stat() const {	return this->chi2stat;	}

	// Get p-value of this chi_squared distribution
	float getPvalue() const {	return this->pVal;	}
	
	// Get sum of absolute errors
	float getAbsError() const {	return this->absError;	}

	// Return summary of all the computed values
	std::string getResultSummary() const;

	// Return expected partial sum
	float getExpectedPartial() const {	return this->ExpectedPartial;	}


private:
	float chi2stat = 0;
	float pVal = 0;
	float absError = 0;
        float ExpectedPartial = 0;
        float ExpectedTotal = 0;
        float ObservedPartial = 0;
        float ObservedTotal = 0;

	// Vector of tuples containing all values in format: observedData, expectedData
	std::vector<std::tuple<unsigned, float, float>> target;

	// Vector containing only the top N values of 'target'
	std::vector<std::tuple<unsigned, float, float>> partialTarget;

        // Check if the Nth value is smaller than threshold
        bool checkN() const {	return std::get<2>(target[topNvalues]) < threshold;	}

	// Compute chi_squared statistic
	float chi2statistic();

	// Compute p-value of this chi_squared distribution
	float pValue();

	// Compute sum of absolute error
	float absoluteError();

	// Put expected and observed data into a tuple vector and sort
	void makeSortedTuple();

	// Copy first N tuples
	void makePartialTuple();
};

#endif /* CHI2TEST_HPP_ */
