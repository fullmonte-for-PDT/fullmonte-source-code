package require FullMonte 1.0

TIMOSMeshReader MR
MR filename "/home/jcassidy/src/FullMonteSW/data/TIM-OS/cube_5med/cube_5med.mesh"
MR read
set M [MR mesh]

TIMOSMaterialReader mR
mR filename "/home/jcassidy/src/FullMonteSW/data/TIM-OS/cube_5med/cube_5med.opt"
mR read
set MS [mR materials]

TIMOSSourceReader SR
SR filename "/home/jcassidy/src/FullMonteSW/data/TIM-OS/cube_5med/cube_5med.source"
SR read
set src [SR source]

TetraVolumeKernel K
K geometry $M
K source $src
K materials $MS
K threadCount 8
K packetCount 10000000
K rouletteWMin 1e-5

puts "Starting"
K runSync
puts "Done"

set RES [K results]

OutputDataSummarize summ
summ visit [$RES getByName "EventCounts"]
summ visit [$RES getByName "ConservationCounts"]

#vtkFullMonteTetraMeshWrapper VTKM
#    VTKM mesh $M
#    VTKM update
#
#vtkFullMonteArrayAdaptor vtkPhi
#    vtkPhi source [$RES getByName "VolumeEnergy"]
#
#set ug [VTKM regionMesh]
#[$ug GetCellData] AddArray [vtkPhi array]
#
#vtkFullMontePacketPositionTraceSetToPolyData TR
#    TR source [$RES getByName "PacketTraces"]
#    TR update
#
#vtkPolyDataWriter PDW
#    PDW SetInputData [TR getPolyData]
#    PDW SetFileName "mouse.traces.vtk"
#    PDW Update
#    PDW Delete
#
#vtkUnstructuredGridWriter W
#    W SetInputData $ug
#    W SetFileName "mouse.vol.vtk"
#    W Update
#    W Delete
