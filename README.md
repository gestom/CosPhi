##CosPhi: An Artificial Pheromone System for Robotic Swarms

###Why to study pheromones?
Pheromones serve as a common means of communication in nature.
Social insects such as bees, ants and termites use pheromones both for inter-agent and agent-swarm communications.
Using pheromone-based communication, these simple individuals form communities and swarms that exhibit complex and intelligent behaviour.
The phenomena of <i>swarm intelligence</i>, which naturally emerges from interation of large amounts of simple individuals, is in the focus of the swarm robotics research.
Understanding pheromone-based communication is one of the keys that will allow to understand the emergence of swarm intelligence.
So far, pheromone-based communication was either simulated or implemented by different chemical (volatile compounds) and physical (RFID tags, light, sound) cues, which were not able to replicate all the aspects of pheromones as seen in nature. 

###What is CosPhi?
CosPhi is a novel artificial pheromone system that is reliable, accurate and uses off-the-shelf components only: an LCD screen and a low-cost USB camera.
The horizontally-positioned LCD displays the spatial distribution of the pheromones, which are sensed by the robots that move on the screen. 
The pheromones are released according to the robots' positions provided by the overhead camera:

![System configuration and Colias-Phi micro robot.](https://raw.githubusercontent.com/wiki/gestom/CosPhi/images/arena.png)<br/>System configuration and Colias-Phi micro robot.

[![Artificial Pheromone Experiments](https://raw.githubusercontent.com/wiki/gestom/CosPhi/images/arena.png)](https://www.youtube.com/watch?v=eXmyAxW8IZ8)"Pheromone-based Leader-Follower Scenario (TODO)"<br/>
<b>Click the picture to see a video of the CosPhi system operation.</b>

The Frequency Map Enhancement (FreMEn) is an enabling technology for long-term mobile robot autonomy in changing environments.
It allows to introduce the notion of dynamics into most spatial models used in the mobile robotics domain, improving the robots' ability to cope with naturally-occuring environment changes.
FreMEn is based on an assumption that from a mid- to long-term perspective, some of the environment dynamics are periodic.
To reflect that, FreMEn models the uncertainty of elementary environment states by combination of periodic functions rather than by a constant probability, which is usual in environment models used in mobile robotics.

![Spatio-temporal occupancy grid of the Lincoln Centre for Autonomous Systems (L-CAS) office. The static cells are in green and cells that exhibit daily periodicity are in red.](https://raw.githubusercontent.com/wiki/gestom/fremen/pics/grid.png) FreMEn-based occupancy grid of the Lincoln Centre for Autonomous Systems (L-CAS) office. The static cells are in green and cells that exhibit daily periodicity are in red.

###How is that useful ?
Modeling the uncertainties as probabilistic functions of time allows integration of long-term observations of the same environment into memory-efficient spatio-temporal models.
These models can **predict the future environment states** with a given level of confidence.
The predictive power of the FreMEn models improves the ability of mobile robots to operate in changing environments for long periods of time.
In several long-term experiments, FreMEn demonstrated to improve mapping [[1,2](#references)], localization [[3,4](#references)], path planning [[5](#references)], robotic search [[6](#references)], patrolling [[7](#references)] and exploration [[8](#references)].

###How does it work ?
The concept is based on the idea of frequency transforms, which represent functions of time by the frequencies that make them up.
FreMen simply takes a given sequence of long-term observation of a particular environment state, calculates its frequency spectra by the Fourier transform and stores the most prominent spectral components.
These components correspond to the observed periodicities of the given environment state.
Knowledge of the spectral components allows to calculate the probability of the environment state for a given time. 

The picture below illustrates the use of FreMEn for visual localization in changing environments. The long-term observations of a particular image feature visibility (red,centre), are transferred to the spectral domain (left). Transferring the most prominent spectral components (left, green) to the time domain provides an analytic expression (centre) representing the probability of the feature being visible at a given time (green, centre). This allows to predict the feature visibility at a time when the robot performs self-localization (blue). In this case, the FreMEn model is applied to all features visible at a given location, which allows to predict its appearance for a specific time. 

[![FreMEn for Visual Localization](https://raw.githubusercontent.com/wiki/gestom/fremen/pics/features.png)](https://www.youtube.com/watch?v=aTr9KD4XMGc"FreMEn for Visual Localization")
<b>Click the picture to see a detailed explanation - make sure you have sound on.</b>

###Where can I download it ?

The FreMEn is part of a software release of the EU-funded project [STRANDS](http://strands-project.eu). You can get its source code on [STRANDS github](https://github.com/strands-project/) or as a [Ubuntu package](http://strands.acin.tuwien.ac.at/software.html). FreMEn is implemented as a ROS action server.

###References
1. F. Arvin, T. Krajník, A. E. Turgut, and S. Yue: <b>[COSΦ: Artificial Pheromone System for Robotic Swarms Research.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_iros_pheromone.pdf)</b>In IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2015.
2. T. Krajnik, F. Arvin, A. E. Turgut, S. Yue, and T. Duckett: <b>[COSΦ: Vision-based Artificial Pheromone System for Robotic Swarms.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_icralbp_pheromone.pdf)</b> In: International Conference on Robotics and Automation. Late breaking poster session. 2015. 
3. F. Arvin, A. Attar, A. E. Turgut, and S. Yue: <b>Power-law distribution of long-term experimental data in swarm robotics.</b>In Advances in Swarm and Computational Intelligence. Springer, 2015.
