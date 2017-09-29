/*
 * TIMOSMaterialReader.hpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TIMOS_TIMOSMATERIALREADER_HPP_
#define STORAGE_TIMOS_TIMOSMATERIALREADER_HPP_

#include "TextFileReader.hpp"

#include "TIMOSMaterial.hpp"

class MaterialSet;

class TIMOSMaterialReader :
		public TextFileReader
{
public:
	TIMOSMaterialReader();
	~TIMOSMaterialReader();

	void read();

	MaterialSet*	materials() const;

private:
	TIMOS::Optical	m_optical;
};

#endif /* STORAGE_TIMOS_TIMOSMATERIALREADER_HPP_ */
