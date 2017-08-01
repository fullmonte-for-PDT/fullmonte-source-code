/*
 * ErrorChecker.hpp
 *
 *  Created on: Apr 4, 2016
 *      Author: jcassidy
 */

#ifndef GEOMETRY_ERRORCHECKER_HPP_
#define GEOMETRY_ERRORCHECKER_HPP_

#include <iostream>
#include <vector>
#include <array>

class Error
{
public:
	Error();
	virtual ~Error();

	virtual void print(std::ostream& os = std::cout) const;
private:
};



class ErrorChecker
{
public:
	ErrorChecker();
	~ErrorChecker();


	void check();

	unsigned tested() const;
	unsigned errors() const;

	// Iteration through the reported errors
	void skipToError(unsigned i);
	bool nextError();
	bool prevError();

	// Inspect current error
	Error* currentError() const;

protected:
	void clear();

	virtual void 	setup()=0;
	virtual bool 	nextTestCase()=0;
	virtual Error* 	test()=0;

private:
	std::vector<Error*>		m_errors;
	unsigned				m_currentError=0;

	unsigned				m_tested=0;
};

#include "TetraMesh.hpp"

class MeshErrorChecker : public ErrorChecker
{
public:
	void mesh(const TetraMesh* M);

protected:
	const TetraMesh* m_mesh=nullptr;
};



class TetraOrientationChecker : public MeshErrorChecker
{
public:
	virtual ~TetraOrientationChecker();

protected:
	virtual void 	setup() 		override;
	virtual bool 	nextTestCase() 	override;
	virtual Error* 	test() 			override;

private:
	// Point heights less than this value will be reported
	double m_degenerateThreshold=1e-2;

	TetraMesh::TetraRange 		m_range;
	TetraMesh::TetraIterator 	m_tetraIt;
};

#endif /* GEOMETRY_ERRORCHECKER_HPP_ */
