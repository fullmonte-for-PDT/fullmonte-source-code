/*
 * TIMOSMaterialReader.cpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#include "TIMOSMaterialReader.hpp"
#include <iostream>
#include <fstream>

#include <FullMonteSW/take_drop.hpp>

#include <FullMonteSW/Geometry/MaterialSet.hpp>
#include <FullMonteSW/Geometry/Material.hpp>

#include <boost/range/adaptor/indexed.hpp>

using namespace std;

TIMOSMaterialReader::TIMOSMaterialReader()
{
}

TIMOSMaterialReader::~TIMOSMaterialReader()
{
}

void TIMOSMaterialReader::read()
{
	std::ifstream is(filename().c_str());

	unsigned materialRegionType;
	unsigned Nm=0;

	m_optical.materials.clear();
	m_optical.per_region=false;

	is >> materialRegionType >> Nm;

	if (materialRegionType == 1)			// per-region properties OK
	{
		m_optical.materials.resize(Nm+1);
	}
	else if (materialRegionType == 2)
	{
		m_optical.per_region=true;
		cout << "ERROR: TIMOSMaterialReader::read() region type 2 (per-tetra properties) is not supported" << endl;
		return;
	}

	m_optical.materials[0].mu_a = 0.0f;
	m_optical.materials[0].mu_s = 0.0f;
	m_optical.materials[0].g	= 0.0f;
	m_optical.materials[0].n	= 1.0f;

	for(auto& m : m_optical.materials | drop(1))
	{
		is >> m.mu_a >> m.mu_s >> m.g >> m.n;
	}

	unsigned boundaryType;

	is >> boundaryType;
	if (boundaryType == 1)
	{
		m_optical.matched = false;
		is >> m_optical.n_ext;

		m_optical.materials[0].n = m_optical.n_ext;
	}
	else if (boundaryType == 2)
	{
		m_optical.matched = true;
		m_optical.materials[0].n = std::numeric_limits<float>::quiet_NaN();
		cout << "WARNING: TIMOSMaterialReader::read() - FullMonte kernel may not support matched boundaries correctly" << endl;
	}

	if (is.fail())
		cout << "ERROR: TIMOSMaterialReader::read() failure on input stream" << endl;
}

MaterialSet* TIMOSMaterialReader::materials() const
{
	MaterialSet* M = new MaterialSet();

	M->resize(m_optical.materials.size());
	M->matchedBoundary(m_optical.matched);

	if (m_optical.per_region)
		cout << "WARNING: TIMOSMaterialReader::materials() specifies per-tetra materials but that is not supported" << endl;

	for(const auto& m : m_optical.materials | boost::adaptors::indexed(0U))
	{
		Material* p = new Material();
		p->scatteringCoeff(m.value().mu_s);
		p->absorptionCoeff(m.value().mu_a);
		p->anisotropy(m.value().g);
		p->refractiveIndex(m.value().n);
		M->set(m.index(),p);
	}

	M->matchedBoundary(m_optical.matched);

	return M;
}


