/*
 * DoseSurfaceHistogram.hpp
 *
 *  Created on: Aug 11, 2016
 *      Author: jcassidy
 */

#ifndef OUTPUTTYPES_DOSEHISTOGRAMGENERATOR_HPP_
#define OUTPUTTYPES_DOSEHISTOGRAMGENERATOR_HPP_

class TetraMesh;
class OutputData;

#include <vector>

template<class Value>class SpatialMap;

/** Abstract base query to generate a dose histogram (surface/volume).
 * It holds the basic parameters pointers to mesh, dose, and partition and provides the machinery to calculate the result.
 *
 * Concrete derived classes must provide the necessary functionality to determine volumes and doses.
 * Override requirements:
 * 		- getPartition() returns an unordered vector of ElRegion which holds (element, region)
 * 		- getDose(elements) returns a vector holding dose[element[i]] for all entries of m_element
 * 		- getMeasures(elements) returns a vector of measure[element[i]] for all entries of m_element (measure = surface/volume/etc)
 */

class DoseHistogramGenerator
{
public:
	DoseHistogramGenerator();
	virtual ~DoseHistogramGenerator();

	/// Get/set the associated mesh
	void 						mesh(const TetraMesh* M);
	const TetraMesh* 			mesh() 						const;

	void 						dose(const OutputData* D);
	const SpatialMap<float>*	dose() 						const;

	void update();

	/// EmpiricalCDF with float value (dose), float weight (area), and standard less-than comparison
	OutputData*					result() const;

protected:

#ifndef SWIG        // suppress SWIG warning re: nested structs
	struct ElRegion {
		unsigned element;
		unsigned region;

		ElRegion(unsigned element_,unsigned region_) : element(element_),region(region_){}

		struct RegionComp {
			bool operator()(const ElRegion& lhs,const ElRegion& rhs) const
				{ return lhs.region < rhs.region || (lhs.region==rhs.region && lhs.element < rhs.element); }

			/// Compare to an unsigned region for lower_bound
			bool operator()(const ElRegion& lhs,unsigned rhs) const
				{ return lhs.region < rhs; }
//
//			bool operator()(unsigned lhs,const ElRegion& rhs) const
//				{ return lhs < rhs.region; }
		};
	};
#endif

	virtual std::vector<ElRegion> getPartition()=0;										///< Return an unordered vector of (element,region)
	virtual std::vector<float> getMeasures(const std::vector<unsigned>& elements)=0;	///< Return a vector of measure[elements[i]]
	virtual std::vector<float> getDose(const std::vector<unsigned>& elements)=0;		///< Return a vector of dose[element[i]]

private:

	/// Updates internal state relevant to partition (m_regionEnd, m_element)
	void updatePartition();


	/// Updates the measures section of m_data, using the existing partition state
	void updateMeasures();


	/// Updates the dose section of m_data, using the existing partition state
	void updateDose();


	/// Updates the histogram itself using m_data and partition state
	void updateHistogram();


	/** Permutation vector such that m_data[i] = dose[m_elements[i]];
	 * Size is equal to the number of elements in all the DVHs.
	 */

	std::vector<unsigned>		m_elements;


	/** Vector tracking the DVH regions.
	 *
	 * Region i contains m_sourceElementIndex[j] for j = [m_regionEnd[i-1], m_regionEnd[i])
	 *
	 * Region 0 (exterior/void) does not enter into the DVH
	 * Size is region count
	 */

	std::vector<unsigned>		m_regionEnd;

#ifndef SWIG    // suppress SWIG warning re: nested structs
	struct InputElement {
		float		measure;		// measure (area/volume)
		float		dose;

		struct DoseLess
		{
			bool operator()(const InputElement& lhs,const InputElement& rhs) const { return lhs.dose < rhs.dose; }
		};
	};

	struct OutputElement : public InputElement
	{
		OutputElement(){}
		OutputElement(const InputElement& i) : InputElement(i){};
		float 		cmeasure=0.0f;
	};
#endif

	std::vector<InputElement>	m_data;
	std::vector<OutputElement>	m_histogram;

	const SpatialMap<float>* 	m_dose=nullptr;
	const TetraMesh*			m_mesh=nullptr;

//#ifndef SWIG
//	static float measure(const InputElement&);
//	static float dose(const InputElement&);
//#endif
};

//#ifndef SWIG
//inline float DoseHistogramGenerator::measure(const DoseHistogramGenerator::InputElement& e){ return e.measure; }
//inline float DoseHistogramGenerator::dose(const DoseHistogramGenerator::InputElement& e){ return e.dose; }
//#endif

#endif /* OUTPUTTYPES_DOSESURFACEHISTOGRAMGENERATOR_HPP_ */
