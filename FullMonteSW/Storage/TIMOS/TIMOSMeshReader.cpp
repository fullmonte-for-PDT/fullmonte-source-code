/*
 * TIMOS.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: jcassidy
 */

#include "TIMOSMeshReader.hpp"

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

TIMOSMeshReader::TIMOSMeshReader()
{
	m_builder = new PTTetraMeshBuilder();
}

TIMOSMeshReader::~TIMOSMeshReader()
{
	delete m_builder;
}

void TIMOSMeshReader::filename(std::string fn)
{
	m_filename=fn;
}

PTTetraMeshBuilder* TIMOSMeshReader::builder() const
{
	return m_builder;
}

void TIMOSMeshReader::read()
{
	unsigned Np=0,Nt=0;

	ifstream is(m_filename.c_str());

	is >> Np >> Nt;

	if (is.fail() || Np == 0 || Nt == 0)
	{
		cout << "Failed to read: input stream error, or zero points/tetras" << endl;
		return;
	}

	// read the point coordinates
	m_builder->setNumberOfPoints(Np+1);

	m_builder->setPoint(0,array<double,3>{{.0,.0,.0}});
	for(unsigned i=1;i<=Np && !is.fail() && !is.eof() ;++i)
	{
		array<double,3> p;

		for(unsigned j=0;j<3;++j)
			is >> p[j];

		m_builder->setPoint(i,p);
	}

	// read the tetra references
	m_builder->setNumberOfTetras(Nt+1);
	m_builder->setTetra(0,array<unsigned,4>{{0,0,0,0}},0);
	for(unsigned i=1;i<=Nt && !is.fail() && !is.eof();++i)
	{
		array<unsigned,4> IDps;
		unsigned IDr;

		for(unsigned j=0;j<4;++j)
			is >> IDps[j];
		is >> IDr;

		m_builder->setTetra(i,IDps,IDr);
	}

	if(is.fail())
		cout << "Failed to read!" << endl;
	else if (is.eof())
		cout << "End of file reached" << endl;
}

TetraMesh* TIMOSMeshReader::mesh() const
{
	m_builder->build();
	return m_builder->mesh();
}

//std::vector<SimpleMaterial> TIMOSMeshReader::materials_simple() const
//{
//	TIMOS::Optical opt = parse_optical(optFn_);
//	std::vector<SimpleMaterial> mat(opt.mat.size()+1);
//
//	assert(opt.by_region);
//	assert(!opt.matched);
//
//	mat[0] = SimpleMaterial(0.0f,0.0f,0.0f,opt.n_ext);
//
//	for(unsigned i=0; i<opt.mat.size(); ++i)
//		mat[i+1] = SimpleMaterial(opt.mat[i].mu_s, opt.mat[i].mu_a, opt.mat[i].g, opt.mat[i].n);
//
//	return mat;
//}

//
//
//Source::Abstract* TIMOSMeshReader::sources() const
//{
//	assert (!sourceFn_.empty() || !"No filename specified for TIMOSReader::sources");
//	std::vector<TIMOS::SourceDef> ts = parse_sources(sourceFn_);
//
//	std::vector<Source::Abstract*> src(ts.size(),nullptr);
//
//	boost::transform(ts,src.begin(),TIMOSMeshReader::convertToSource);
//
//	if (ts.size() > 1)
//		return new Source::Composite(1.0,std::move(src));
//	else
//		return src[0];
//}

