/*
 * TetrasFromLayers.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#include "TetrasFromLayered.hpp"
#include <FullMonteSW/Geometry/Layered.hpp>
#include <FullMonteSW/Geometry/Layer.hpp>

#include "Tetra.hpp"

#include <limits>

using namespace std;

TetrasFromLayered::TetrasFromLayered()
{

}

TetrasFromLayered::~TetrasFromLayered()
{

}

void TetrasFromLayered::layers(const Layered *L)
{
	m_layers=L;
}

void TetrasFromLayered::update()
{
	unsigned Nl = m_layers->layerCount();
	m_tetras.clear();
	m_tetras.resize(Nl+2);

	// layer edges.
	// layer i (1..N) spans (edges[i-1], edges[i]) and contains material i
	// material[0] is top exterior, material[N+1] is bottom exterior

	// current limitation: top & bottom exterior must be same material

	vector<float> edges(Nl+1);

	edges[0] = 0.0f;
	for(unsigned i=1;i<=Nl;++i)
		edges[i] = edges[i-1] + m_layers->layer(i)->thickness();


	// top exterior tetra
	m_tetras[0].matID = 0;
	m_tetras[0].adjTetras = array<unsigned,4>{{0U,0U,0U,0U }};
	m_tetras[0].IDfds = array<unsigned,4>{{ 0U,0U,0U,0U }};
	m_tetras[0].faceFlags = 0;

	m_tetras[0].nx = m_tetras[0].ny = m_tetras[0].nz = m_tetras[0].C = _mm_setzero_ps();

	for(unsigned i=1;i<=Nl;++i)
	{
		m_tetras[i].matID = i;
		m_tetras[i].faceFlags = 0;
		m_tetras[i].adjTetras = array<unsigned,4>{{ i-1, i+1, 0U, 0U }};
		m_tetras[i].IDfds = array<unsigned,4>{{ 2*i, 2*i+3, 0U, 0U }};

		m_tetras[i].nx = _mm_setzero_ps();
		m_tetras[i].ny = _mm_setzero_ps();
		m_tetras[i].nz = _mm_setr_ps(1.0f, -1.0f, 0.0f, 0.0f );
		m_tetras[i].C  = _mm_setr_ps(edges[i-1], -edges[i], -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());

		cout << "Tetra [" << i << "] ranges (" << edges[i-1] << ',' << edges[i] << ")" << endl;
	}

	// bottom exterior tetra
	m_tetras[Nl].adjTetras[1] = 0U;		// bottom tetra links down to tetra 0 (exterior) -- ideally would be a separate tetra
	m_tetras[Nl+1] = m_tetras[0];

	for(unsigned i=0;i<m_tetras.size();++i)
	{
		cout << "Tetra [" << i << "] links to " << m_tetras[i].adjTetras[0] << " and " << m_tetras[i].adjTetras[1] << endl;
	}
}

const vector<Tetra>& TetrasFromLayered::tetras() const
{
	return m_tetras;
}
