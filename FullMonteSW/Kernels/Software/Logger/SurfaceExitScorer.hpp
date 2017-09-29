/*
 * VolumeAbsorptionScorer.hpp
 *
 *  Created on: Sep 28, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_LOGGER_SurfaceExitScorer_HPP_
#define KERNELS_SOFTWARE_LOGGER_SurfaceExitScorer_HPP_

#include "AbstractScorer.hpp"
#include "AtomicMultiThreadAccumulator.hpp"
#include <vector>
#include "BaseLogger.hpp"

/**
 * Since surface exit is a relatively rare phenomenon, use an atomic accumulator to track.
 */

class SurfaceExitScorer : public AbstractScorer
{
public:
	SurfaceExitScorer();
	~SurfaceExitScorer();

	typedef AtomicMultiThreadAccumulator<double,float> Accumulator;


	////// Concrete Scorer override requirements
	virtual void prepare(const Kernel* K) override;
	virtual void clear() override;
	virtual void postResults(OutputDataCollection* C) const override;


	////// Scorer concept requirements
	class Logger;
	Logger createLogger();

private:
	Accumulator		m_acc;
};


class SurfaceExitScorer::Logger: public BaseLogger
{
public:
	typedef SurfaceExitScorer Scorer;

	Logger();
	~Logger();

	Logger(Logger&& lv_) = default;
	Logger(const Logger& lv_) = delete;

	inline void eventExit(AbstractScorer& S,const Ray3,int IDf,double w);

	// commit/clear are no-op because we accumulate atomically
	void eventCommit(AbstractScorer& S){}
	void eventClear(AbstractScorer& S){}

private:
	Accumulator::ThreadHandle		m_handle;		///< Actually an empty struct since the only info needed is the
													///< reference to the accumulator provided in the event call
};

inline void SurfaceExitScorer::Logger::eventExit(AbstractScorer& S,const Ray3,int IDf,double w)
{
	SurfaceExitScorer& SS = static_cast<SurfaceExitScorer&>(S);

	// IDf >> 1 converts the directed face into an undirected face
	m_handle.accumulate(SS.m_acc,IDf>>1,float(w));
}

#endif /* KERNELS_SOFTWARE_LOGGER_SurfaceExitScorer_HPP_ */
