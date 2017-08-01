/*
 * TIMOSSourceReader.cpp
 *
 *  Created on: May 29, 2017
 *      Author: jcassidy
 */

#include "TIMOSSourceReader.hpp"

#include <fstream>
#include <iostream>

#include <FullMonteSW/Geometry/Sources/Composite.hpp>
#include <FullMonteSW/Geometry/Sources/Volume.hpp>
#include <FullMonteSW/Geometry/Sources/PencilBeam.hpp>
#include <FullMonteSW/Geometry/Sources/SurfaceTri.hpp>

using namespace std;

template<typename T,size_t N>istream& operator>>(istream& is,array<T,N>& a)
{
	for(unsigned i=0;i<N;++i)
		is >> a[i];
	return is;
}

TIMOSSourceReader::TIMOSSourceReader()
{

}

TIMOSSourceReader::~TIMOSSourceReader()
{

}

void TIMOSSourceReader::filename(std::string fn)
{
	m_filename=fn;
}

Source::Abstract* TIMOSSourceReader::source() const
{
	cout << "Converting " << m_sources.size() << " sources" << endl;
	if (m_sources.size() == 0)
		return nullptr;
	else if (m_sources.size() > 1)
	{
		Source::Composite* c = new Source::Composite();

		for(const auto s : m_sources)
			c->add(convertToSource(s));
		return c;
	}
	else
		return convertToSource(m_sources[0]);
}

Source::Abstract* TIMOSSourceReader::convertToSource(TIMOS::GenericSource gs)
{
	switch(gs.type)
	{
	case TIMOS::GenericSource::Face:
		return new Source::SurfaceTri(gs.w,gs.details.face.IDps);
	case TIMOS::GenericSource::Volume:
		return new Source::Volume(gs.w,gs.details.tetra.tetID);
	case TIMOS::GenericSource::Point:
		return new Source::Point(gs.w,gs.details.point.pos);
	case TIMOS::GenericSource::PencilBeam:
		return new Source::PencilBeam(gs.w,gs.details.pencil.pos,gs.details.pencil.dir);

	default:
		cout << "Unrecognized source type " << gs.type << endl;
		throw std::logic_error("TIMOSSourceReader::convertToSource() unrecognized source type");
	}
}

void TIMOSSourceReader::read()
{
	ifstream is(m_filename.c_str());

	if (!is.good())
	{
		cout << "TIMOSSourceReader::read() failed to open input file '" << m_filename << '\'' << endl;
		return;
	}
	unsigned Ns;

	is >> Ns;

	if (!is.fail() && !is.eof())
		m_sources.resize(Ns);
	else
		return;

	for(auto& s : m_sources)
		read(is,s);
}

void TIMOSSourceReader::read(istream& is,TIMOS::GenericSource& s)
{
	is >> s.type;

	switch(s.type)
	{
	case TIMOS::GenericSource::Face:
		read(is,s.details.face); break;
	case TIMOS::GenericSource::Volume:
		read(is,s.details.tetra); break;
	case TIMOS::GenericSource::PencilBeam:
		read(is,s.details.pencil); break;
	case TIMOS::GenericSource::Point:
		read(is,s.details.point); break;

	default:
		cout << "ERROR in TIMOSSourceReader::read() - unrecognized source type " << s.type << endl;
	}

	is >> s.w;
}


void TIMOSSourceReader::read(istream& is,TIMOS::PointSource& p)
{
	is >> p.pos;
}

void TIMOSSourceReader::read(istream& is,TIMOS::FaceSource& p)
{
	is >> p.IDps;
}

void TIMOSSourceReader::read(istream& is,TIMOS::PencilBeamSource& p)
{
	is >> p.tetID >> p.pos >> p.dir;
}

void TIMOSSourceReader::read(istream& is,TIMOS::TetraSource& p)
{
	is >> p.tetID;
}


