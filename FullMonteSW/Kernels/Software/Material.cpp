/*
 * TetraKernelBase.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: jcassidy
 */

#include <iomanip>
#include "Material.hpp"

using namespace std;

namespace x86Kernel
{

Material::Material()
{}

Material::Material(float muA_,float muS_,float n_)
{
	muT = muA_+muS_;
	n = n_;

	// propagation vector along unit physical step: time elapsed increases, dimensionless step decreases by mu_t, and physical step decreases by 1)
	m_prop = _mm_set_ps(0.0f,	n/c0,	-muT,	-1.0f);

	// initial propagation vector (1 physical/dimensionless step remaining, 0 time, 0 X)
	m_init = _mm_set_ps(0.0f,	0.0f,		1.0f,		1.0f/muT);

	absfrac = muA_/muT;
	scatters = muS_ != 0.0f;
}

};
