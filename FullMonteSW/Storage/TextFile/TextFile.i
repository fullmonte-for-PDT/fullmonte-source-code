%module FullMonteTextFileTCL

%include "std_string.i"

%{
//#include "TextFileMeshWriter.hpp"
#include "TextFileDoseHistogramWriter.hpp"
#include <FullMonteSW/Geometry/TetraMesh.hpp>
%}

//%include "TextFile.hpp"
//%include "TextFileMeshWriter.hpp"
%include "TextFileDoseHistogramWriter.hpp"