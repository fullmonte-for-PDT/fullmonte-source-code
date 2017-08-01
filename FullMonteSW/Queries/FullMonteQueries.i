%module FullMonteQueriesTCL

%{
#include "EnergyToFluence.hpp"
#include "DoseHistogramGenerator.hpp"
#include "DoseHistogram.hpp"
#include "../OutputTypes/OutputData.hpp"
%}

%include "../OutputTypes/OutputData.hpp"
%include "EnergyToFluence.hpp"
%include "DoseHistogramGenerator.hpp"
%include "DoseHistogram.hpp"
