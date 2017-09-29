/*
 * Material.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_MATERIAL_HPP_
#define KERNELS_SOFTWARE_MATERIAL_HPP_

#include <immintrin.h>

namespace x86Kernel
{

class Material
{
public:
	Material();
	Material(float muA,float muS,float n);
	Material(const Material&)=default;

	/// Change per unit physical step in: [0] physical step remaining, [1] dimensionless step remaining, [2] time elapsed, [3] X
	__m128 m_prop=_mm_setzero_ps();

	/// Initial value of propagation vector for unit dimensionless step length
	__m128 m_init=_mm_setzero_ps();

	static constexpr float c0 = 299.792458;		///< Speed of light in mm/ns

	float absfrac=0.0f;		///< Fraction absorbed at each interaction
	bool scatters=false;	///< Flag to indicate if the material scatters light or not

	float n=1.0f;
	float muT=0.0f;
};

};

#endif /* KERNELS_SOFTWARE_MATERIAL_HPP_ */
