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
  void generate(CPheroField *pA,CPheroField *pB,CPheroField *pC,int c);

  void plotLine(int x,int y);
  void plotCenter();

  int  getSaveNumber();

  double getOverallBrightness(bool upperHalf);
	  
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
