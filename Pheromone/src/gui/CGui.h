#ifndef __CGUI_H__
#define __CGUI_H__

#include "CRawImage.h"
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class CGui
{
public:
  CGui(int *wi,int *he);
  ~CGui();

  void drawTimeStats(int evalTime,int numBots);
  void displayCalibrationInfo(int numBots);
  void drawImage(CRawImage* image);
  void displayInitialPositions(int x, int y,float phi,int id,int radius);
  void displayCalibrationInfo(float camHeight,int numBots,int numVisible);
  void saveScreen(int a);
  void update();


  void displayRobot(int x, int y,int phi,int id);
  void drawCircle(float cx,float cy,float r);
  void drawLine(float sx1,float sx2,float sy1,float sy2);
  void displayHelp(bool wtf);

private:
  SDL_Surface *screen;
  int width,height;
  TTF_Font *smallFont;
  int averageTime,maxTime,numStats;	
};

#endif

/* end of CGui.h */
