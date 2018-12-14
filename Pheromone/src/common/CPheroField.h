#ifndef CFEROFIELD_H
#define CFEROFIELD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CTimer.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#define MAX_ID 1000

/**
@author Tom Krajnik
@about class that emulates pheromone spatial and temporal distribution of artificial pheromones  
for details, see paper Arvin, Krajnik, Turgut, Yue: "CosPhi: Artificial Pheromone System for Robotic Swarms Research", IROS 2015
if you use this class, please cite the aforementioned paper
*/
class CPheroField
{
public:

  /*create the field with given dimensions and parameters
   * pheromone field 0 simulates a longer-decay pheromone that the other robots follow
   * pheromone field 1 is released by the leader if it gets too close to arena boundaries causing the leader to avoid them - this pheromone decays quickly
   * pheromone field 2 is released by the leader to supress pheromone field 0 (this avoids the leader to detect pheromone 0 by its sensors)
   *evaporation defines pheromone's half-life, diffusion its spreading over time and strength determines how the pheromone influences the LCD-displayed image
   for details, see the chapter 2 of paper Arvin, Krajnik, Turgut, Yue: "CosPhi: Artificial Pheromone System for Robotic Swarms Research", IROS 2015*/
  CPheroField(int wi,int he,float evaporation,float diffusion, float influence);
  ~CPheroField();

  /*apply evaporation and diffusion*/
  void recompute();

  /*inject pheromone to a given spot*/
  void add(int x, int y,int id,int num,int radius);

  /*inject pheromone around every pixel on a line
    between the last and current injected position*/
  void addTo(int x, int y,int id,int num,int radius = 25);

  /*read pheromone value at a given spot*/
  float get(int x, int y);

  /*clear the entire pheromone field*/
  void clear();

  float evaporation;		//pheromone half-life
  float diffusion;		//pheromone spreading rate
  float influence;		//phromone influence

  int width;
  int height;
  int size;
  int *lastX; 
  int *lastY;

  CTimer timer;
  float* data;
};

#endif
