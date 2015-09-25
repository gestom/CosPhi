#ifndef CPOSITIONCLIENT_H
#define CPOSITIONCLIENT_H

#define MAX_POSITIONS 1000

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "CDump.h" 
#include "CTimer.h" 

/**
@author Tom Krajnik
@this class provides a TCP-based communication interface with the WhyCon system
*/
class CPositionClient
{
public:
  CPositionClient();
  ~CPositionClient();

  /*connect to the WhyCon localization system at a given IP and PORT*/
  int init(const char *ip,const char* port);

  /*ask WhyCon to calibrate its coordinate system given the provided parameters*/
  void calibrate(int numRobots,float fieldLength,float fieldWidth,float camHeight,float robotDiameter,float robotHeight);

  /*read incoming messages*/
  int checkForData();

  /*get positions, orientations and latest detection information 
   of the individual robots provided by the WhyCon system*/
  float getX(int i);
  float getY(int i);
  float getPhi(int i);
  float getTime(int i);
  bool exists(int i);
  int getID(int i);

  /*resets detection timers*/ 
  void resetTime();

  int numSearched;
  int updates;
  float robotDiameter;
  int numDetected;
  int64_t frameTime;
  bool calibrated;

private:
  float tArray[MAX_POSITIONS];
  float xArray[MAX_POSITIONS];
  float yArray[MAX_POSITIONS];
  float pArray[MAX_POSITIONS];
  bool vArray[MAX_POSITIONS];
  int idArray[MAX_POSITIONS];

  int robotOrder;
  CTimer timer;
  int mySocket;
  TLogModule module;
};

#endif
