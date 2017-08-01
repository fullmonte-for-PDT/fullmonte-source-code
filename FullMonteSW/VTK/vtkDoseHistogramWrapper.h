/*
 * vtkFullMonteFieldAdaptor.h
 *
 *  Created on: Aug 12, 2016
 *      Author: jcassidy
 */

#ifndef VTK_vtkDoseHistogramWrapper_H_
#define VTK_vtkDoseHistogramWrapper_H_

#include <vtkObject.h>

#include <FullMonteSW/OutputTypes/OutputData.hpp>

class DoseHistogram;
class vtkFloatArray;
class vtkTable;

class vtkAbstractArray;

/** Converts a DoseHistogram to a vtkTable
 *
 * Output may be given in either %-of-organ terms or measure (area/volume) terms.
 *
 *
 */

class vtkDoseHistogramWrapper : public vtkObject
{
public:
	vtkTypeMacro(vtkDoseHistogramWrapper,vtkObject)

	enum MeasureMode { Relative, Absolute };
	enum MeasureType { Unknown, Surface, Volume };

	static vtkDoseHistogramWrapper* New();

	/// Change the source map
	void source(const char* mptr);
	void source(OutputData* D);

	/// Update the values from the source map
	void update();

	/// Collect the result (note: this is a single copy owned by this class; callers should not modify the returned pointer)
	vtkTable* table() const;


	/// Get/set the measure mode (Relative means % of total organ, Absolute means cumulative measure)
	MeasureMode		measureMode() const;
	MeasureMode		measureMode(MeasureMode m);

	MeasureType		measureType() const;
	MeasureType		measureType(MeasureType m);

protected:
	vtkDoseHistogramWrapper();
	~vtkDoseHistogramWrapper();

private:
	DoseHistogram*			m_histogram=nullptr;
	MeasureMode				m_measureMode=Relative;
	MeasureType				m_measureType=Unknown;
	vtkTable*				m_vtkTable;
};


#endif /* VTK_vtkDoseHistogramWrapper_H_ */
