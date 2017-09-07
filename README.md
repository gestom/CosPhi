## CosΦ: An Artificial Pheromone System for Robotic Swarms

### Why to study pheromones?
Pheromones serve as a common means of communication for most animals.
Using pheromone-based communication, simple life-forms like bees, ants and termites form communities that exhibit complex and intelligent behaviour.
The phenomena of <b>swarm intelligence</b>, which naturally emerges from interaction of large amounts of simple individuals, is in the focus of the swarm robotics research.
Understanding pheromone-based communication is one of the keys that will allow to grasp the principles that are behind emergence of swarm intelligence.
So far, pheromone-based communication was either simulated or implemented by different chemical (volatile compounds) and physical (RFID tags, light, sound) cues, which were not able to replicate more complex aspects of pheromones as seen in nature. 

###What is CosΦ?
CosΦ is a novel artificial pheromone system that is reliable, accurate and uses off-the-shelf components only: an LCD screen and a low-cost USB camera.
The horizontally-positioned LCD displays the spatial distribution of the pheromones, which are sensed by the robots that move on the screen. 
The pheromones are released according to the robots' positions provided by the overhead camera.


[![System configuration and Colias-Φ micro robot.](https://raw.githubusercontent.com/wiki/gestom/CosPhi/images/arena.png)](https://www.youtube.com/watch?v=TkYbpEhDa58)<br/>
System configuration and Colias-Φ micro robot. Click the picture to see a video of the CosΦ system operation.

In contrast to other systems, CosΦ can simulate several pheromones while precisely controlling their evaporation, diffusion and release rate.
Along with the precise localization system [[2](#references)], CosΦ allows for an affordable way to perform <b>repeatable experiments</b> in swarm robotics.
So far, the system was used to study cue-based aggregation [[1](#references)] and leader-follower interactions [[3,4](#references)].
  
### Can I use it ?

CosΦ is fully open-source and free.
The CosΦ system is composed of two software modules: the [SwarmCon](https://github.com/gestom/CosPhi/tree/master/Localization) module, which is a derivative of the [WhyCon](http://purl.org/robotics/whycon) system [[2](#references)], and the [Pheromone simulator](https://github.com/gestom/CosPhi/tree/master/Pheromone), which simulates the individual pheromones and displays them on the horizontal LCD. Note, that the full version of the software will be released after its presentation at the IROS 2015 conference.
If you use this software for your work, please cite the two papers that describe these software modules - here is their [bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/cite.bib).

### How can I use it ?
First, get the software from this GitHub repository. 
After that, set up the [SwarmCon](https://github.com/gestom/CosPhi/tree/master/Localization) localization system.
Then, set up the [Pheromone](https://github.com/gestom/CosPhi/tree/master/Pheromone) simulator.

### References
1. F. Arvin, T. Krajník, A. E. Turgut, and S. Yue: <b>[COSΦ: Artificial Pheromone System for Robotic Swarms Research.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_iros_pheromone.pdf)</b>In IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS), 2015 [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_iros_pheromone.bib)].
2. T. Krajník, M. Nitsche et al.: <b>[A Practical Multirobot Localization System.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_JINT_whycon.pdf)</b>Journal of Intelligent and Robotic Systems (JINT), 2014 [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_JINT_whycon.bib)].
3. T. Krajnik, F. Arvin, A. E. Turgut, S. Yue, and T. Duckett: <b>[COSΦ: Vision-based Artificial Pheromone System for Robotic Swarms.](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_icralbp_pheromone.pdf)</b> In: International Conference on Robotics and Automation. Late breaking poster session. 2015 [[bibtex](http://raw.githubusercontent.com/wiki/gestom/CosPhi/papers/2015_icralbp_pheromone.bib)].
4. F. Arvin, A. Attar, A. E. Turgut, and S. Yue: <b>Power-law distribution of long-term experimental data in swarm robotics.</b>In Advances in Swarm and Computational Intelligence. Springer, 2015.
