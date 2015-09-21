##Phero - pheromone simulator of the CosΦ system

###What is Phero ?

<i>Phero</i> is a pheromone simulator and visualizer of the <i>CosΦ</i> system. 
Together with <i>SwarmCon</i>, <i>Phero</i> constitute the <i>CosΦ</i> artificial pheromone system that is reliable, accurate and uses off-the-shelf components only: an LCD screen and a low-cost USB camera.
The <i>Phero</i> system uses a horizontally-positioned LCD displays the spatial distribution of the pheromones, which are sensed by the robots that move on the screen. 
The pheromones are released according to the robots' positions provided by the <i>SwarmCon</i> system, which uses an overhead camera and circular markers placed on the robots.
In contrast to other systems, <i>CosΦ/Phero</i> can simulate several pheromones while precisely controlling their evaporation, diffusion and release rate.
The pheromone system is described in a short, late-breaking paper [[1](#references)] and a full research paper [[2](#references)].
 
###Setting it up

1. Install and setup the <a href="https://github.com/gestom/CosPhi/tree/master/Localization">SwarmCon</a> localization system if you did not do so already.
2. Download the software from GitHub and go to the <b>Pheromone/src</b> directory.
3. Setup your arena: place the robots on a horizontal LCD and fix your camera above it. Make sure that the camera is above the LCD's centre facing straight down. Make sure that it can see the entire LCD area and that the camera is oriented in the same way as the monitor. The set-up should look like on the picture below.
3. Open the <b>main/phero.cpp</b> and adjust the system parameters so these conform with your setup - i.e. with your LCD screen dimensions, camera height above the screen, robot dimensions, dual-monitor setup etc.
4. After updating the parameters, you can compile the software - just by typing <b>make</b>.

![Fig.1: Pheromone System configuration and Colias-Φ micro robot.](https://raw.githubusercontent.com/wiki/gestom/CosPhi/images/arena.png)<br/>

###Running it 
1. First, run the <a href="https://github.com/gestom/CosPhi/tree/master/Localization">SwarmCon</a> localization system (e.g. <b>../../Localization/bin/swarmcon /dev/video0 1</b>. You should see a window displaying the camera view - the entire LCD screen should be visible.
2. Then, run the Pheromone simulator itself: <b>../bin/phero E N</b>, where <i>E</i> is the half-life of the first pheromone (in seconds) and <i>N</i> is the number of robots on the field.
3. If you run in dual-monitor mode, the pheromones will appear in a borderless window. You can move this window to the horizontal LCD monitor using <i>ALT+mouse drag</i>. If you run a single-monitor setup, the pheromones will appear in fullscreen.
4. At first, the pheromone system will display four patterns at the corners of the screen and request calibration of the <i>SwarmCon</i> system. At this point, you should place your robots on the screen so that the <i>SwarmCon</i> can detect all the robots and calibration patterns.
5. Once detected, the system performs calibration and blue circles should appear aroung the individual robots. If these are slightly misaligned, you can press <b>C</b> to re-calibrate again. If the circles are terribly misaligned, then your camera might be oriented opposite to the screen orientation - you can correct it by turning the camera (or the screen) by 180 degrees.
6. Pressing SPACE start the pheromone simulation - as the robot with ID 0 moves, a pheromone track appears behind it.
7. Recalculation of the pheromones is computationally heavy. On slower machines, there can be a significant lag between the robot position and the displayed pheromone. This can be solved simply by reducing image dimensions / screen resolution in <b>phero.cpp</b>.

###Some additional remarks
1. At this point, you can start experimenting with the system by changing evaporation rates, adding diffusion etc.
2. We have tried to comment the code so an experienced programmer should be able to alter the system accordingly. However, if you have any questions regarding the code, feel free to contact [Tom Krajnik](http://scholar.google.co.uk/citations?user=Qv3nqgsAAAAJ&hl=en&oi=ao) or [Farshad](http://scholar.google.co.uk/citations?user=cUdtAowAAAAJ&hl=en)
3. If you use this system for your research, please don't forget to cite the two [papers](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/cite.bib), that describe the components of this system.
4. Have fun!
</ol>

###References
1. F. Arvin, T. Krajník, A. E. Turgut, and S. Yue: <b>[COSΦ: Artificial Pheromone System for Robotic Swarms Research.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_iros_pheromone.pdf)</b>In IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2015 [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_iros_pheromone.bib)].
3. T. Krajník, F. Arvin, A. E. Turgut, S. Yue, and T. Duckett: <b>[COSΦ: Vision-based Artificial Pheromone System for Robotic Swarms.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_icralbp_pheromone.pdf)</b> In: International Conference on Robotics and Automation. Late breaking poster session. 2015 [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_icralbp_pheromone.bib)].
