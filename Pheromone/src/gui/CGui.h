#ifndef __CGUI_H__
#define __CGUI_H__

#include "CRawImage.h"
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

/*
author Tom Krajnik tkrajnik@lincoln.ac.uk
CGui uses the SDL library to display the pheromones field and to guide the user through the experiment setup
*/
class CGui
{
public:
  /*GUI initialization
   if dualMonitor = false, then GUI goes fullscreen and width and height are autodetected from the current settings
   if dualMonitor = true, then GUI appears in a window with a given resolution*/
  CGui(int *width,int *height,bool dualMonitor);

  /*GUI destructor*/
  ~CGui();

  /*draws the image that combined the pheromones*/
  void drawImage(CRawImage* image);

  /*displays the calibration circles the the display corners + WhyCon information*/
  void displayCalibrationInfo(float camHeight,int numBots,int numVisible,int radius,int refreshTime);

  /*displays the positions of the robots to place when starting an experiment*/
  void displayInitialPositions(int x, int y,float phi,int id,int radius);
  /*provides additional information about robot placement*/
  void displayPlacementInfo(int numBots,int numVisible);
  /*displays the outline of the detected robot - used to verify if the WhyCon system works correctly*/
  void displayRobot(int x, int y,float phi,int id,int radius);

  /*displays a robot pattern at a given place - used to determine overall system lag*/
  void displayPattern(int x, int y, int radius);

  /*GUI update - causes the GUI to be drawn on the screen*/
  void update();

  /*saves the screen to the 'output' directory*/
  void saveScreen(int a);

private:
  SDL_Surface *screen;
  int width,height;
  TTF_Font *smallFont;
  int averageTime,maxTime,numStats;	
};

#endif

/* end of CGui.h */
