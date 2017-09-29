%module FullMonteCOMSOLTCL

%include "std_string.i"

%include "std_vector.i"
%include "../../Geometry/FullMonteGeometry_types.i"


%{
#include <FullMonteSW/Geometry/Sources/Abstract.hpp>
#include <FullMonteSW/Geometry/TetraMesh.hpp>

#include <FullMonteSW/Storage/COMSOL/COMSOLMeshReader.hpp>
#include <FullMonteSW/Storage/COMSOL/COMSOLMeshWriter.hpp>

%}

%include "COMSOLMeshReader.hpp"
%include "COMSOLMeshWriter.hpp"

