/*
 * Composite.hpp
 *
 *  Created on: Jan 26, 2016
 *      Author: jcassidy
 */

#ifndef GEOMETRY_SOURCES_COMPOSITE_HPP_
#define GEOMETRY_SOURCES_COMPOSITE_HPP_

#include "Abstract.hpp"
#include <vector>
#include <boost/range.hpp>

#include <FullMonteSW/OutputTypes/clonable.hpp>

namespace Source {

class Composite : public Abstract
{
public:
	explicit Composite(float w=1.0) : Abstract(w){}
#ifndef SWIG
	Composite(float w,std::vector<Abstract*>&& srcs) : Abstract(w),m_elements(srcs){}
#endif

	DERIVED_SOURCE_MACRO(Abstract,Composite)

	typedef boost::iterator_range<std::vector<Source::Abstract*>::iterator> 		range;
	typedef boost::iterator_range<std::vector<Source::Abstract*>::const_iterator>	const_range;

	range elements()
		{
		return range( m_elements.begin(), m_elements.end() );
		}

	const_range elements() const
			{
			return const_range( m_elements.cbegin(), m_elements.cend() );
			}

	unsigned count() const { return m_elements.size(); }

	void add(Source::Abstract* A)
	{
		m_elements.push_back(A);
	}

private:
	std::vector<Abstract*>		m_elements;
};

};

#endif /* GEOMETRY_SOURCES_COMPOSITE_HPP_ */
