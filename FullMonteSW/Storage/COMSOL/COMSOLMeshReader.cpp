/*
 * COMSOLMeshReader.cpp
 *
 *  Created on: July 24, 2017
 *      Author: Siyun Li
 */

#include "COMSOLMeshReader.hpp"

#include <FullMonteSW/Geometry/PTTetraMeshBuilder.hpp>
#include <cassert>
#include <vector>

#include <FullMonteSW/Geometry/Sources/Point.hpp>
#include <FullMonteSW/Geometry/Sources/SurfaceTri.hpp>
#include <FullMonteSW/Geometry/Sources/PencilBeam.hpp>
#include <FullMonteSW/Geometry/Sources/Volume.hpp>
#include <FullMonteSW/Geometry/Sources/Composite.hpp>

#include <iostream>
#include <fstream>

using namespace std;

COMSOLMeshReader::COMSOLMeshReader()
{
	m_builder = new PTTetraMeshBuilder();
}

COMSOLMeshReader::~COMSOLMeshReader()
{
	delete m_builder;
}

void COMSOLMeshReader::filename(std::string fn)
{
	m_filename=fn;
}

PTTetraMeshBuilder* COMSOLMeshReader::builder() const
{
	return m_builder;
}

void COMSOLMeshReader::read()
{
	unsigned Np=0,Nt=0;
	
	ifstream inFile;
	inFile.open(m_filename.c_str());

	if (!inFile)
	{
		cout << "Cannot open input file" << endl;
		return;
	}
	
        string line;
	getline(inFile, line);

	while (line.find("# number of mesh points") == string::npos)
		getline(inFile, line);

	stringstream ss(line);
	ss >> Np;
	while (line.find("# Mesh point coordinates") == string::npos)
		getline(inFile, line);

	// read the point coordinates
	m_builder->setNumberOfPoints(Np+1);
	m_builder->setPoint(0,array<double,3>{{.0,.0,.0}});

	for(unsigned i=1;i<=Np && line != "\n";++i)
	{
		getline(inFile, line);
		array<double,3> p;
		stringstream subline(line);
		subline >> p[0] >> p[1] >> p[2];
		m_builder->setPoint(i,p);
	}

	while (line.find("tet # type name") == string::npos)
		getline(inFile, line);

	while (line.find("# number of elements") == string::npos)
		getline(inFile, line);

	stringstream ss2(line);
	ss2 >> Nt;

	getline(inFile, line);

	// read the tetra references
	m_builder->setNumberOfTetras(Nt+1);
	m_builder->setTetra(0,array<unsigned,4>{{0,0,0,0}});

	for(unsigned i=1;i<=Nt && line != "\n";++i)
	{
		getline(inFile, line);
		array<unsigned,4> IDps;
// This should store the region data, but it is not available from the COMSOL file given
//		unsigned IDr;
		stringstream subline(line);
		subline >> IDps[0] >> IDps[1] >> IDps[2] >> IDps[3];
		for (unsigned j = 0; j < 4; ++j)
			++IDps[j];
		m_builder->setTetra(i,IDps);
	}
}

TetraMesh* COMSOLMeshReader::mesh() const
{
	m_builder->build();
	return m_builder->mesh();
}

