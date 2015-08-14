<html>
<head/>
<body>
<h3>What is SwarmCon ?</h3>

SwarmCon is a minimalistic version of the <a href="http://www.youtube.com/watch?v=KgKrN8_EmUA">WhyCon</a> localization system intended for swarm applications.
While the core of the SwarmCon system is the same as the WhyCon one, it was designed to have as least dependencies as possible.
Moreover, the SwarmCon is especially intended for external localization of ground-robot swarms.
Thus, unlike WhyCon, SwarmCon can distinguish between individual robots and calculate their heading.

<h3>To use it</h3>

To start with the software:
<ol>
<li>Install the <a href="#libraries">SDL libraries</a>.</li>
<li>Download the software from GitHub and go to the <b>src</b> directory.</li>
<li>Adjust the camera resolution in the <b>main/swarmcon.cpp</b>.</li>
<li>Compile the software - just type <b>make</b>.</li>
<li>Print one circular <a href="pattern.pdf">pattern</a>.</li>
<li>Try a test run - you need to run the binary from the <b>bin</b> directory. Type <b>./swarmcon /dev/videoX 1</b>, where X is the number of the camera and 1 tells the system to track one pattern.</li> 
<li>You should see the image with some numbers below the circle. Pressing <b>D</b> shows the segmentation result.</li>
<li>Open your browser to view localhost:6666. You should see the circle position.</li>
</ol>

To setup a coordinate system and use more robots:
<ol>
<li>Calibrate your camera using the MATLAB (or Octave) calibration toolbox and put the <b>Calib_result.m</b> in the <b>etc</b> directory.</li>
<li>Go to the <b>etc</b> directory and call <b>create.sh N M</b> to generate patterns for <b>NxM</b> robots.</li>
<li>Print the generated file <b>pattern_n_m.pdf</b>, put the eliptical markers on your robots and place the remaining four circular markers at the corners of their operation space.</li>
<li>Modify the dimensions of the operation space in the <b>main/swarmcon.cpp</b>.</li> 
<li>Adjust the circle diameter in the <b>main/swarmcon.cpp</b>, default diameter is 30 mm.</li>
<li>Call <b>make</b> to recompile, put your camera facing down and overhead the your swarm.</li>
<li>Go to <b>bin</b> directory and run  <b>./swarmcon /dev/videoX Y</b>, where X is the number of your camera and Y is the number of patterns you want to track, i.e. Y=NxM+4.</li>
<li>Once all the patterns are found, press <b>a</b> and the four outermost patterns will be used to calculate the coordinate system.</li>
<li>Each pattern will have four numbers - ID, heading and x,y in mm.</li>
</ol>

For postprocessing :

<ol>
<li>Processing a saved video rather than the camera feed is likely to provide more precise results.</li>
<li>To create a video, simply create an <b>output</b> directory in the place where you run the <b>swarmcon</b>.
<li>If your camera supports MJPEG, then the system will create a video in the <b>output</b> directory.</li>
<li>You can run <b>swarmcon videofile Y</b> to process that video in the same way as when using the camera.</li>
<li>If your camera does not support MJPEG, it will save the video feed as a serie of bitmaps, that you can process later as well.</li>
</ol>

Furhermore:

<ol>
<li>Running the system as <b>./swarmcon /dev/videoX Y nogui</b> causes text-only output.</li>
<b>h</b> displays help.</li>
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
