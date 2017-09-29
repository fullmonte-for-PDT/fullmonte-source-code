/*
 * EventScorer.cpp
 *
 *  Created on: May 31, 2017
 *      Author: jcassidy
 */

#include "EventScorer.hpp"
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>

void EventLogger::postResults(OutputDataCollection* C,const MCEventCounts& st)
{
	OutputData* d = new MCEventCountsOutput(st);
	d->name("EventCounts");
	C->add(d);
}

void EventLogger::prepare(const Kernel*)
{
}
