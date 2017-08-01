/*
 * MCMLOutputReader.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#include <iostream>


#include <FullMonteSW/OutputTypes/OutputDataCollection.hpp>
#include <FullMonteSW/OutputTypes/SpatialMap2D.hpp>

#include "MCMLOutputReader.hpp"

#include <sstream>
#include <tuple>

#include <FullMonteSW/Storage/TextFile/Util.hpp>

using namespace std;

MCMLOutputReader::MCMLOutputReader()
{
}

MCMLOutputReader::~MCMLOutputReader()
{
}

void MCMLOutputReader::filename(std::string fn)
{
	m_filename=fn;
}

vector<float> MCMLOutputReader::readVector(istream& is,unsigned N)
{
	vector<float> v(N,numeric_limits<float>::quiet_NaN());

	for(unsigned i=0;i<N && !is.eof() && !is.fail();++i)
		is >> v[i];
	return v;
}

void MCMLOutputReader::clear()
{
	// reset everything but filename
	string fn = m_filename;
	*this = MCMLOutputReader();
	m_filename = fn;
}

void MCMLOutputReader::read()
{
	clear();

	char *bufStart, *bufEnd;

	tie(bufStart,bufEnd) = slurp(m_filename);
	bufEnd = stripEndOfLineComments(make_pair(bufStart,bufEnd),'#');
	bufEnd = compressWhitespace(make_pair(bufStart,bufEnd));

	stringstream ss(string(bufStart,bufEnd-bufStart));

	delete[] bufStart;

	string ver;

	m_Npkt=0;

	ss >> ver;

	string section;

	ss >> section;

	string fn;
	char fmt;

	ss >> fn >> fmt;
	ss >> m_Npkt;
	ss >> m_dz >> m_dr;
	ss >> m_nz >> m_nr >> m_na;
	ss >> m_nl;

	float nAbove, nBelow;

	ss >> nAbove;

	for(unsigned i=0;i<m_nl;++i)
	{
		float n,g,muA,muS,t;
		ss >> n >> g >> muA >> muS >> t;
	}

	ss >> nBelow;


	for (ss >> section; !ss.eof(); ss>>section)
	{
		if (section == "RAT")
		{
			ss >> m_specularReflectance;
			ss >> m_diffuseReflectance;
			ss >> m_absorption;
			ss >> m_transmission;
		}
		else if (section == "A_l")
			m_absorptionByLayer = readVector(ss,m_nl);
		else if (section == "A_z")
			m_absorptionByDepth = readVector(ss,m_nz);
		else if (section == "Rd_r")
			m_diffuseReflectanceByRadius = readVector(ss,m_nr);
		else if (section == "Rd_a")
			m_diffuseReflectanceByAngle = readVector(ss,m_na);
		else if (section == "Tt_r")
			m_transmissionByRadius = readVector(ss,m_nr);
		else if (section == "Tt_a")
			m_transmissionByAngle = readVector(ss,m_na);
		else if (section == "A_rz")
			m_absorptionByRadiusAndDepth = readVector(ss,m_nr*m_nz);
		else if (section == "Rd_ra")
			m_diffuseReflectanceByRadiusAndAngle = readVector(ss,m_na*m_nr);
		else if (section == "Tt_ra")
			m_transmissionByRadiusAndAngle = readVector(ss,m_na*m_nr);
	}
}


void MCMLOutputReader::createResultVector(OutputDataCollection* C,const vector<float>& v, size_t dim,const char* name)
{
	if (v.size() > 0)
	{
		if (v.size() != dim)
		{
			stringstream ss;
			ss << "MCMLOutputReader::result() dimension mismatch in 1D vector " << name;
			throw std::logic_error(ss.str());
		}
		SpatialMap<float> *d = new SpatialMap<float>(v,AbstractSpatialMap::Volume,AbstractSpatialMap::Scalar);
		d->name(name);
		C->add(d);
	}
}

OutputDataCollection* MCMLOutputReader::result() const
{
	OutputDataCollection *C = new OutputDataCollection();

	if (m_absorptionByRadiusAndDepth.size() > 0)
	{
		SpatialMap2D<float> *absorptionmap = new SpatialMap2D<float>(m_nr,m_nz,m_absorptionByRadiusAndDepth, AbstractSpatialMap::Volume, AbstractSpatialMap::Scalar);
		absorptionmap->name("Absorption_rz");
		C->add(absorptionmap);
	}

	if (m_diffuseReflectanceByRadiusAndAngle.size() > 0)
	{
		SpatialMap2D<float> *diffusereflectancemap = new SpatialMap2D<float>(m_nr,m_na,m_diffuseReflectanceByRadiusAndAngle, AbstractSpatialMap::Volume, AbstractSpatialMap::Scalar);
		diffusereflectancemap->name("DiffuseReflectance_ra");
		C->add(diffusereflectancemap);
	}

	if (m_transmissionByRadiusAndAngle.size() > 0)
	{
		SpatialMap2D<float> *transmissionmap = new SpatialMap2D<float>(m_nr,m_na,m_transmissionByRadiusAndAngle, AbstractSpatialMap::Volume, AbstractSpatialMap::Scalar);
		transmissionmap->name("Transmission_ra");
		C->add(transmissionmap);
	}

	createResultVector(C,m_transmissionByAngle,m_na,"Transmission_a");
	createResultVector(C,m_transmissionByRadius,m_nr,"Transmission_r");
	createResultVector(C,m_diffuseReflectanceByAngle,m_na,"Reflectance_a");
	createResultVector(C,m_diffuseReflectanceByRadius,m_nr,"Reflectance_r");
	createResultVector(C,m_absorptionByDepth,m_nz,"Absorption_z");

	// absorption by layer
	return C;
}
