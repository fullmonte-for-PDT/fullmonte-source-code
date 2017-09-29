The data files in this directory are generated with the following input parameters:

(1) The total number of packets launched is 1e6.

(2) The source type is point. The location of the source is [90.77090454101562, 170.47908742804466, 124.069055680822278]. This is subject to rounding of the simulator's precision.

(3) The data type is volume-based energy/fluence, as specified with the file name.

(4) The comparison picture shows FullMonte data on the left and MMC data on the right. All the data have been adjusted to be unitized. The scale is set to logarithmic so the minimum data used is 1e-4 times of the maximum value.



####### Additional notes on MMC input file #######

To run MMC, a .json file is used as input. The commandline options used is listed as follows:
	-b 1 -C 0	----->		output cell data
	-O E		----->		output energy data
	-O X		----->		output fluence data
	-n 1e6		----->		set packet count to 1e6
	-s NAME		----->		set output file prefix to NAME
	-e 1e-5		----->		change roulette triggered at 1e-5 to match with FullMonte
					(MMC default is 1e-6)

