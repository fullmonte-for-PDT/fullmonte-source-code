/*
 * VolumeAbsorptionScorer.hpp
 *
 *  Created on: Sep 28, 2016
 *      Author: jcassidy
 */

#ifndef KERNELS_SOFTWARE_LOGGER_VOLUMEABSORPTIONSCORER_HPP_
#define KERNELS_SOFTWARE_LOGGER_VOLUMEABSORPTIONSCORER_HPP_

#include "AbstractScorer.hpp"

#include "QueuedMultiThreadAccumulator.hpp"
#include "BaseLogger.hpp"

/** Logs absorption events per volume element
 *
 */

class VolumeAbsorptionScorer : public AbstractScorer
{
public:
	VolumeAbsorptionScorer();
	virtual ~VolumeAbsorptionScorer();

	typedef QueuedMultiThreadAccumulator<double,float> Accumulator;

	////// Concrete Scorer requirements
	virtual void prepare(const Kernel* K) override;
	virtual void clear() override;
	virtual void postResults(OutputDataCollection* o) const override;



	////// Scorer logger factory requirements
	class Logger: public BaseLogger
	{
	public:
		explicit Logger(std::size_t qSize);
		Logger(const Logger&) = delete;
		Logger(Logger&&) = default;

		inline void eventAbsorb(AbstractScorer& S,Point3 p,unsigned IDt,double w0,double dw)
			{ m_handle.accumulate(static_cast<VolumeAbsorptionScorer&>(S).m_acc,IDt,dw); }

		void eventClear();
		void eventCommit(AbstractScorer& S);

	private:
		typename Accumulator::ThreadHandle m_handle;
	};

	Logger createLogger();



	////// Configuration
	void queueSize(unsigned q);

private:
	unsigned m_queueSize=1024;

	QueuedMultiThreadAccumulator<double,float> m_acc;
};

#endif /* KERNELS_SOFTWARE_LOGGER_VOLUMEABSORPTIONSCORER_HPP_ */
