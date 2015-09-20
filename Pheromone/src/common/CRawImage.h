#ifndef CIMAGE_H
#define CIMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CTimer.h>
#include <math.h>
#include <CPheroField.h>

/**
@author Tom Krajnik
@about a simple class to create bitmap images from the pheromones
*/
class CRawImage
{
public:

  CRawImage(int wi,int he);
  CRawImage(unsigned char *datai,int wi,int he);
  ~CRawImage();

  /*combine pheromones into a grayscale image according to their influence*/
  void combinePheromones(CPheroField *p[],int numPheromones,int color);

  void saveBmp(const char* name);
  void saveBmp();
  bool loadBmp(const char* name);
  void swap();


  void plotLine(int x,int y);
  void plotCenter();

  int  getSaveNumber();

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
