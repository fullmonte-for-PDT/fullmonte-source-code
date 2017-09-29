/*
 * ThreadedMCKernelBase.hpp
 *
 *  Created on: Feb 1, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_THREADEDMCKERNELBASE_HPP_
#define KERNELS_SOFTWARE_THREADEDMCKERNELBASE_HPP_

#include "../MCKernelBase.hpp"
#include <thread>
#include <vector>

#include <boost/random/additive_combine.hpp>

class AbstractScorer;
class Geometry;

class ThreadedMCKernelBase : public MCKernelBase
{
public:
	~ThreadedMCKernelBase();

	void				threadCount(unsigned Nth)					{ m_threadCount=Nth;					}
	unsigned			threadCount()						const	{ return m_threadCount;					}

	// Final overrides: distribute the requests to the underlying threads
	virtual unsigned long long 	simulatedPacketCount() 	const 	final override;

	// Final override: do parent prep and then create thread structures
	virtual void 				prepare_()						final override;

	class Thread;

	void resetSeedRng()
	{
		m_seedGenerator.seed(rngSeed_);
	}

	unsigned getUnsignedRNGSeed()
	{
		unsigned rnd = m_seedGenerator();
		m_seedGenerator.discard(1000);
		return rnd;
	}

protected:
	void addScorer(AbstractScorer* s);

private:
	virtual void				awaitFinish()					final override;
	virtual void 				start_() 						final override;

	virtual void				gatherResults()					final override;

	// implemented by derived classes
	virtual Thread*				makeThread()=0;
	virtual void				parentPrepare()=0;

	boost::random::ecuyer1988 					m_seedGenerator;

	unsigned 									m_threadCount=8;
	std::vector<ThreadedMCKernelBase::Thread*> 	m_workers;

	std::vector<AbstractScorer*>				m_scorers;
};



#ifndef SWIG

class ThreadedMCKernelBase::Thread
{
public:
	virtual ~Thread(){}

	void start(ThreadedMCKernelBase* kernel,unsigned long long N);
	bool done() const;
	void awaitFinish();

protected:
	unsigned long long 	m_nPktReq=0;
	unsigned long long 	m_nPktDone=0;

private:
	virtual void doWork(ThreadedMCKernelBase* kernel)=0;

	static void threadFunction(ThreadedMCKernelBase* kernel,ThreadedMCKernelBase::Thread* t);

	bool 				m_done=false;

	std::thread m_thread;

	friend class ThreadedMCKernelBase;
};

#endif

#endif /* KERNELS_SOFTWARE_THREADEDMCKERNELBASE_HPP_ */
