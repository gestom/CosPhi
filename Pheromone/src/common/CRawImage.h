#ifndef CIMAGE_H
#define CIMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CTimer.h>
#include <math.h>
#include <CPheroField.h>
//#define THICK_CROSS

/**
@author Tom Krajnik
*/
class CRawImage
{
public:

  CRawImage(int wi,int he);
  CRawImage(unsigned char *datai,int wi,int he);
  ~CRawImage();
  void saveBmp(const char* name);
  void saveBmp();
  bool loadBmp(const char* name);
  void swap();
  void generate(CPheroField *phero,int chan);

  void plotLine(int x,int y);
  void plotCenter();

  int  getSaveNumber();

  double getOverallBrightness(bool upperHalf);
  void displayCalibration();
  void displayRobot(int x, int y,int phi,int i);
  void displayRobotFull(int x, int y,int phi,int i);
	  
  int width;
  int height;
  int palette;
  int size;
  int bpp;
  unsigned char header[122]; 

  CTimer timer;
  unsigned char* data;
  bool ownData;
  int numSaved;
};

#endif
