## SwarmCon - marker-based localization system for swarm robotics

### What is SwarmCon ?
SwarmCon is a minimalistic version of the <a href="http://www.youtube.com/watch?v=KgKrN8_EmUA">WhyCon</a> localization system intended for swarm applications.
While the core of the <i>SwarmCon</i> system is the same as the <i>WhyCon</i> one, it was designed to have as least dependencies as possible.
Moreover, the <i>SwarmCon</i> is especially intended for external localization of ground-robot swarms.
Thus, unlike <i>WhyCon</i>, <i>SwarmCon</i> can distinguish between individual robots and calculate their heading.

### Who did it ?
The <i>WhyCon</i> system was developed as a joint project between the University of Buenos Aires, Czech Technical University and University of Lincoln in UK.
The main contributors were [Matias Nitsche](https://scholar.google.co.uk/citations?user=Z0hQoRUAAAAJ&hl=en&oi=ao), [Tom Krajnik](http://scholar.google.co.uk/citations?user=Qv3nqgsAAAAJ&hl=en&oi=ao) and [Jan Faigl](https://scholar.google.co.uk/citations?user=-finD_sAAAAJ&hl=en).
<i>WhyCon</i> was first presented on International Conference on Advanced Robotics 2013 [[1](#references)], later in the Journal of Intelligent and Robotics Systems [[2](#references)] and finally at the Workshop on Open Source Aerial Robotics during the International Conference on Intelligent Robotic Systems, 2015 [[3](#references)]. Its early version was also presented at the International Conference of Robotics and Automation, 2013 [[4](#references)].
If you decide to use this software for your research, please cite <i>WhyCon</i> using the one of the references provided in this [bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/WhyCon.bib) file.

### Setting up SwarmCon

#### Quick setup for initial testing

1. Install the required <a href="#libraries">libraries</a>.
1. Check the available resolutions, framerates and formats of your USB camera by running <b>guvcview -L</b>. 
1. Run <b>guvcview</b> and adjust your camera settings (exposure, brightness etc).
1. Download the software from GitHub and go to the <b>Localization/src</b> directory.
1. Adjust the camera resolution in the <b>main/swarmcon.cpp</b>.
1. Compile the software - just type <b>make</b>.
1. Download, resize and print one circular <a href="etc/test.pdf">pattern</a> - you have the pattern also in the <b>Localization/etc/test.pdf</b> file.
1. Try a test run - you need to run the binary from the <b>bin</b> directory. Type <b>./swarmcon /dev/videoX 1</b>, where X is the number of the camera and 1 tells the system to track one pattern.</li> 
1. You should see the image with some numbers below the circle. Pressing <b>D</b> shows the segmentation result.
1. At this point, you can also change camera brightness, exposure, contrast by pressing <i>(SHIFT) b, e, c</i> respectively. These settings are stored in <i>etc/camera.cfg</i> and reloaded on restart.
1. Open your browser to view localhost:6666. You should see the circle position.

#### Advanced setup for more robots in a rectangular field

1. Calibrate your camera using the MATLAB (or Octave) calibration toolbox and put the <b>Calib_Results.m</b> in the <b>etc</b> directory. (this step is not necessary, but it improves localization precision)
2. Go to the <b>etc</b> directory and call <b>create.sh N M</b> to generate patterns for <b>NxM</b> robots.
3. Print the generated file <b>pattern_n_m.pdf</b>, put the eliptical markers on your robots and place the remaining four circular markers at the corners of their operation space.
4. Modify the dimensions of the operation space in the <b>main/swarmcon.cpp</b>. 
5. If you have resized the markers (their default size is 30mm), then adjust the their diameter in the <b>main/swarmcon.cpp</b> file.
6. Call <b>make</b> to recompile, put your camera facing down, overhead your swarm.
6. Go to <b>bin</b> directory and run  <b>./swarmcon /dev/videoX Y</b>, where X is the number of your camera and Y is the number of patterns you want to track, i.e. Y=NxM+4.
7. Once all the patterns are found, press <b>a</b> and the four outermost patterns will be used to calculate the coordinate system.
8. Each pattern will have four numbers - ID, heading and x,y in mm.
9. Pressing <b>+</b>,<b>-</b> changes the number of localized patterns.

#### To postprocess the stored videos

1. To create a log of the robot positions, simply create an <b>output</b> folder at the directory where you run the <b>swarmcon</b>.
2. If your camera supports the MJPEG format, then the system will create a video in the <b>output</b> folder as well.
3. If your camera does not support MJPEG, <b>swarmcon</b> will save the video feed as a serie of bitmaps, that you can process later as well.
4. You can run <b>swarmcon video_file_name Y</b> to process that video in the same way as when using the camera, i.e. <b>video_file_name</b> instead of <b>/dev/videoX</b>.
5. Processing a saved video rather than the camera feed is likely to provide more precise results.
6. Running the system with a <b>nogui</b> argument e.g. <b>./swarmcon /dev/video0 1 nogui</b> causes text-only output - this can speed-up postprocessing.
7. Logs and videos might be large - to prevent saving logs and videos, run the system with <b>nolog</b> or <b>novideo</b> argument.

#### Some additional remarks

2. At this point, you can start experimenting with the syste by adding whatever features you might think useful.
3. We have tried to comment the code so an experienced programmer should be able to alter the system accordingly. However, if you have any questions regarding the code, feel free to contact [Tom Krajnik](http://scholar.google.co.uk/citations?user=Qv3nqgsAAAAJ&hl=en&oi=ao) or [Matias Nitsche](https://scholar.google.co.uk/citations?user=Z0hQoRUAAAAJ&hl=en&oi=ao)
4. If you use this localization system for your research, please don't forget to cite at least one relevant paper from these [bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/WhyCon.bib) records.
5. Have fun!
</ol>

<hr>

### Dependencies

All the following libraries are probably in your packages.

1. <b>libsdl1.2-dev</b> for graphical user interface.
2. <b>libsdl-ttf2.0-dev</b> to print stuff in the GUI.
3. <b>libncurses5-dev</b> to print stuff on the terminal.
4. <b>luvcview</b> or <b>guvcview</b> to set-up the camera.

### References
1. T. Krajník, M. Nitsche et al.: <b>[External localization system for mobile robotics.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2013_icar_whycon.pdf)</b> International Conference on Advanced Robotics (ICAR), 2013. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2013_icar_whycon.bib)].
2. T. Krajník, M. Nitsche et al.: <b>[A Practical Multirobot Localization System.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_JINT_whycon.pdf)</b> Journal of Intelligent and Robotic Systems (JINT), 2014. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_JINT_whycon.bib)].
3. M. Nitsche, T. Krajník et al.: <b>[WhyCon: An Efficent, Marker-based Localization System.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_irososar_whycon.pdf)</b> IROS Workshop on Open Source Aerial Robotics, 2015. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_irososar_whycon.bib)].
4. J. Faigl, T. Krajník et al.: <b>[Low-cost embedded system for relative localization in robotic swarms.](http://ieeexplore.ieee.org/xpls/abs_all.jsp?arnumber=6630694)</b> International Conference on Robotics and Automation (ICRA), 2013. [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2013_icra_whycon.bib)].

### Acknowledgements

The development of this work was supported by EU within its Seventh Framework Programme project ICT-600623 ``STRANDS''.
The Czech Republic and Argentina have given support through projects 7AMB12AR022, ARC/11/11, 13-18316P and 17-27006Y STRoLL - Spatio-Temporal Representations for Mobile Robot Navigation. We sincerely acknowledge [Jean Pierre Moreau](http://jean-pierre.moreau.pagesperso-orange.fr/infos.html) for his excellent libraries for numerical analysis that we use in our project. 
 .
