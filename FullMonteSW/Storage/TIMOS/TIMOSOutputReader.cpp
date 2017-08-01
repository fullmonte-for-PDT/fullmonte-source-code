/*
 * TIMOSOutputReader.cpp
 *
 *  Created on: Jun 2, 2017
 *      Author: jcassidy
 */

#include "TIMOSOutputReader.hpp"

#include <FullMonteSW/Storage/TextFile/Util.hpp>

#include <tuple>
#include <utility>
#include <sstream>
#include <iostream>

using namespace std;


TIMOSOutputReader::TIMOSOutputReader()
{

}

TIMOSOutputReader::~TIMOSOutputReader()
{
	clear();
}

void TIMOSOutputReader::clear()
{
	if (m_surfaceData)
	{
		delete[] m_surfaceData[0];
		delete[] m_surfaceData;
	}

	if (m_volumeData)
	{
		delete[] m_volumeData[0];
		delete[] m_volumeData;
	}

	m_faces.clear();
	m_tetras.clear();
	m_timeSteps=0;

	m_surfaceEnergy=0.0;
}

void TIMOSOutputReader::filename(std::string fn)
{
	m_filename=fn;
}

void TIMOSOutputReader::read()
{
	// slurp file, remove comments, and compress whitespace
	char* bufStart,*bufEnd;
	tie(bufStart,bufEnd) = slurp(m_filename);

	bufEnd = stripEndOfLineComments(make_pair(bufStart,bufEnd),'%');
	bufEnd = compressWhitespace(make_pair(bufStart,bufEnd));

	stringstream ss(string(bufStart,bufEnd-bufStart));

	delete[] bufStart;

	unsigned section,Ns,Nv,Nt;

	clear();


	while(!ss.eof() && !ss.fail())
	{
		ss >> section;

		m_surfaceEnergy=0.0;

		if (section == 1)
		{
			if (m_surfaceData)
				throw std::logic_error("invalid file format - duplicate surface data");

			ss >> Ns >> Nt;
			cout << Ns << " surface elements with " << Nt << " timesteps" << endl;

			m_surfaceData = new float*[Nt];
			m_surfaceData[0] = new float[Nt*Ns];

			for(unsigned i=1;i<Nt;++i)
				m_surfaceData[i] = m_surfaceData[0]+i*Nt;

			m_faces.resize(Ns+1);

			for(unsigned i=1;i<=Ns;++i)
			{
				for(unsigned j=0;j<3;++j)
					ss >> m_faces[i].IDps[j];
				ss >> m_faces[i].area;
				for(unsigned j=0;j<Nt;++j)
				{
					ss >> m_surfaceData[j][i];
					m_surfaceEnergy += m_surfaceData[j][i] * m_faces[i].area;
				}


			}
		}
		else if (section == 2)
		{
			if (m_volumeData)
				throw std::logic_error("invalid file format - duplicate volume data");

			ss >> Nv >> Nt;
			cout << Nv << " volume elements with " << Nt << " timesteps" << endl;

			m_volumeData = new float*[Nt];
			m_volumeData[0] = new float[Nt*Nv];

			for(unsigned i=1;i<Nt;++i)
				m_volumeData[i] = m_volumeData[0]+i*Nt;

			m_tetras.resize(Nv+1);

			for(unsigned i=1;i<=Nv;++i)
			{
				for(unsigned j=0;j<4;++j)
					ss >> m_tetras[i].IDps[j];
				ss >> m_tetras[i].volume;
				for(unsigned j=0;j<Nt;++j)
					ss >> m_volumeData[j][i];
			}
		}
	}

	cout << "Total surface energy: " << m_surfaceEnergy << endl;
}

unsigned TIMOSOutputReader::surfaceSize() const
{
	return m_faces.size();
}

unsigned TIMOSOutputReader::volumeSize() const
{
	return m_tetras.size();
}

unsigned TIMOSOutputReader::surfaceTimeSteps() const
{
	return m_timeSteps;
}

unsigned TIMOSOutputReader::volumeTimeSteps() const
{
	return m_timeSteps;
}

