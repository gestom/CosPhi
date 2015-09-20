<html>
<head/>
<body>
<h3>What is Phero ?</h3>

Phero is a pheromone simulator intended for micro-robotic swarm applications.
While the core of the SwarmCon system is the same as the WhyCon one, it was designed to have as least dependencies as possible.
Moreover, the SwarmCon is especially intended for external localization of ground-robot swarms.
Thus, unlike WhyCon, SwarmCon can distinguish between individual robots and calculate their heading.

WhyCon was first presented on ICRA2013 conference and later in the JINT journal.
If you use this software for your publication it is mandatory to cite WhyCon using the references in the provided cite.bib file. 
Full URL to this file is: https://raw.github.com/lrse/whycon/master/cite.bib.

<h3>Setting up</h3>

<ol>
<li>Install and setup the <a href="https://github.com/gestom/CosPhi/tree/master/Localization">SwarmCon</a> localization system if you did not do so already.</li>
<li>Download the software from GitHub and go to the <b>Pheromone/src</b> directory.</li>
<li>Setup your arena: place the robots on a horizontal LCD and fix your camera above it. Make sure that the camera is above the LCD's centre facing straight down. Make sure that it can see the entire LCD area and that the camera is oriented in the same way as the monitor. The set-up should look like on the picture below.</li>
<li>Open the <b>main/phero.cpp</b> and adjust the system parameters so these conform with your setup - i.e. with your LCD screen dimensions, camera height above the screen, robot dimensions, dual-monitor setup etc.</li>
<li>After updating the parameters, you can compile the software - just by typing <b>make</b>.</li>
</ol>

![Fig.1: Pheromone System configuration and Colias-Φ micro robot.](https://raw.githubusercontent.com/wiki/gestom/CosPhi/images/arena.png)<br/>

<h3>Running up</h3>
<ol>
<li>First, run the <a href="https://github.com/gestom/CosPhi/tree/master/Localization">SwarmCon</a> localization system (e.g. <b>../../Localization/bin/swarmcon /dev/video0 1</b>. You should see a window displaying the camera view - the entire LCD screen should be visible.</li>
<li>Then, run the Pheromone simulator itself: <b>../bin/phero E N</b>, where <i>E</i> is the half-life of the first pheromone (in seconds) and <i>N</i> is the number of robots on the field.</li>
<li>If you run in dual-monitor mode, the pheromones will appear in a borderless window. You can move this window to the horizontal LCD monitor using <i>ALT+mouse drag</i>. If you run a single-monitor setup, the pheromones will appear in fullscreen.</li>
<li>At first, the pheromone system will display four patterns at the corners of the screen and request calibration of the <i>SwarmCon</i> system. At this point, you should place your robots on the screen so that the <i>SwarmCon</i> can detect all the robots and calibration patterns.</li>
<li>Once detected, the system performs calibration and blue circles should appear aroung the individual robots. If these are misaligned, then your camera might be oriented opposite to the screen orientation - you can correct it by turning the camera (or the screen) by 180 degrees.</li>
<li>Pressing SPACE start the pheromone simulation - as the robot with ID 0 moves, a pheromone track appears behind it.</li>
</ol>

<h3>Some additional remarks</h3>
<ol>
<li>At this point, you can start experimenting with the system by changing evaporation rates, adding diffusion etc.</li>
<li>We have tried to comment the code so an experienced programmer should be able to alter the system accordingly. However, if you have any questions regarding the code, feel free to contact [me](http://scholar.google.co.uk/citations?user=Qv3nqgsAAAAJ&hl=en&oi=ao) or [Farshad](http://scholar.google.co.uk/citations?user=cUdtAowAAAAJ&hl=en)</li>
<li>If you use this system for your research, please don't forget to cite the two [papers](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/cite.bib), that describe the components of this system.</li>
<li>Have fun!</li>
</ol>

</body>
</html>
