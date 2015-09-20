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
<li>Install and setup the <a href="https://github.com/gestom/CosPhi/tree/master/Localization">SwarmCon system</a> if you did not do so already.</li>
<li>Download the software from GitHub and go to the <b>Pheromone/src</b> directory.</li>
<li>Setup your arena: place the robots on a horizontal LCD and fix your camera above it. Make sure that the camera is above the LCD's centre facing straight down. Make sure that it can see the entire LCD area and that the camera is oriented in the same way as the monitor. The set-up should look like on picture</li>
<li>Open the <b>main/phero.cpp</b> and adjust the system parameters so these conform with your setup - i.e. with your LCD screen dimensions, camera height above the screen, robot dimensions, dual-monitor setup etc.</li>
<li>After updating the parameters, you can compile the software - just by typing <b>make</b>.</li>
<li>.</li>
</ol>

[!Fig.1: Pheromone System configuration and Colias-Φ micro robot.](https://raw.githubusercontent.com/wiki/gestom/CosPhi/images/arena.png)<br/>

<h4>Advanced setup for more robots in a rectangular field</h4>
<ol>
<li>Calibrate your camera using the MATLAB (or Octave) calibration toolbox and put the <b>Calib_result.m</b> in the <b>etc</b> directory. (this step is not necessary, but it improves localization precision)</li>
<li>Go to the <b>etc</b> directory and call <b>create.sh N M</b> to generate patterns for <b>NxM</b> robots.</li>
<li>Print the generated file <b>pattern_n_m.pdf</b>, put the eliptical markers on your robots and place the remaining four circular markers at the corners of their operation space.</li>
<li>Modify the dimensions of the operation space in the <b>main/swarmcon.cpp</b>.</li> 
<li>If you have resized the markers (their default size is 30mm), then adjust the their diameter in the <b>main/swarmcon.cpp</b> file.</li>
<li>Call <b>make</b> to recompile, put your camera facing down, overhead your swarm.</li>
<li>Go to <b>bin</b> directory and run  <b>./swarmcon /dev/videoX Y</b>, where X is the number of your camera and Y is the number of patterns you want to track, i.e. Y=NxM+4.</li>
<li>Once all the patterns are found, press <b>a</b> and the four outermost patterns will be used to calculate the coordinate system.</li>
<li>Each pattern will have four numbers - ID, heading and x,y in mm.</li>
</ol>

<h4>To postprocess the stored videos</h4>
<ol>
<li>Processing a saved video rather than the camera feed is likely to provide more precise results.</li>
<li>To create a video, simply create an <b>output</b> folder at the directory where you run the <b>swarmcon</b>.
<li>If your camera supports the MJPEG format, then the system will create a video in the <b>output</b> directory that you created.</li>
<li>You can run <b>swarmcon video_file_name Y</b> to process that video in the same way as when using the camera, i.e. <b>video_file_name</b> instead of <b>/dev/videoX</b>.</li>
<li>If your camera does not support MJPEG, <b>swarmcon</b> will save the video feed as a serie of bitmaps, that you can process later as well.</li>
</ol>

<h4>Some additional remarks</h4>

<ol>
<li>Running the system as <b>./swarmcon /dev/videoX Y nogui</b> causes text-only output - this can speed-up postprocessing.</li>
<li>Pressing <b>h</b> displays help.</li>
<li>Pressing <b>+</b>,<b>-</b> changes the number of localized patterns.</li>
</ol>

<hr>
<h4>Dependencies</h4><a NAME="libraries"></a>

All the following libraries are probably in your packages.

<ul>
<li><b>libSDL-1.2</b> for graphical user interface.</li>
<li><b>libSDL_ttf-2.0</b> to print stuff in the GUI</li>
<li><b>libncurses5-dev</b> to print stuff on the terminal</li>
</ul>

</body>
</html>
