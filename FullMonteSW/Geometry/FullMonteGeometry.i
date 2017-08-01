%module FullMonteGeometryTCL

%include "std_string.i"
%include "std_vector.i"

%include "FullMonteGeometry_types.i"

%{

#include <FullMonteSW/Geometry/Sources/Abstract.hpp>
#include <FullMonteSW/Geometry/Sources/Ball.hpp>
#include <FullMonteSW/Geometry/Sources/Line.hpp>
#include <FullMonteSW/Geometry/Sources/Volume.hpp>
#include <FullMonteSW/Geometry/Sources/Composite.hpp>
#include <FullMonteSW/Geometry/Sources/Surface.hpp>
#include <FullMonteSW/Geometry/Sources/Point.hpp>
#include <FullMonteSW/Geometry/Sources/SurfaceTri.hpp>
#include <FullMonteSW/Geometry/Sources/PencilBeam.hpp>
#include <FullMonteSW/OutputTypes/clonable.hpp>
#include <FullMonteSW/OutputTypes/visitable.hpp>
#include <FullMonteSW/Geometry/Point.hpp>
#include <FullMonteSW/Geometry/Vector.hpp>
#include <FullMonteSW/Geometry/Geometry.hpp>
#include <FullMonteSW/Geometry/Layered.hpp>
#include <FullMonteSW/Geometry/Layer.hpp>

%}

%include "Sources/Abstract.hpp"
%include "Sources/Directed.hpp"
%include "Sources/Point.hpp"
%include "Sources/Volume.hpp"
%include "Sources/Ball.hpp"
%include "Sources/Composite.hpp"
%include "Sources/Surface.hpp"
%include "Sources/SurfaceTri.hpp"
%include "Sources/Line.hpp"
%include "Sources/PencilBeam.hpp"
%include "Geometry.hpp"
%include "Layered.hpp"
%include "Layer.hpp"

%{
#include <vector>
#include <FullMonteSW/Geometry/TetraMesh.hpp>
#include <FullMonteSW/Geometry/FaceLinks.hpp>
#include <sstream>
%}

%include "TetraMesh.hpp"