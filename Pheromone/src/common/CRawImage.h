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
  /*allocates main data array*/
  CRawImage(int wi,int he);

  /*uses the data array pointed to by 'datai'*/
  CRawImage(unsigned char *datai,int wi,int he);
  ~CRawImage();

  /*combine pheromones into a grayscale image according to their influence*/
  void combinePheromones(CPheroField *p[],int numPheromones,int color);

  /*saving and loading - provide a name*/
  void saveBmp(const char* name);
  bool loadBmp(const char* name);

  /*saving to 'images' directory with a incremental number*/
  void saveBmp();
  int  getSaveNumber();

  /*channel swap*/
  void swap();

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
