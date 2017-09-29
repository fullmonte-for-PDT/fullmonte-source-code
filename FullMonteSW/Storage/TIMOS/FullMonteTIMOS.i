%module FullMonteTIMOSTCL

%include "std_string.i"

%include "std_vector.i"
%include "../../Geometry/FullMonteGeometry_types.i"

%{
#include <FullMonteSW/Geometry/Sources/Abstract.hpp>
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Storage/TIMOS/TextFileReader.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSMeshReader.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSSourceReader.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSMeshWriter.hpp>
#include <FullMonteSW/Storage/TIMOS/TIMOSMaterialReader.hpp>

%}

%include "TextFileReader.hpp"
%include "TIMOSMaterialReader.hpp"
%include "TIMOSMeshReader.hpp"
%include "TIMOSMeshWriter.hpp"
%include "TIMOSSourceReader.hpp"