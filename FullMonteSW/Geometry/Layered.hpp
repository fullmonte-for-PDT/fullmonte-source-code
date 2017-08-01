/*
 * Layered.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_LAYERED_HPP_
#define GEOMETRY_LAYERED_HPP_

#include "Geometry.hpp"
#include <array>
#include <vector>

class Layer;
class Materials;
class MaterialSet;

/** Layered geometry.
 *
 * Layers are oriented in the xy-plane, extending into -z.
 *
 * NOTE: Layers start at 1 (matching MCML convention), as do radial & depth bins
 *
 * Layer[1] runs (r,z) = 	(0,d1)
 * Layer[2] runs 			(d1,d1+d2) etc.
 */

struct CylIndex
{
	unsigned ri;
	unsigned zi;

	bool operator==(CylIndex rhs) const;
};

std::ostream& operator<<(std::ostream& os,CylIndex ci);

inline bool CylIndex::operator==(CylIndex rhs) const { return rhs.ri == ri && rhs.zi == zi; }


struct CylCoord
{
	float r;
	float z;
};

class Layered : public Geometry
{
public:
	Layered();
	virtual ~Layered();


	/// Get/set spatial domain
	void					resolution(float dr,float dz);
	CylCoord				resolution() const;

	void					extent(float R,float Z);
	CylCoord				extent() const;

	void					dims(unsigned Nr,unsigned Nz);
	CylIndex				dims() const;

	void					angularBins(unsigned Na);
	unsigned				angularBins() const;
	float					angularResolution() const;


	/// Query layers
	unsigned				layerCount() const;
	Layer*					layer(unsigned i) const;

	/// Add layers
	void					addLayer(Layer* l);

	CylIndex				binFromIndex(unsigned i) const;	///< Get 2D (ri,zi) bin for a linear index
	unsigned				indexForBin(CylIndex ci) const;	///< Get linear index 1..Nr*Nz for 2D index (ri,zi)

	float					binVolume(unsigned i)	const;	///< Size of i'th bin, with (r,z) row-major
	float					binVolume(CylIndex ci) 	const;	///< Size of bin (r,z)

private:
	void updateMaterials();

	unsigned				m_Na=1U;
	unsigned				m_Nr=0U;
	unsigned				m_Nz=0U;

	float					m_dz=0.0f;
	float					m_dr=0.0f;

	std::vector<Layer*>		m_layers;

	MaterialSet*			m_materials=nullptr;
};





#endif /* GEOMETRY_LAYERED_HPP_ */
