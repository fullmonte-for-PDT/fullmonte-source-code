/*
 * MCMLCase.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#include "MCMLCase.hpp"

#include <boost/math/constants/constants.hpp>

using namespace std;

MCMLCase::MCMLCase()
{
}

MCMLCase::~MCMLCase()
{
}

void MCMLCase::packets(unsigned long long N)
{
	m_packetCount = N;
}

unsigned long long MCMLCase::packets() const
{
	return m_packetCount;
}

char MCMLCase::outputFormat() const
{
	return m_format;
}

Layered* MCMLCase::geometry() const
{
	return m_geometry;
}

void MCMLCase::geometry(Layered *L)
{
	m_geometry=L;
}

void MCMLCase::materials(MaterialSet* M)
{
	m_materials=M;
}

MaterialSet* MCMLCase::materials() const
{
	return m_materials;
}

void MCMLCase::outputFilename(std::string fn,char format)
{
	m_outputFilename=fn;
	m_format=format;
}

const std::string& MCMLCase::outputFilename() const
{
	return m_outputFilename;
}
//
//void MCMLCase::resolution(float dr,float dz)
//{
//	m_resolution = array<float,2>{{dr,dz}};
//}
//
//array<float,2> MCMLCase::resolution() const
//{
//	return m_resolution;
//}
//
//void MCMLCase::dims(unsigned Nr,unsigned Nz)
//{
//	m_dims = array<unsigned,2>{{Nr,Nz}};
//}
//
//array<unsigned,2> MCMLCase::dims() const
//{
//	return m_dims;
//}

float MCMLCase::rouletteThreshold() const
{
	return m_rouletteThreshold;
}

void MCMLCase::rouletteThreshold(float th)
{
	m_rouletteThreshold=th;
}
//
//void MCMLCase::angularBins(unsigned Na)
//{
//	m_angularBins=Na;
//}
//
//unsigned MCMLCase::angularBins() const
//{
//	return m_angularBins;
//}
//
//float MCMLCase::angularResolution() const
//{
//	return boost::math::constants::half_pi<double>();
//}
