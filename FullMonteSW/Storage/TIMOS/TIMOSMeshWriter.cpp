/*
 * TIMOSWriter.cpp
 *
 *  Created on: Mar 7, 2015
 *      Author: jcassidy
 */

#include <iostream>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <FullMonteSW/take_drop.hpp>
#include <FullMonteSW/Geometry/Partition.hpp>

#include <FullMonteSW/Geometry/Sources/Abstract.hpp>
#include <FullMonteSW/Geometry/Sources/Composite.hpp>
#include <FullMonteSW/Geometry/Sources/SurfaceTri.hpp>
#include <FullMonteSW/Geometry/Sources/Surface.hpp>
#include <FullMonteSW/Geometry/Sources/Volume.hpp>
#include <FullMonteSW/Geometry/Sources/Point.hpp>
#include <FullMonteSW/Geometry/Sources/PencilBeam.hpp>

#include <sstream>

#include <fstream>
#include <iomanip>

#include <FullMonteSW/Geometry/Points.hpp>
#include <FullMonteSW/Geometry/Cells.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSMeshWriter.hpp>

struct delim {
	std::string pre,delim,post;
};

struct delim_stream {
	delim D;
	std::ostream& os;
};

delim_stream operator<<(std::ostream& os,delim D)
{
	return delim_stream{ D, os };
}


template<typename T,size_t N>std::ostream& operator<<(const delim_stream ds,const std::array<T,N> a)
{
	auto w = ds.os.width();
	ds.os.width(0);
	ds.os << ds.D.pre;
	ds.os << std::setw(w) << a[0];
	for(unsigned i=1;i<N;++i)
		ds.os << ds.D.delim << std::setw(w) << a[i];
	ds.os << ds.D.post;
	return ds.os;
}

using namespace std;

TIMOSMeshWriter::TIMOSMeshWriter()
{

}

TIMOSMeshWriter::~TIMOSMeshWriter()
{

}

void TIMOSMeshWriter::filename(std::string fn)
{
	m_filename=fn;
}

void TIMOSMeshWriter::mesh(const TetraMesh* m)
{
	m_mesh=m;
}

void TIMOSMeshWriter::write() const
{
    ofstream os(m_filename.c_str());

    if(!os.good())
    {
    	cout << "ERROR - TIMOSWriter::writeMesh(const TetraMesh&) failed to open file '" << m_filename << "' for writing" << endl;
    	return;
    }

    if (m_mesh->points()->size() == 0 || m_mesh->tetraCells()->size() == 0)
    	return;

    os << m_mesh->points()->size()-1 << endl << m_mesh->tetraCells()->size()-1 << endl;

    // TIM-OS uses 1-based indexing so drop tetra 0 and point 0
    for(unsigned i=1; i < m_mesh->points()->size(); ++i)
    {
    	Point<3,double> P = m_mesh->points()->get(i);
    	os << fixed << setprecision(m_coordinatePrecision);

    	for(unsigned j=0;j<3;++j)
    	{
    		os << setw(m_coordinateWidth) << P[j];
    		if (j != 2)
    			os << ' ';
    	}
    	os << endl;
    }

    for(unsigned i=1; i < m_mesh->tetraCells()->size(); ++i)
    {
    	TetraByPointID T = m_mesh->tetraCells()->get(i);
    	for(unsigned j=0; j<4; ++j)
    		os << setw(m_indexWidth) << T[j] << ' ';
    	os << setw(m_regionWidth) << m_mesh->regions()->get(i);
    	os << endl;
    }

    if (os.fail())
    {
    	cout << "ERROR - TIMOSWriter::writeMesh(const TetraMesh&) failure during writing of file '" << m_filename << "'" << endl;
    	return;
    }
}
