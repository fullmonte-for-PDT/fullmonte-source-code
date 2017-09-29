/*
 * COMSOLMeshWriter.cpp
 *
 *  Created on: July 26, 2017
 *      Author: Siyun Li
 */

#include <ctime>
#include <chrono>
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

#include <FullMonteSW/Storage/COMSOL/COMSOLMeshWriter.hpp>

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

COMSOLMeshWriter::COMSOLMeshWriter()
{

}

COMSOLMeshWriter::~COMSOLMeshWriter()
{
	delete[] typeName;
	delete[] node;
	delete[] element;
}

void COMSOLMeshWriter::filename(std::string fn)
{
	m_filename=fn;
}

void COMSOLMeshWriter::mesh(const TetraMesh* m)
{
	m_mesh=m;
}

void COMSOLMeshWriter::write() const
{
    ofstream os(m_filename.c_str());

    if(!os.good())
    {
    	cout << "ERROR - COMSOLMeshWriter::writeMesh(const TetraMesh&) failed to open file '" << m_filename << "' for writing" << endl;
    	return;
    }

    if (m_mesh->points()->size() == 0 || m_mesh->tetraCells()->size() == 0)
        return;
    
    chrono::time_point<chrono::system_clock> now;
    now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

// Some of the tag names and version numbers are not included here. This is subject to each model and should be added as a post-processing step.
    os << "# Created by FullMonte " << ctime(&currentTime) << endl;
    os << "\n";
    os << "# Major & minor version" << endl;
    os << "" << endl;
    os << "" << " # number of tags" << endl;
    os << "# Tags" << endl;
    os << " " << endl;
    os << "" << " # number of types" << endl;
    os << "# Types" << endl;
    os << " " << endl;
    os << "\n";
    os << "# --------- Object 0 ----------" << endl;
    os << "\n";
    os << " " << endl;
    os << " " << " # class" << endl;
    os << " " << " # version" << endl;
    os << " " << " # sdim" << endl;
    os << m_mesh->points()->size()-1 << " # number of mesh points" << endl;
    os << 0 << " # lowest mesh point index" << endl;
    os << endl << "# Mesh point coordinates" << endl;

    // COMSOL uses 0-based indexing but still need to drop tetra 0 and point 0
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

    os << "\n";

    os << elementType << " # number of element types" << endl;
    os << "\n";

    for (unsigned i = 0; i < elementType; ++i)
    {
	os << "# Type #" << i << endl;
	os << "\n";
	os << typeName[i] << " # type name" << endl;
	os << "\n\n";
	os << node[i] << " # number of nodes per element" << endl;
	os << element[i] << " # number of elements" << endl;
	os << "# Elements" << endl;

	for(unsigned i=1; i < m_mesh->tetraCells()->size(); ++i)
	{
		TetraByPointID T = m_mesh->tetraCells()->get(i);
		for(unsigned j=0; j<4; ++j)
		{
			os << setw(m_indexWidth) << --T[j];
			if (j !=3)
				os << ' ';
		}
		os << endl;
	}
    }

    if (os.fail())
    {
    	cout << "ERROR - TIMOSWriter::writeMesh(const TetraMesh&) failure during writing of file '" << m_filename << "'" << endl;
    	return;
    }
}

void COMSOLMeshWriter::setElementTypeCount(unsigned types)
{
	this->elementType = types;
	this->typeName = new string[elementType];
	this->node = new unsigned[elementType];
	this->element = new unsigned[elementType];

}

void COMSOLMeshWriter::setElementDetails(unsigned count, string name, unsigned nodeCount)
{
	typeName[count] = name;
	node[count] = nodeCount;
	element[count] = m_mesh->tetraCells()->size()-1;
}
