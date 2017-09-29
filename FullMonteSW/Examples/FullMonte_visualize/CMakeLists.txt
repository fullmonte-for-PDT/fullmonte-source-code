ADD_EXECUTABLE(FullMonte_visualize FullMonte_visualize.cpp ../Helpers/visualize.hpp ../Helpers/visualize.cpp)

SET_TARGET_PROPERTIES (FullMonte_visualize 
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Examples/FullMonte_visualize) 

TARGET_LINK_LIBRARIES(FullMonte_visualize ${VTK_LIBRARIES})


FILE(COPY FullMonte_visualize.in DESTINATION ${CMAKE_BINARY_DIR}/Examples/FullMonte_visualize)

