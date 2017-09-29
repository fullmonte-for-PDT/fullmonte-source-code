/*
 * chi2test.cpp
 *
 *  Created on: Jun 26, 2017
 *      Author: siyunli
 */

#include "chi2test.hpp"
#include <iostream>
Chi2Test::Chi2Test()
{
}

Chi2Test::~Chi2Test()
{
}

// Compute chi_squared statistic and p-value
void Chi2Test::compute()
{
	makeSortedTuple();
	this->absError = absoluteError();
	if (!checkN())
	{
		makePartialTuple();
		this->chi2stat = chi2statistic();
		if (chi2stat == 0)
			this->pVal = 1;
		else
			this->pVal = pValue();
	}
}

// Generate a string summarizing all the computed values
std::string Chi2Test::getResultSummary() const
{
	if (!checkN())
	{
		std::stringstream ss;
		ss << std::fixed << std::setprecision(6) << chi2stat << "\t" << pVal << "\t" << absError << "\t" << ObservedPartial << "\t" << ObservedTotal << "\t" << ExpectedPartial << "\t" << ExpectedTotal << std::endl;
		return ss.str();
	}
	else
		return "N is too large for given threshold. No test performed.";
}

// Compute chi_squared statistic and calculate the amount of energy included in the calculation
float Chi2Test::chi2statistic()
{
	float sum = 0;
	unsigned counter = 0;
	while (counter < partialTarget.size())
	{
		sum += pow((std::get<2>(partialTarget[counter]) - std::get<1>(partialTarget[counter])),2) / std::get<2>(partialTarget[counter]);
		this->ExpectedPartial += std::get<2>(partialTarget[counter]);
		this->ObservedPartial += std::get<1>(partialTarget[counter]);
		++counter;
	}
	unsigned counter2 = 0;
	while (counter2 < target.size())
	{
		this->ExpectedTotal += std::get<2>(target[counter2]);
		this->ObservedTotal += std::get<1>(target[counter2]);
		++counter2;
	}
	return sum;
}

// Compute p-value of this chi_squared distribution
float Chi2Test::pValue()
{
	// Create chi2 distribution object with specified degrees of freedom
	boost::math::chi_squared chi2dist(topNvalues);
	// p value = 1 - cdf(distribution, statistic)
	return 1-boost::math::cdf(chi2dist, chi2stat);
}

// Compute sum of absolute error among all tetrahedrons
float Chi2Test::absoluteError()
{
	float sum = 0;
	for (unsigned i = 0; i < target.size(); ++i)
		sum += std::abs(std::get<2>(target[i]) - std::get<1>(target[i]));
	return sum;
}

// Put expected and observed data into a tuple vector and sort
void Chi2Test::makeSortedTuple()
{
	// Make tuples
	for (unsigned i = 0; i < expectedData.size(); ++i)
		target.push_back(std::make_tuple(i, observedData[i], expectedData[i]));

	// Sort tuples
	std::sort(target.begin(), target.end(), [](const std::tuple<int, float, float>& firstTuple, const std::tuple<int, float, float>& secondTuple){
		return std::get<2>(firstTuple) > std::get<2>(secondTuple); 
	});
}

// Copy first N tuples
void Chi2Test::makePartialTuple()
{
	std::vector<std::tuple<unsigned, float, float>>::const_iterator first = target.begin();
	std::vector<std::tuple<unsigned, float, float>>::const_iterator last = target.begin()+topNvalues;
	std::vector<std::tuple<unsigned, float, float>> partial(first,last);
	this->partialTarget = partial;
}
