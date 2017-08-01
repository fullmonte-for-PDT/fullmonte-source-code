/*
 * AbstractScorer.hpp
 *
 *  Created on: Sep 29, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_LOGGER_ABSTRACTSCORER_HPP_
#define KERNELS_SOFTWARE_LOGGER_ABSTRACTSCORER_HPP_

/** Scorer Concept
 *
 * A Scorer manages the scoring process, including the following responsibilities:
 *
 * 	1) Acting as a Logger factory when the Kernel starts up
 * 	2) Conversion of the Logger information into a finalized OutputData*
 *
 *
 *
 *
 * Concrete class overrides
 *
 * 		virtual postResults(OutputDataCollection*)				Posts its results to the specified collection
 * 		virtual void prepate(Kernel* K)							Prepares to score the results of specified kernel
 * 		virtual void clear()									Resets all scores to a default state
 *
 *
 * Concept requirements
 *
 * 		Default-constructible
 * 		typedef Logger											The type of logger created
 * 		Logger createLogger();									Logger factory
 */

class OutputDataCollection;
class Kernel;

class AbstractScorer
{
public:
	virtual ~AbstractScorer();

	virtual void clear()=0;
	virtual void prepare(const Kernel* K)=0;
	virtual void postResults(OutputDataCollection* o) const=0;

protected:
	AbstractScorer();

private:
};

#endif /* KERNELS_SOFTWARE_LOGGER_ABSTRACTSCORER_HPP_ */
