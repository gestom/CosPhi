#ifndef __CGUI_H__
#define __CGUI_H__

#include "CRawImage.h"
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

class CGui
{
public:
  CGui(int wi,int he,int scale);
  ~CGui();

  void drawImage(CRawImage* image);
  void drawTimeStats(int evalTime,int numBots);
  void saveScreen(int a);
  void update();
  void drawCircle(float cx,float cy,float r);
  void drawLine(float sx1,float sx2,float sy1,float sy2);
  void displayHelp(bool wtf);

private:
  SDL_Surface *screen;
  int width,height,scale;
  TTF_Font *smallFont;
  int averageTime,maxTime,numStats;	
};

#endif

/* end of CGui.h */
