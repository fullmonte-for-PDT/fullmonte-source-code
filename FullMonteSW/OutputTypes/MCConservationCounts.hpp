/*
 * MCConservationCounts.hpp
 *
 *  Created on: Feb 5, 2016
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_MCCONSERVATIONCOUNTS_HPP_
#define OUTPUTTYPES_MCCONSERVATIONCOUNTS_HPP_

class OutputData;
class OutputDataType;

struct MCConservationCounts
{
	MCConservationCounts(){}
	MCConservationCounts(const MCConservationCounts&) = default;

    double w_launch=0.0;			///< Amount of energy launched (generally 1.0 * Npackets)
    double w_absorb=0.0;			///< Amount of energy absorbed
    double w_die=0.0;				///< Amount of energy terminated in roulette
    double w_exit=0.0;				///< Amount of energy exiting
    double w_roulette=0.0;			///< Amount of energy added by winning roulette
    double w_abnormal=0.0;			///< Amount of energy terminated due to abnormal circumstances
    double w_time=0.0;				///< Amount of energy terminated due to time gate expiry
    double w_nohit=0.0;				///< Amount of energy terminated for failure to find an intersecting face

    /// Add another ConservationCounts
    MCConservationCounts& operator+=(const MCConservationCounts&);

    void clear(){ *this = MCConservationCounts(); }
};

#include "OutputData.hpp"

class MCConservationCountsOutput :
		public OutputData,
		public MCConservationCounts
{
public:
	MCConservationCountsOutput() : MCConservationCounts(){}
	MCConservationCountsOutput(const MCConservationCounts& C) : MCConservationCounts(C){}

	CLONE_METHOD(OutputData,MCConservationCountsOutput)
	ACCEPT_VISITOR_METHOD(OutputData,MCConservationCountsOutput)

	virtual const OutputDataType* type() const override;
};


#endif /* OUTPUTTYPES_MCCONSERVATIONCOUNTS_HPP_ */
