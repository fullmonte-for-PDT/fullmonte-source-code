/*
 * Builder.hpp
 *
 *  Created on: May 17, 2017
 *      Author: jcassidy
 */

#ifndef GEOMETRY_BUILDER_HPP_
#define GEOMETRY_BUILDER_HPP_

bool TetraMesh::checkFaces() const
{
	unsigned err=0;
	//assert(T.size()   == m_tetraPoints.size());
	assert(m_tetraFaces.size() == m_tetraPoints.size());
	assert(m_tetraMaterials.size() == m_tetraPoints.size());

	assert(m_faces.size() == m_facePoints.size());
	assert(m_faceTetras.size() == m_facePoints.size());

	if (m_enableVerboseConstruction)
		cout << "INFO TetraMesh::checkFaces() checking " << m_tetraPoints.size() << " tetras' faces for correct orientation" << endl;

	for(unsigned IDt=1;IDt < m_tetraPoints.size();++IDt)
	{
		TetraByPointID IDps = m_tetraPoints[IDt];
		TetraByFaceID IDfs = m_tetraFaces[IDt];

		Face F[4];

		bool tetOK=true;

		std::array<std::array<double,4>,4> h;

		for(unsigned f=0;f<4;++f)
		{
			int IDf = IDfs[f];

			F[f] = getFace(IDf);

			for(unsigned p=0;p<4;++p)
			{
				if ((h[f][p] = F[f].pointHeight(m_points[IDps[p]])) < -m_pointHeightTolerance)
				{
					if (m_enableVerboseConstruction)
						cout << "ERROR! TetraMesh::makeKernelTetras reports point height less than zero (" << h[f][p] << ") for " << p << "'th point of tetra " << IDt << " over face " << IDf << endl;
					tetOK=false;
				}

				if (h[f][p] < -1e-4)
				{
					if (m_enableVerboseConstruction)
						cout << "  NOTE: Face should be flipped" << endl;
					++err;
//					F[f].flip();
				}
			}
		}

//		if (m_enableVerboseConstruction && !tetOK)
//			printTetra(IDt);
	}

	return err==0;

	cout << "INFO: checkFaces() done at " << m_timer.elapsed().wall*1e-9 << 's' << endl;
}



void TetraMesh::buildTetrasAndFaces()
{
	cout << "Building mesh data structures" << endl;
	m_timer.start();

		mapTetrasToFaces();
		cout << "INFO: Tetra-face mapping finished at " << m_timer.elapsed().wall*1e-9 << "s" << endl;

	createFaceNormals();
	cout << "INFO: Face normals finished at " << m_timer.elapsed().wall*1e-9 << "s" << endl;

		orientFaces();
		cout << "INFO: Face orientation check finished at " << m_timer.elapsed().wall*1e-9 << "s" << endl;

	makeKernelTetras();
	cout << "INFO: Kernel tetras built at " << m_timer.elapsed().wall*1e-9 << "s" << endl;

	std::size_t Nf_surf = boost::size(m_faceTetras |		 boost::adaptors::filtered([](array<unsigned,2> i){ return i[1]==0; }));

	cout << "New mesh construction: " << m_points.size() << " points, " << m_tetraPoints.size() << " tetras, " << m_pointIDsToFaceMap.size() <<
			" faces (" << Nf_surf << " surface)" << endl;

	assert(m_tetraPoints.size() == m_tetraFaces.size());
}

#endif /* GEOMETRY_BUILDER_HPP_ */
