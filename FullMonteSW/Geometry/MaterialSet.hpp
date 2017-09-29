/*
 * MaterialSet.hpp
 *
 *  Created on: Sep 26, 2016
 *      Author: jcassidy
 */

#ifndef GEOMETRY_MATERIALSET_HPP_
#define GEOMETRY_MATERIALSET_HPP_

#include "WrappedVector.hpp"

class Material;

class MaterialSet : public WrappedVector<Material*,unsigned>
{
public:
	MaterialSet();
	~MaterialSet();

	/// Create a new material set, copying all materials from the original
	//MaterialSet*	clone() const;


	/// Alias for material(0) which represents the exterior of the mesh
	Material*		exterior();
	void			exterior(Material* m){ set(0,m); }

	Material*		material(unsigned i);

	/// Override the normal set function with automatic growth
	void			set(unsigned i,Material* m);

	/// Provide the ability to append a material to the set
	unsigned		append(Material* m);

	bool			matchedBoundary() const;
	void			matchedBoundary(bool m);

private:
	std::vector<Material*>		m_materials;
	bool						m_matched=false;
};

#endif /* GEOMETRY_MATERIALSET_HPP_ */
