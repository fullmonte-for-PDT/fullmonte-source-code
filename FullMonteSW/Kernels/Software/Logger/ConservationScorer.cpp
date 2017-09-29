/*
 * ConservationScorer.cpp
 *
 *  Created on: May 31, 2017
 *      Author: jcassidy
 */

#include <FullMonteSW/OutputTypes/MCConservationCounts.hpp>
#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>

#include "ConservationScorer.hpp"

void ConservationLogger::postResults(OutputDataCollection* C,const MCConservationCounts& st)
{
	OutputData* d = new MCConservationCountsOutput(st);
	d->name("ConservationCounts");
	C->add(d);
}


void ConservationLogger::prepare(const Kernel* k)
{
}
