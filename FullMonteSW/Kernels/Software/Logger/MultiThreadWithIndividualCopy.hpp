/*
 * MultiThreadWithIndividualCopy.hpp
 *
 *  Created on: Feb 8, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_LOGGER_MULTITHREADWITHINDIVIDUALCOPY_HPP_
#define KERNELS_SOFTWARE_LOGGER_MULTITHREADWITHINDIVIDUALCOPY_HPP_

#include <mutex>
#include <type_traits>

#include "AbstractScorer.hpp"

/** MultiThreadWithIndividualCopy creates a Scorer from a simple single-threaded Logger type.
 * It creates one logger per thread, allows it to run until completion, then merges the results.
 *
 * It derives a new Logger type from the base, with an additional commit(AbstractScorer* S) method that merges
 * the results to the parent and clears the copy. Merging is protected by a mutex to avoid hazards, and by
 * default uses the += operator on the Logger::State type.
 *
 * Logger concept requirements:
 *
 * 		typedef State		State type held by the Logger
 *
 * 		Logger State concept requirements:
 *
 * 			operator+=(const State&)
 * 			void clear()
 *
 * 			OutputData* createOutputData(const State&)			Package up the output data
 */

template<class SingleThreadLoggerT>class MultiThreadWithIndividualCopy : public AbstractScorer
{
public:
	MultiThreadWithIndividualCopy(){}

	////// Concrete Scorer override requirements
	virtual void clear() override;
	virtual void prepare(const Kernel* K) override;
	virtual void postResults(OutputDataCollection* C) const override;

	/// The SingleThreadLoggerT with an added commit(AbstractScorer*) override
	class Logger;
	typedef SingleThreadLoggerT									SingleThreadLogger;
	typedef typename SingleThreadLogger::State					State;


	/// Return a copy of the prototype
	Logger createLogger(){ return m_prototype; }

protected:
	void merge(SingleThreadLogger& st);

private:
	std::mutex			m_mutex;
	State				m_state;

	Logger				m_prototype;
};


/** Need to ensure that events don't happen between the merge and the clear. Best way is to always have the merge driven
 * by the worker thread.
 */

template<class SingleThreadLoggerT>void MultiThreadWithIndividualCopy<SingleThreadLoggerT>::merge(SingleThreadLoggerT& st)
{
	{
		std::unique_lock<std::mutex> L(m_mutex);
		m_state += st.state();
	}
	st.eventClear(*this);
}

template<class SingleThreadLoggerT>void MultiThreadWithIndividualCopy<SingleThreadLoggerT>::prepare(const Kernel* K)
{
	m_prototype.prepare(K);
}

template<class SingleThreadLoggerT>void MultiThreadWithIndividualCopy<SingleThreadLoggerT>::clear()
{
	m_state.clear();
}

template<class SingleThreadLoggerT>void MultiThreadWithIndividualCopy<SingleThreadLoggerT>::postResults(OutputDataCollection* C) const
{
	SingleThreadLogger::postResults(C,m_state);
}

template<class SingleThreadLoggerT>class MultiThreadWithIndividualCopy<SingleThreadLoggerT>::Logger : public SingleThreadLoggerT
{
	public:
		void eventCommit(AbstractScorer& S)
		{
			static_cast<MultiThreadWithIndividualCopy<SingleThreadLoggerT>&>(S).merge(*this);
		}
};

#endif /* KERNELS_SOFTWARE_LOGGER_MULTITHREADWITHINDIVIDUALCOPY_HPP_ */
