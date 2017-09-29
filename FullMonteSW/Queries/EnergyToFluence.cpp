/*
 * FluenceConverter.cpp
 *
 *  Created on: Mar 6, 2016
 *      Author: jcassidy
 */


#include <FullMonteSW/OutputTypes/SpatialMap.hpp>

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include "EnergyToFluence.hpp"

#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/MaterialSet.hpp>
#include <FullMonteSW/Geometry/Material.hpp>

#include <FullMonteSW/Geometry/Units/BaseUnit.hpp>

class EnergyToFluence::Converter
{
public:
	virtual ~Converter(){}
	virtual float operator()(std::size_t i,float x) const=0;
	//virtual double operator()(std::size_t i,double x) const=0;
};

class EnergyToFluence::SurfaceConverter : public EnergyToFluence::Converter
{
public:
	SurfaceConverter(const TetraMesh* M) : m_mesh(M){}

	virtual float operator()(std::size_t i,float x) const override
	{
		return x / get(area,*m_mesh,TetraMesh::FaceDescriptor(i));
	}

protected:
	const TetraMesh* m_mesh=nullptr;
};

class EnergyToFluence::DirectedSurfaceConverter : public EnergyToFluence::Converter
{
public:
	DirectedSurfaceConverter(const TetraMesh* M) : m_mesh(M){}

	virtual float operator()(std::size_t i,float x) const override
			{
		return x / get(area,*m_mesh,TetraMesh::DirectedFaceDescriptor(i));
			}
protected:
	const TetraMesh* m_mesh=nullptr;
};

class EnergyToFluence::VolumeConverter : public EnergyToFluence::Converter
{
public:
	VolumeConverter(const TetraMesh* M,const MaterialSet* mat) :
		m_mesh(M),
		m_materials(mat){}

	virtual float operator()(std::size_t i,float x) const override
	{
		float V 		= get(volume,*m_mesh,TetraMesh::TetraDescriptor(i));
		unsigned matID 	= get(region,*m_mesh,TetraMesh::TetraDescriptor(i));
		float muA 		= m_materials->get(matID)->absorptionCoeff();
		return x / (V*muA);
	}

private:
	const TetraMesh* m_mesh=nullptr;
	const MaterialSet* m_materials=nullptr;
};



EnergyToFluence::EnergyToFluence()
{
}

EnergyToFluence::~EnergyToFluence()
{
}

void EnergyToFluence::materials(const MaterialSet* MS)
{
	m_materials=MS;
}

void EnergyToFluence::source(OutputData* M)
{
	m_values=dynamic_cast<AbstractSpatialMap*>(M);
	if (!m_values)
		cout << "ERROR: EnergyToFluence::source(OutputData* M) input cannot be cast to AbstractSpatialMap" << endl;
	else
		cout << "INFO: EnergyToFluence::source(OutputData*) updated source with dim=" << m_values->dim() << endl;
}

const AbstractSpatialMap* EnergyToFluence::source() const
{
	return m_values;
}

void EnergyToFluence::update()
{
	Converter* C=nullptr;
	delete m_output;
	m_output=nullptr;

	if (!m_values)
	{
		cout << "ERROR: EnergyToFluence::results() requested but no input values" << endl;
		return;
	}

	if (!m_mesh)
	{
		cout << "ERROR: EnergyToFluence::results() requested but no attached mesh" << endl;
		return;
	}

	switch(m_values->spatialType())
	{
	case AbstractSpatialMap::Surface:
		if (dynamic_cast<const SpatialMap<float>*>(m_values))
			C = new SurfaceConverter(m_mesh);
		break;

	case AbstractSpatialMap::DirectedSurface:
		if (dynamic_cast<const SpatialMap<float>*>(m_values))
			C = new DirectedSurfaceConverter(m_mesh);
		break;

	case AbstractSpatialMap::Volume:
		if (!m_mesh)
		{
			cout << "ERROR: EnergyToFluence::results() requested but no attached mesh" << endl;
			return;
		}
		if (!m_materials)
		{
			cout << "ERROR: EnergyToFluence::results() requested but no material information provided" << endl;
			return;
		}
        if (!dynamic_cast<const SpatialMap<float>*>(m_values))
        {
            cout << "ERROR: EnergyToFluence::results() requested but input type can't be cast to SpatialMap<float>*" << endl;
            return;
        }
		C = new VolumeConverter(m_mesh,m_materials);
		break;

	default:
		cout << "ERROR: EnergyToFluence::results() requested but input is neither a surface nor a volume map" << endl;
		return;
	}

	if (!C)
		return;

	// Clone input to get same dimensions and type
	m_output = static_cast<AbstractSpatialMap*>(m_values->clone());
	m_output->quantity(&Units::fluence);

	if (auto mf = dynamic_cast<const SpatialMap<float>*>(m_values))
	{
		for(unsigned i=0;i<m_values->dim();++i)
			static_cast<SpatialMap<float>&>(*m_output)[i] = (*C)(i,(*mf)[i]);
	}
	else if (auto md = dynamic_cast<const SpatialMap<double>*>(m_values))
	{
		for(unsigned i=0;i<m_values->dim();++i)
			static_cast<SpatialMap<double>&>(*m_output)[i] = (*C)(i,(*md)[i]);
	}
	else
		cout << "ERROR: EnergyToFluence::results() requested but input array type is not recognized" << endl;

	delete C;
}

OutputData* EnergyToFluence::result() const
{
	if (m_output)
		return m_output;

	cout << "ERROR! Returning a null pointer from EnergyToFluence::result()" << endl;
	return nullptr;
}

