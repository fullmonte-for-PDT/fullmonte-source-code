/*
 * LoggerWithState.hpp
 *
 *  Created on: May 31, 2017
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_LOGGER_LOGGERWITHSTATE_HPP_
#define KERNELS_SOFTWARE_LOGGER_LOGGERWITHSTATE_HPP_

#include "BaseLogger.hpp"

/** Helpful template for a simple logger that contains just simple state (eg. event counts)
 *
 * Logger concept requirements not fulfilled
 *
 * 		void eventCommit(AbstractScorer*)
 *
 * State concept requirements
 * 		Default-constructible 	to a known/zero state
 * 		operator+= 				acts in the usual way
 * 		void clear()			clears the state to its initialized
 *
 *
 */

template<class StateT>class LoggerWithState : public BaseLogger
{
public:
	typedef StateT 	State;
	void eventClear(AbstractScorer&){ m_state.clear(); }

	const State& state() const { return m_state; }

	void merge(const LoggerWithState& L){ m_state += L.m_state; }

protected:
	State m_state;
};



#endif /* KERNELS_SOFTWARE_LOGGER_LOGGERWITHSTATE_HPP_ */
