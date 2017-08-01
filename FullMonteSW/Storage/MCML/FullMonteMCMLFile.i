%module FullMonteMCMLFileTCL

%include "std_string.i"

%include "std_vector.i"
%include "../../Geometry/FullMonteGeometry_types.i"

%{
#include <FullMonteSW/Storage/MCML/MCMLOutputReader.hpp>
#include <FullMonteSW/Storage/MCML/MCMLInputReader.hpp>
#include <FullMonteSW/Storage/MCML/MCMLCase.hpp>

%}

%include "MCMLCase.hpp"
%include "MCMLOutputReader.hpp"
%include "MCMLInputReader.hpp"