/*
 * Layered.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#include "Layered.hpp"

#include <boost/math/constants/constants.hpp>

using namespace std;

Layered::Layered()
{
}

Layered::~Layered()
{
}

void Layered::resolution(float dr,float dz)
{
	m_dr=dr;
	m_dz=dz;
}

CylCoord Layered::resolution() const
{
	return CylCoord { m_dr, m_dz };
}

CylIndex Layered::dims() const
{
	return CylIndex { m_Nr, m_Nz };
}

void Layered::dims(unsigned Nr,unsigned Nz)
{
	m_Nr = Nr;
	m_Nz = Nz;
}

void Layered::extent(float R,float Z)
{
	m_dr = R/float(m_Nr);
	m_dz = Z/float(m_Nz);
}

CylCoord Layered::extent() const
{
	return { m_dr*float(m_Nr), m_dz*float(m_Nz) };
}

unsigned Layered::layerCount() const
{
	return m_layers.size();
}

Layer* Layered::layer(unsigned i) const
{
	if (i==0)
		throw std::out_of_range("Layered::layer(i) starts at i=1");
	return m_layers.at(i-1);
}

void Layered::addLayer(Layer* l)
{
	m_layers.push_back(l);
}

void Layered::angularBins(unsigned Na)
{
	m_Na = Na;
}

unsigned Layered::angularBins() const
{
	return m_Na;
}

float Layered::angularResolution() const
{
	return boost::math::constants::pi<float>()/float(m_Na);
}


/** Compute the 1D index for a 2D bin.
 *
 * ri = 1..Nr
 * zi = 1..Nz
 *
 * i = 1..Nr*Nz (0 is special not-a-bin)
 */

unsigned			Layered::indexForBin(CylIndex ci) const
{
	if (ci.zi == 0U || ci.ri == 0U)
		return 0U;
	else
		return (ci.zi-1)*m_Nr + ci.ri;
}

CylIndex	Layered::binFromIndex(unsigned i) const
{
	if (i == 0)
		return CylIndex {0U, 0U};
	else
		return CylIndex {
			((i-1) % m_Nr) + 1,
			((i-1) / m_Nr) + 1
		};
}

float Layered::binVolume(unsigned i) const
{
	CylIndex bin = binFromIndex(i);
	return binVolume(bin);
}

float Layered::binVolume(CylIndex ci) const
{
	// radial & depth bins start at 1, so ri*dr is the outer diameter and (ri-1)*dr is inner
	// pi * (ro^2 - (ro-dr)^2) * dz
	return boost::math::constants::pi<float>()*m_dz*m_dr*m_dr*float(2*ci.ri-1);
}

std::ostream& operator<<(std::ostream& os,CylIndex ci)
{
	const auto w = os.width();
	return os << '[' << ci.ri << "][" << setw(w) << ci.zi << ']';
}

