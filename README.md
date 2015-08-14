# CosPhi: Vision-based Artificial Pheromone System for Robotic Swarms

Artificial pheromone system

Pheromone-based communication is one of the most effective ways of communication widely observed in nature.
It is particularly used by social insects such as bees, ants and termites, both for inter-agent and agent-swarm communications.
Due to their effectiveness, artificial pheromones have been adopted in multi-robot and swarm robotic systems for more than a decade.
Although these artificial pheromones were implemented by different means such as chemical (use of particular chemical compounds) or physical (RFID tags, light, sound) cues, none of them were able to replicate all the aspects of pheromones as seen in nature.
We propose a novel artificial pheromone system that is reliable, accurate and uses off-the-shelf components only~-- an LCD screen and a low-cost USB camera.
The horizontally-positioned LCD displays the spatial distribution of the pheromones, which are sensed by the robots that move on the screen.
The pheromones are released according to the robots' positions provided by the overhead camera. 
The system configuration is shown in Figure~\ref{fig:colias}.

The main advantage of our system is its ability to simulate multiple pheromones and their interactions in real time.
The system allows to alter the parameters of the individual pheromones (diffusion, evaporation, etc.) on the fly, which allows for precisely controllable, repeatable experiments.
Each robot can release several pheromones at the same time depending on user-specified conditions, e.g. the distance from a food source or another robot.
The combined effect of the pheromones is displayed on a horizontal LCD screen  as a color image, which is sensed by the robots' light sensors. 

To release the pheromones at the positions of the robots, each robot is localized by a visual-based subsystem based on an overhead camera.
The core of the localization system is a freely available software package capable of fast and precise tracking of a large number of robots~\cite{whycon_2014_JINT}. 

The behaviour of each pheromone is determined by four parameters: injection $\iota$, which defines how fast a particular pheromone is released by a given robot, evaporation $e_\phi$, which determines how quickly the pheromone strength fades over time, diffusion $\kappa$, that defines the rate at which the pheromone is spreading, and influence $c$, which characterizes how much the pheromone influences the image displayed on the horizontal screen.

Given that the image displayed on the screen is represented as a matrix $\mathbf{I}$, the brightness of a pixel at a position $(x,y)$ is presented as $\mathbf{I}(x,y)$, and the $i^{th}$ pheromone is modeled as a matrix $\mathbf{\Phi_i}$, the brightness of each pixel that is displayed on the horizontal screen is given by 
%
\begin{equation}\label{eqn:pherofield}
	\mathbf{I}(x,y)=\sum_{i=1}^{n}c_i\mathbf{\Phi_i}(x,y) \; ,
\end{equation}
%
where $\mathbf{\Phi_i}(x,y)$ is a 2D array that represents the $i^{th}$ pheromone's intensity at location $(x,y)$ and $c_i$ defines the pheromone's influence on the displayed image. 
Note that the values of $c_i$ can be both positive and negative, which allows the pheromones both to amplify and to suppress each other.
%
\begin{figure}[!ht]
	\centering
	\includegraphics[width=0.95\columnwidth]{fig/col-field} 
	\caption{System configuration and Colias-$\Phi$ micro robot.}\label{fig:colias}
\vspace{-2mm}
\end{figure}
%
Compared to the previous works, our method improves the state-of-the-art by providing \textit{(a)} high resolution and precise trails due to precise robot localization and the high resolution LCD screen, \textit{(b)} the ability to accurately control pheromone pheromone diffusion, evaporation and release, \textit{(c)} simulation of the interaction of multiple pheromones, which can amplify or suppress each other, and \textit{(d)} the ability to display several pheromone types encoded by their color.

The performance of the system was tested using the \textit{Colias} platform~\cite{Colias-14} (Fig.~\ref{fig:colias}) in single-robot and swarm scenarios, (see article~\cite{pheromone_1} and video \cite{video}).
We could conclude that the proposed system is feasible as a tool for bio-inspired swarm robotics research. 
To allow the swarm robotics community to use the system for their research, we plan to provide it as a freely available open-source package.
