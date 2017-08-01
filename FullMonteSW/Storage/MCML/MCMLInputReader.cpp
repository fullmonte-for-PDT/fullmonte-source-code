/*
 * MCMLInputReader.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#include "MCMLInputReader.hpp"
#include <iostream>
#include <fstream>

#include "MCMLCase.hpp"

#include <FullMonteSW/Geometry/Layered.hpp>
#include <FullMonteSW/Geometry/Layer.hpp>
#include <FullMonteSW/Geometry/Material.hpp>
#include <FullMonteSW/Geometry/MaterialSet.hpp>

#include <stdexcept>

extern "C" {
#include <FullMonteSW/External/src-mcml/mcml.h>

//void CheckParm(FILE* f,InputStruct* i);
short ReadNumRuns(FILE*);
void ReadParm(FILE* f,InputStruct* i);
}

#include <cstdio>

using namespace std;

MCMLInputReader::MCMLInputReader()
{
}

MCMLInputReader::~MCMLInputReader()
{
}

void print(InputStruct* is,std::ostream& os)
{
	cout << "Output filename: " << is->out_fname << "  format '" << is->out_fformat << "'" << endl;
	cout << "Photons: " << is->num_photons << endl;

	cout << "Roulette threshold: " << is->Wth << endl;

	cout << "dz=" << is->dz << " dr=" << is->dr << " da=" << is->da << endl;
	cout << "nz=" << is->nz << " nr=" << is->nr << " na=" << is->na << endl;
	cout << "Layers: " << is->num_layers << endl;
	for(int i=0;i<=is->num_layers;++i)
		cout << "  z=(" << is->layerspecs[i].z0 << "," << is->layerspecs[i].z1 << ")" <<
			"mua=" << is->layerspecs[i].mua << " mus=" << is->layerspecs[i].mus << " g=" << is->layerspecs[i].g << " n=" << is->layerspecs[i].n << endl;

}

void MCMLInputReader::read()
{
	FILE* i = fopen(m_filename.c_str(),"r");
	InputStruct is;

	if (!i)
		throw std::logic_error("MCMLInputReader::read() failed to open file");

	short Nr = ReadNumRuns(i);

	for(int r=0;r<Nr;++r)
	{
		ReadParm(i,&is);
		if (m_verbose)
			print(&is,cout);

		MCMLCase* c = new MCMLCase();

		Layered* L = new Layered();
		c->geometry(L);
		c->outputFilename(is.out_fname, is.out_fformat);
		c->packets(is.num_photons);
		c->rouletteThreshold(is.Wth);

//		c->resolution(is.dr,is.dz);
		L->resolution(is.dr,is.dz);

		//c->dims(is.nr,is.nz);
		L->dims(is.nr,is.nz);

		//c->angularBins(is.na);
		L->angularBins(is.na);

		MaterialSet* M = new MaterialSet();

		// layer 0 is top exterior
		M->append(new Material(0.0f,0.0f,0.0f,is.layerspecs[0].n));

		for(int l=1;l<=is.num_layers;++l)
		{
			Layer* lp = new Layer();
			lp->thickness(is.layerspecs[l].z1-is.layerspecs[l].z0);
			L->addLayer(lp);

			Material* m = new Material(is.layerspecs[l].mua, is.layerspecs[l].mus, is.layerspecs[l].g, is.layerspecs[l].n);
			M->append(m);
		}

		// layer Nl+1 is bottom exterior
		M->append(new Material(0.0f,0.0f,0.0f,is.layerspecs[is.num_layers+1].n));

		c->materials(M);

		m_cases.push_back(c);
	}

	free(is.layerspecs);


	// print out

	if(m_verbose)
	{

		for(start(); !done(); next())
		{
			MCMLCase* c = current();
			Layered* L = c->geometry();

			CylCoord resolution = L->resolution();
			CylIndex dims = L->dims();

			cout << "Packets: " << c->packets() << endl;
			cout << "Resolution dr=" << resolution.r << " dz=" << resolution.z << " da=" << L->angularResolution() << endl;
			cout << "Bins:      Nr=" << dims.ri << " Nz=" << dims.zi << " Na=" << L->angularBins() << endl;
			cout << "Output file: " << c->outputFilename() << " format '" << c->outputFormat() << "'" << endl;
			cout << "Layers (" << c->geometry()->layerCount() << ")" << endl;
			for(unsigned l=1;l<=c->geometry()->layerCount();++l)
			{
				Material* m = c->materials()->get(l+1);
				cout << "  thickness " << c->geometry()->layer(l)->thickness() << " mua=" << m->absorptionCoeff() << " mus=" << m->scatteringCoeff() << " g=" << m->anisotropy() << " n=" << m->refractiveIndex() << endl;
			}
		}
	}

	fclose(i);
}

void MCMLInputReader::filename(std::string fn)
{
	m_filename=fn;
}

void MCMLInputReader::start()
{
	m_iterator = m_cases.begin();
}

void MCMLInputReader::next()
{
	if (m_iterator != m_cases.end())
		m_iterator++;
}

bool MCMLInputReader::done() const
{
	return m_iterator == m_cases.end();
}

MCMLCase* MCMLInputReader::current() const
{
	return m_iterator != m_cases.end() ? *m_iterator : nullptr;
}

MCMLCase* MCMLInputReader::get(unsigned i) const
{
	list<MCMLCase*>::const_iterator it=m_cases.begin();
	for(unsigned j=0;j<i && it != m_cases.end();++j)
		++it;

	return it == m_cases.end() ? nullptr : *it;
}

unsigned MCMLInputReader::cases() const
		{
	return m_cases.size();
		}

