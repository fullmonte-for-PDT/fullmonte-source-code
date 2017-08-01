/*
 * Material.hpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_MATERIAL_HPP_
#define GEOMETRY_MATERIAL_HPP_


/** Very simple material description
 *
 */

class Material
{
public:
	Material();
	Material(float muA,float muS,float g,float n);
	~Material();


	/// Sets the scattering coefficient such that the reduced scatter (1-g)mu_s equals the provided value using the existing g
	void reducedScatteringCoeff(float mu_s_prime);

	/// Sets the scattering coefficient and shape parameter to match the specified reduced scattering coefficient
	void reducedScatteringCoeffWithG(float mu_s_prime,float g);

	float reducedScatteringCoeff() const;

	/// Get/set the scattering coefficient
	void scatteringCoeff(float mu_s);
	float scatteringCoeff() const;

	/// Get/set the absorption coefficient
	void absorptionCoeff(float mu_a);
	float absorptionCoeff() const;

	/// Get/set the Henyey-Greenstein G parameter
	void anisotropy(float g);
	float anisotropy()			const;

	/// Get/set the refractive index
	void refractiveIndex(float n);
	float refractiveIndex()		const;

//private:

	float 	muS=0.0f;
	float 	muA=0.0f;
	float	g=0.0f;
	float	n=0.0f;
};




#endif /* GEOMETRY_MATERIAL_HPP_ */
