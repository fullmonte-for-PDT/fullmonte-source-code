/*
 * TIMOSMaterial.hpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TIMOS_TIMOSMATERIAL_HPP_
#define STORAGE_TIMOS_TIMOSMATERIAL_HPP_

#include <limits>
#include <vector>

namespace TIMOS
{

struct Material
{
	float mu_a=std::numeric_limits<float>::quiet_NaN();
	float mu_s=std::numeric_limits<float>::quiet_NaN();
	float g=std::numeric_limits<float>::quiet_NaN();
	float n=std::numeric_limits<float>::quiet_NaN();
};

struct Optical
{
	std::vector<Material> materials;
	bool matched=false;
	bool per_region=false;
	float n_ext=std::numeric_limits<float>::quiet_NaN();
};

}

#endif /* STORAGE_TIMOS_TIMOSMATERIAL_HPP_ */
