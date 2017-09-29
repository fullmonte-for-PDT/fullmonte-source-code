/*
 * Kernel.hpp
 *
 *  Created on: Apr 22, 2015
 *      Author: jcassidy
 */

#ifndef KERNELS_KERNEL_HPP_
#define KERNELS_KERNEL_HPP_

#include <vector>

#include <memory>

#include <boost/range/any_range.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <string>
#include <list>

class OutputDataCollection;
class KernelObserver;
class MaterialSet;
class Geometry;

namespace Source { class Abstract; }

class OutputData;

class Kernel {
public:
	enum Status { Idle, Preparing, Running, Finished };

	virtual ~Kernel(){}

	void runSync();
	void startAsync();

	void finishAsync();

	bool 			done() const { return m_status == Finished; }
	virtual float 	progressFraction() 	const=0;

	/// get/set source (multiple sources accommodated by Source::Composite)
	void						source(const Source::Abstract* s)			{ m_src=s;			}
	const Source::Abstract*		source()							const 	{ return m_src; 	}

	/// get/set material definitions
	void						materials(const MaterialSet* m);
	const MaterialSet*			materials() const;

	void						geometry(const Geometry* G);
	const Geometry*				geometry() const;

	/// Get results
	OutputDataCollection*		results();

protected:
	const Source::Abstract*				m_src=nullptr;

	OutputDataCollection*				m_results=nullptr;

	const MaterialSet*					m_materialSet=nullptr;

	void awaitStatus(Status st);
	void updateStatus(Status st);
	Status status() const;

private:

	const Geometry*				m_geometry=nullptr;

	virtual void 	awaitFinish()		=0;

	std::vector<KernelObserver*> m_observers;

	virtual void gatherResults()=0;

	virtual void prepare_()=0;
	virtual void start_()=0;

	Status						m_status=Idle;
	std::mutex					m_statusMutex;
	std::condition_variable 	m_statusCV;

	std::thread 				m_parentThread;
};

#endif /* KERNELS_KERNEL_HPP_ */

