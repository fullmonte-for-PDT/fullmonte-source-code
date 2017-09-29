/*
 * MCMLOutputReader.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_MCML_MCMLOUTPUTREADER_HPP_
#define STORAGE_MCML_MCMLOUTPUTREADER_HPP_

#include <string>
#include <vector>
#include <limits>

class OutputDataCollection;

/** Reads data from an MCML .mco file and generates FullMonte OutputData structures for it.
 *
 */

class MCMLOutputReader
{
public:
	MCMLOutputReader();
	~MCMLOutputReader();

	void filename(std::string fn);
	void read();

	void clear();

	OutputDataCollection*	result() const;

private:
	std::vector<float> readVector(std::istream& is,unsigned N);
	static void createResultVector(OutputDataCollection* C,const std::vector<float>&, size_t dim,const char* name);

	std::string				m_filename;


	/** For the 2D fields below, they are stored by least significant index first,
	 * ie. A_rz reads out as A[r=0][z=0] A[r=0][z=1] ...
	 */

	float					m_absorption=std::numeric_limits<float>::quiet_NaN();
	std::vector<float>		m_absorptionByLayer;
	std::vector<float>		m_absorptionByDepth;
	std::vector<float>		m_absorptionByRadiusAndDepth;		// A_rz

	float					m_transmission=std::numeric_limits<float>::quiet_NaN();
	std::vector<float>		m_transmissionByRadius;
	std::vector<float>		m_transmissionByAngle;
	std::vector<float>		m_transmissionByRadiusAndAngle;		// R_ra

	float					m_specularReflectance=std::numeric_limits<float>::quiet_NaN();
	float					m_diffuseReflectance=std::numeric_limits<float>::quiet_NaN();
	std::vector<float>		m_diffuseReflectanceByRadius;
	std::vector<float>		m_diffuseReflectanceByAngle;
	std::vector<float>		m_diffuseReflectanceByRadiusAndAngle;	//R_ra

	float					m_dz=0.0f, m_dr=0.0f;
	unsigned				m_nz=0U, m_nr=0U, m_na=0U, m_nl=0U;
	unsigned long long		m_Npkt=0ULL;
};

#endif /* STORAGE_MCML_MCMLOUTPUTREADER_HPP_ */
