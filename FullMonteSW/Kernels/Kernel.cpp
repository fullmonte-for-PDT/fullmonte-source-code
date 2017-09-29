/*
 * Kernel.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: jcassidy
 */

#include "Kernel.hpp"

#include <FullMonteSW/Geometry/Material.hpp>
#include <FullMonteSW/Geometry/MaterialSet.hpp>

#include <FullMonteSW/Kernels/KernelObserver.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include <FullMonteSW/OutputTypes/OutputData.hpp>
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>
#include <iostream>

using namespace std;

void Kernel::runSync()
{
	if (m_status != Idle && m_status != Finished)
		throw std::logic_error("Kernel::runSync called while status not Idle || Finished");

	updateStatus(Preparing);

	results()->clear();


	// notify observers we're preparing
	for(const auto o : m_observers)
		o->notify_prepare(*this);

	prepare_();

	// notify observers we're starting
	for(const auto o: m_observers)
		o->notify_start(*this);

	updateStatus(Running);

	start_();

	awaitFinish();

	gatherResults();

	updateStatus(Finished);

	// notify observers we're done
	for(const auto o : m_observers)
		o->notify_finish(*this);

	// share the results
	OutputDataCollection* res = results();
	for(unsigned i=0;i<res->size();++i)
	{
		OutputData* d = res->getByIndex(i);
		for(const auto o : m_observers)
			o->notify_result(*this,d);
	}
}

void Kernel::startAsync()
{
	if (m_status != Idle && m_status != Finished)
		throw std::logic_error("Kernel::runSync called while status not Idle || Finished");

	results()->clear();

	// launch a thread that runs the normal synchronous routine
	m_parentThread = std::thread(std::mem_fn(&Kernel::runSync),this);

	// return as soon as it's running
	awaitStatus(Running);
}

void Kernel::finishAsync()
{
	m_parentThread.join();
}

void Kernel::updateStatus(Status status)
{
	std::unique_lock<std::mutex> lk(m_statusMutex);
	m_status=status;
	m_statusCV.notify_all();
}

Kernel::Status Kernel::status() const
{
	return m_status;
}

void Kernel::awaitStatus(Status status)
{
	std::unique_lock<std::mutex> lk(m_statusMutex);
	m_statusCV.wait(lk, [this,status]{ return m_status==status; });
}

const MaterialSet* Kernel::materials() const
{
	return m_materialSet;
}

void Kernel::geometry(const Geometry* G)
{
	m_geometry=G;
}

const Geometry* Kernel::geometry() const
{
	return m_geometry;
}

void Kernel::materials(const MaterialSet* ms)
{
	m_materialSet=ms;
}

OutputDataCollection* Kernel::results()
{
	if (!m_results)
		m_results = new OutputDataCollection();
	return m_results;
}
