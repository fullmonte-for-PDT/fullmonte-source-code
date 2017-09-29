/*
 * MCEventCounts.cpp
 *
 *  Created on: Feb 5, 2016
 *      Author: jcassidy
 */

#include "OutputDataType.hpp"
#include "MCEventCounts.hpp"
#include "OutputDataCollection.hpp"

MCEventCounts& MCEventCounts::operator+=(const MCEventCounts& rhs)
{
    Nlaunch     += rhs.Nlaunch;
    Nabsorb     += rhs.Nabsorb;
    Nscatter    += rhs.Nscatter;
    Nbound      += rhs.Nbound;
    Ntir        += rhs.Ntir;
    Nfresnel    += rhs.Nfresnel;
    Nrefr       += rhs.Nrefr;
    Ninterface  += rhs.Ninterface;
    Nexit       += rhs.Nexit;
    Ndie        += rhs.Ndie;
    Nwin        += rhs.Nwin;
    Nabnormal	+= rhs.Nabnormal;
    Ntime       += rhs.Ntime;
    Nnohit      += rhs.Nnohit;
    return *this;
}


const OutputDataType mcEventCountsType{
	"MCEventCounts"
};

const OutputDataType* MCEventCountsOutput::type() const
{
	return & mcEventCountsType;
}



//#include <boost/serialization/export.hpp>
//BOOST_CLASS_EXPORT_GUID(MCEventCountsOutput,"MCEventCountsOutput")
