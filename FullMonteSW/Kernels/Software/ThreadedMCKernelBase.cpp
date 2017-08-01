/*
 * ThreadedMCKernelBase.cpp
 *
 *  Created on: Feb 1, 2016
 *      Author: jcassidy
 */

#include "ThreadedMCKernelBase.hpp"

#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>

#include "Logger/AbstractScorer.hpp"

void ThreadedMCKernelBase::awaitFinish()
{
	for(auto* t : m_workers)
	{
		t->awaitFinish();
		delete t;			// if std::thread outlives main thread, an exception is raised so make sure it dies when it's done
	}
	m_workers.clear();
}


unsigned long long ThreadedMCKernelBase::simulatedPacketCount() const
{
	unsigned long long sum=0;
	for(const ThreadedMCKernelBase::Thread* t : m_workers)
		sum += t->m_nPktDone;
	return sum;
}

void ThreadedMCKernelBase::start_()
{
	unsigned long long N = Npkt_/m_threadCount;

	for(auto t : m_workers)
		t->start(this,N);
}

void ThreadedMCKernelBase::prepare_()
{
	// get the parent class (shared state) ready
	parentPrepare();

	for(const auto s : m_scorers)
	{
		s->prepare(this);
		s->clear();
	}

	// create individual thread instances but do not start them yet
	m_workers.resize(m_threadCount);

	for(auto& w: m_workers)
		w = makeThread();
}

ThreadedMCKernelBase::~ThreadedMCKernelBase()
{
	for(auto t : m_workers)
		delete t;
}

void ThreadedMCKernelBase::gatherResults()
{
	OutputDataCollection* C = results();
	C->clear();

	for(const auto s : m_scorers)
		s->postResults(C);
}


void ThreadedMCKernelBase::Thread::start(ThreadedMCKernelBase* kernel,unsigned long long N)
{
	m_nPktDone=0;
	m_nPktReq=N;
	m_thread = std::thread(threadFunction,kernel,this);
}

bool ThreadedMCKernelBase::Thread::done() const
{
	return m_done;
}

void ThreadedMCKernelBase::Thread::awaitFinish()
{
	m_thread.join();
}

void ThreadedMCKernelBase::Thread::threadFunction(ThreadedMCKernelBase* kernel,ThreadedMCKernelBase::Thread* t)
{
	t->m_done=false;
	t->doWork(kernel);
	t->m_done=true;
}

void ThreadedMCKernelBase::addScorer(AbstractScorer* s)
{
	m_scorers.push_back(s);
}
