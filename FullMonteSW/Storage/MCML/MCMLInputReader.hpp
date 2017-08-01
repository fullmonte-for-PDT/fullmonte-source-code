/*
 * MCMLInputReader.hpp
 *
 *  Created on: Jun 1, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_MCML_MCMLINPUTREADER_HPP_
#define STORAGE_MCML_MCMLINPUTREADER_HPP_

#include <string>
#include <list>

class MCMLCase;

class MCMLInputReader
{
public:
	MCMLInputReader();
	~MCMLInputReader();

	void filename(std::string fn);

	void read();

	/// Iterate through cases
	void 		start();
	void 		next();
	bool		done() const;
	MCMLCase*	current() const;
	MCMLCase*	get(unsigned i) const;
	unsigned	cases() const;

private:
	std::string 			m_filename;
	std::list<MCMLCase*>	m_cases;

	std::list<MCMLCase*>::iterator m_iterator;

	bool					m_verbose=false;
};




#endif /* STORAGE_MCML_MCMLINPUTREADER_HPP_ */
