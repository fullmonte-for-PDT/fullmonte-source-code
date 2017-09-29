/*
 * MCMLCase.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_MCML_MCMLCASE_HPP_
#define STORAGE_MCML_MCMLCASE_HPP_

#include <string>
#include <array>

class Layered;
class MaterialSet;

class MCMLCase
{
public:
	MCMLCase();
	~MCMLCase();

	void				outputFilename(std::string fn,char format);
	const std::string&	outputFilename() const;
	char				outputFormat() const;

	unsigned long long 	packets() const;
	void				packets(unsigned long long N);

	Layered*			geometry() const;
	void				geometry(Layered*);

	MaterialSet*		materials() const;
	void				materials(MaterialSet*);

	void				rouletteThreshold(float th);
	float				rouletteThreshold() const;

private:
	unsigned long long 	m_packetCount=0ULL;
	char				m_format='A';
	std::string			m_outputFilename="";

	float				m_rouletteThreshold=1e-5;

	MaterialSet*		m_materials=nullptr;
	Layered*			m_geometry=nullptr;
};


#endif /* STORAGE_MCML_MCMLCASE_HPP_ */
