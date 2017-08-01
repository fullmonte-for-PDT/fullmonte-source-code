package provide FullMonte 1.0

namespace eval ::FullMonte {
    namespace export datadir

    #default file prefix
    set datadir "/home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/data"

    # default number of threads (set by cmake at configure time)
    set defaultThreadCount 8
}

proc ::FullMonte::runKernelWithProgress { k } {
    $k runAsync
}

proc ::FullMonte::defaultProgressCommand { } {
}



###### Load required FullMonte libraries

# TIMOS format reader (for Digimouse) with TCL bindings
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libFullMonteTIMOSTCL.so

# Geometry model with TCL bindings
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libFullMonteGeometryTCL.so

# Software kernels with TCL bindings
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libFullMonteSWKernelTCL.so
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libFullMonteKernelsTCL.so

# Data output manipulation
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libFullMonteDataTCL.so

# Data queries
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libFullMonteQueriesTCL.so

# MCML file input/output
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libFullMonteMCMLFileTCL.so

# VTK interface
load /home/afsharne/fullmonte_test_5/fullmonte_source_repositories/FullMonteSW/Build/Release/lib/libvtkFullMonteTCL-6.0.so
