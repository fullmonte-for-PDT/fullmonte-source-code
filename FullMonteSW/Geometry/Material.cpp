/*
 * Material.cpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#include "Material.hpp"

Material::Material()
{

}

Material::Material(float muA,float muS,float g_,float n_) :
		mu_s(muS), mu_a(muA), g(g_), n(n_)
{
}

Material::~Material()
{

}

void Material::reducedScatteringCoeff(float mu_s_prime_)
{
	mu_s = mu_s_prime_/(1-g);
}

void Material::reducedScatteringCoeffWithG(float mu_s_prime_,float g_)
{
	g = g_;
	reducedScatteringCoeff(mu_s_prime_);
}

float Material::reducedScatteringCoeff() const
{
	return (1-g)*mu_s;
}

float Material::scatteringCoeff() const
{
	return mu_s;
}

void Material::scatteringCoeff(float mu_s_)
{
	mu_s = mu_s_;
}

float Material::absorptionCoeff() const
{
	return mu_a;
}

void Material::absorptionCoeff(float mu_a_)
{
	mu_a = mu_a_;
}

void Material::anisotropy(float g_)
{
	g = g_;
}

float Material::anisotropy() const
{
	return g;
}

float Material::refractiveIndex() const
{
	return n;
}

void Material::refractiveIndex(float n_)
{
	n = n_;
}
