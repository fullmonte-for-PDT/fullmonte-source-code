/*
 * vtkFullMontePacketPositionTraceToPolyData.h
 *
 *  Created on: Sep 26, 2016
 *      Author: jcassidy
 */

#ifndef VTK_VTKFULLMONTEPACKETPOSITIONTRACESETTOPOLYDATA_H_
#define VTK_VTKFULLMONTEPACKETPOSITIONTRACESETTOPOLYDATA_H_

#include <vtkObject.h>

class vtkPolyData;
class PacketPositionTraceSet;
class vtkUnsignedShortArray;
class vtkFloatArray;
class vtkIntArray;

class vtkFullMontePacketPositionTraceSetToPolyData : public vtkObject
{
public:
	vtkTypeMacro(vtkFullMontePacketPositionTraceSetToPolyData,vtkObject)

	virtual ~vtkFullMontePacketPositionTraceSetToPolyData();

	static vtkFullMontePacketPositionTraceSetToPolyData* New();

	void source(const char* swigPtrString);
	void source(const PacketPositionTraceSet* traces);

	const PacketPositionTraceSet* source() const;

	void update();

	vtkPolyData* getPolyData() const;

	void includeWeight(bool e);
	void includeLogWeight(bool e);
	void includeTime(bool e);
	void includeLength(bool e);
	void includeSteps(bool e);

	void includeTetra(bool e);
	void includeMaterial(bool e);

protected:
	vtkFullMontePacketPositionTraceSetToPolyData();

private:
	bool						m_includeWeight=true;
	bool						m_includeTime=false;
	bool						m_includeLength=true;
	bool 						m_includeSteps=true;
	bool						m_includeLogWeight=true;
	bool						m_includeTetra=false;
	bool						m_includeMaterial=false;

	const PacketPositionTraceSet*	m_traces=nullptr;
	vtkPolyData*					m_vtkPD=nullptr;

	vtkFloatArray*					m_vtkWeight=nullptr;
	vtkFloatArray*					m_vtkLogWeight=nullptr;
	vtkFloatArray*					m_vtkTime=nullptr;
	vtkFloatArray*					m_vtkLength=nullptr;
	vtkUnsignedShortArray*			m_vtkStepCount=nullptr;
	vtkIntArray*					m_vtkTetra=nullptr;
	vtkUnsignedShortArray*			m_vtkMaterial=nullptr;
};


#endif /* VTK_VTKFULLMONTEPACKETPOSITIONTRACESETTOPOLYDATA_H_ */
