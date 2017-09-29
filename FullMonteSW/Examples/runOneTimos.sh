# -T <time-step-ns> <num-time-steps>
# -m[i][s]      Capture (s)urface &/ (i)nternal fluence
# -t <num-thread>
# -f <mesh-file-name>
# -p <optical-file-name>
# -s <source-file-name>
# -o <output-file-name>
# -r <rand-seed>

/home/jcassidy/src/TIMOS/Build/Release/timos \
    -f /home/jcassidy/src/TIMOS/example/cube_5med/cube_5med.mesh \
    -p /home/jcassidy/src/TIMOS/example/cube_5med/cube_5med.opt \
    -s /home/jcassidy/src/TIMOS/example/cube_5med/cube_5med.source \
    -o cube_5med.out \
    -r 1 \
    -m si \
    -t 8

#-T 2000 1
