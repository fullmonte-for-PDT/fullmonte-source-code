/*
 * TIMOSSource.hpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TIMOS_TIMOSSOURCE_HPP_
#define STORAGE_TIMOS_TIMOSSOURCE_HPP_

#include <array>

namespace Source { class Abstract; }

namespace TIMOS
{


struct PointSource
{
	std::array<float,3> pos;
};

struct TetraSource
{
	 unsigned tetID;
};

struct PencilBeamSource
{
	std::array<float,3> pos;
	std::array<float,3> dir;
	unsigned tetID;
};

struct FaceSource
{
	std::array<unsigned,3>	IDps;
};

struct PointSource;
struct TetraSource;
struct PencilBeamSource;
struct FaceSource;

struct GenericSource
{
	unsigned type=0;
	unsigned w=0;

	enum Type { Point=1, Volume=2, PencilBeam=11, Face=12 };

	union {
		PointSource			point;
		TetraSource			tetra;
		PencilBeamSource 	pencil;
		FaceSource			face;
	} details;

};


	//friend std::ostream& operator<<(std::ostream& os,const SourceDef& s);
}

#endif /* STORAGE_TIMOS_TIMOSSOURCE_HPP_ */
