Run localisation system, check video (if all screen is in the camera view, etc) 

cd /home/farshad/CosPhi/Localization/src
../bin/swarmcon /dev/video1 0

if you know that camera is set correctly, then run with the nogui param:

../bin/swarmcon /dev/video1 0 nogui

Run pheromone system:
 
cd /home/farshad/CosPhi/Pheromone/src
SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS=0 ../bin/phero  experimentTime[s] evaporation[s] numRobots[-] pheromoneMultiplier[-] pheromoneMaxIntensity[s](0,1.0,0.9) cueMultiplier[-] (not working now)  cue0Diameter[m] cue0Intensity[-](0-1.0) cue1Diameter[m] cue1Intensity[-](0.0-1.0) Optional: cue0X:[m] cue0Y:[m] cue1X:[m] cue1Y:[m]
e.g.:

SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS=0 ../bin/phero 60 20 1 1.0 0.9 0 0.2 1.0 0 0


ADDITIONAL INFO:

Black spot parameters are around line 35 of Pheromone/src/main/phero.cpp
