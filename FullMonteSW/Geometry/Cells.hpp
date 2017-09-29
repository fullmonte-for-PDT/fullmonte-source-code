/*
 * Cells.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_CELLS_HPP_
#define GEOMETRY_CELLS_HPP_

#include "WrappedVector.hpp"

#include <array>

template<std::size_t N>class Cells : public WrappedVector<std::array<unsigned,N>,unsigned>
{
};

using FaceCells  = Cells<3>;
using TetraCells = Cells<4>;

#endif /* GEOMETRY_CELLS_HPP_ */
