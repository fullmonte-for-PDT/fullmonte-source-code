/*
 * MCEventCount.hpp
 *
 *  Created on: Feb 5, 2016
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_MCEVENTCOUNTS_HPP_
#define OUTPUTTYPES_MCEVENTCOUNTS_HPP_

class OutputData;
class OutputDataCollection;

struct MCEventCounts
{
    unsigned long long Nlaunch=0;
    unsigned long long Nabsorb=0;
    unsigned long long Nscatter=0;
    unsigned long long Nbound=0;
    unsigned long long Ntir=0;
    unsigned long long Nfresnel=0;
	unsigned long long Nrefr =0;
	unsigned long long Ninterface=0;
	unsigned long long Nexit=0;
	unsigned long long Ndie=0;
	unsigned long long Nwin=0;
	unsigned long long Nabnormal=0;
	unsigned long long Ntime=0;
	unsigned long long Nnohit=0;

	MCEventCounts& operator +=(const MCEventCounts&);

	void clear(){ *this = MCEventCounts(); }
};


#include "OutputData.hpp"


class MCEventCountsOutput :
		public OutputData,
		public MCEventCounts
{
public:
	MCEventCountsOutput(const MCEventCounts& C=MCEventCounts()) : MCEventCounts(C){}

	CLONE_METHOD(OutputData,MCEventCountsOutput)
	ACCEPT_VISITOR_METHOD(OutputData,MCEventCountsOutput)

	virtual const OutputDataType* type() const override;
};


#endif /* OUTPUTTYPES_MCEVENTCOUNTS_HPP_ */
