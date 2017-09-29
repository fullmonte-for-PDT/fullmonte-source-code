/*
 * TetrasFromLayers.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_TETRASFROMLAYERED_HPP_
#define KERNELS_SOFTWARE_TETRASFROMLAYERED_HPP_

#include <vector>
#include "Tetra.hpp"

/** Create kernel tetras from a layered geometry description
 *
 * Uses degenerate tetras (with 2 faces at infinity) that will never see a photon interaction
 */

class Layered;

class TetrasFromLayered
{
public:
	TetrasFromLayered();
	~TetrasFromLayered();

	void						layers(const Layered* L);
	void						update();

	const std::vector<Tetra>&	tetras() const;

private:
	const Layered*				m_layers=nullptr;
	std::vector<Tetra>			m_tetras;

};



#endif /* KERNELS_SOFTWARE_TETRASFROMLAYERED_HPP_ */
