/*
 * TIMOSOutputReader.hpp
 *
 *  Created on: Jun 2, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TIMOS_TIMOSOUTPUTREADER_HPP_
#define STORAGE_TIMOS_TIMOSOUTPUTREADER_HPP_

#include <string>
#include <array>
#include <vector>

class OutputData;

class TIMOSOutputReader
{
public:
	TIMOSOutputReader();
	~TIMOSOutputReader();

	void filename(std::string fn);

	void read();
	void clear();

	// select the specified timestep for output
	void timestep(unsigned);

	// sum all timesteps for output
	void sumTimesteps();



	unsigned surfaceSize() const;
	unsigned surfaceTimeSteps() const;

	unsigned volumeSize() const;
	unsigned volumeTimeSteps() const;

	OutputData* volumeFluence() const;
	OutputData* surfaceFluence() const;

private:
	std::string 	m_filename;

	struct FaceInfo {
		std::array<unsigned,3> IDps;
		float area;
	};

	struct TetraInfo {
		std::array<unsigned,4> IDps;
		float volume;
	};

	std::vector<FaceInfo>		m_faces;
	std::vector<TetraInfo>		m_tetras;

	unsigned		m_timeSteps=0;

	float**			m_surfaceData=nullptr;
	float**			m_volumeData=nullptr;

	double			m_surfaceEnergy=0.0;
};


#endif /* STORAGE_TIMOS_TIMOSOUTPUTREADER_HPP_ */
