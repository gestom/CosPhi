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
The pheromones are released according to the robots' positions provided by the overhead camera.


[![System configuration and Colias-Phi micro robot.](https://raw.githubusercontent.com/wiki/gestom/CosPhi/images/arena.png)](https://www.youtube.com/watch?v=eXmyAxW8IZ8)<br/>
System configuration and Colias-Phi micro robot. Click the picture to see a video of the CosPhi system operation.

###How is that useful ?

The system can be used to study pheromone-based communication in robotic swarms.
In contract to other systems, CosPhi can simulate several pheromones while precisely controlling their evaporation, diffusion and release rate.
Along with the precise localization system, CosPhi allows for an affordable way to perform repeatable swarm robotics experiment.
So far, the system was used to study cue-based aggregation~[[1](#references)] and leader-follower interactions [[2,3](#references)].
  
###Can I use it ?

Yes, CosPhi is fully open-source.
The CosPhi system is composed of two software modules: the [[SwarmCon](https://github.com/gestom/CosPhi/tree/master/Localization)] module, which is a derivative of the  

###References
1. F. Arvin, T. Krajník, A. E. Turgut, and S. Yue: <b>[COSΦ: Artificial Pheromone System for Robotic Swarms Research.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_iros_pheromone.pdf)</b>In IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2015.
2. T. Krajnik, F. Arvin, A. E. Turgut, S. Yue, and T. Duckett: <b>[COSΦ: Vision-based Artificial Pheromone System for Robotic Swarms.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_icralbp_pheromone.pdf)</b> In: International Conference on Robotics and Automation. Late breaking poster session. 2015. 
3. F. Arvin, A. Attar, A. E. Turgut, and S. Yue: <b>Power-law distribution of long-term experimental data in swarm robotics.</b>In Advances in Swarm and Computational Intelligence. Springer, 2015.
