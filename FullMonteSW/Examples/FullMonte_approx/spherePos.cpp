/*
 * spherePos.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: Siyun Li
 */

#include "spherePos.hpp"
#include <iostream>

SpherePos::SpherePos()
{
}

SpherePos::~SpherePos()
{
}

void SpherePos::findPoints()
{
	for (unsigned i = 1; i < rad; ++i)
		for (unsigned j = 0; j < 14; ++j)
		{
			std::array<float, 3> temp, temp2;
			for (unsigned k = 0; k < 3; ++k)
			{
				temp[k] = centre[k] + unitDist[j][k]*i;
				temp2[k] = centre[k] + temp_pts[j][k]*i;
			}
			points.push_back(temp);
			points_temp.push_back(temp2);
		}
}

void SpherePos::writePoints()
{
	findPoints();
	std::ofstream fout("points_unitDist.txt");
	fout << std::fixed << std::setprecision(6);
	fout << centre[0] << "\t" << centre[1] << "\t" << centre[2] << std::endl;
	for (auto &v : points)
	{
		for (unsigned j = 0; j < 3; ++j)
			fout << v[j] << "\t";
		fout << std::endl;
	}

        std::ofstream fout2("points_temp.txt");
	fout2 << std::fixed << std::setprecision(6);
	fout2 << centre[0] << "\t" << centre[1] << "\t" << centre[2] << std::endl;
        for (auto &v : points_temp)
        {
                for (unsigned j = 0; j < 3; ++j)
                        fout2 << v[j] << "\t";
                fout2 << std::endl;
        } 
}
