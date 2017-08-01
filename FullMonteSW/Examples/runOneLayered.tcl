package require FullMonte 1.0

MCMLInputReader R
R filename "../External/src-mcml/sample.mci"
R read

R start

set C [R current]
puts "[$C packets] packets"
puts "Output to [$C outputFilename]"
puts "[[$C geometry] layerCount] layers"
puts "Roulette threshold: [$C rouletteThreshold]"

MCMLKernel K
K geometry [$C geometry]
K packetCount [$C packets]
K threadCount 8
K rouletteWMin [$C rouletteThreshold]
K materials [$C materials]

puts "Running"
set tStart [clock clicks -millisecond]
K runSync
set tEnd [clock clicks -millisecond]
puts "[expr $tEnd-$tStart] ms"
puts "Done"

set res [K results]

OutputDataSummarize summ
summ visit [$res getByName "EventCounts"]
summ visit [$res getByName "ConservationCounts"]

