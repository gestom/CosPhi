#ifndef CFEROFIELD_H
#define CFEROFIELD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CTimer.h>
#include <math.h>
#define MAX_ID 1000

/**
@author Tom Krajnik
*/
class CPheroField
{
public:

  CPheroField(int wi,int he);
  ~CPheroField();

  void addCalibration();
  void recompute(float decay,float diffuse = 0);
  void add(int x, int y,int id,int num);
  void remove(int x, int y,float a,int num);
  void addTo(int x, int y,int id,int num);

  int width;
  int height;
  int size;
  int *lastX; 
  int *lastY;

  CTimer timer;
  float* data;
};

#endif
