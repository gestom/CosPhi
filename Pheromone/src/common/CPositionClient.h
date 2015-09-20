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
*/
class CPositionClient
{
public:
  CPositionClient(int numBots,float diam);
  ~CPositionClient();

  int init(const char *ip,const char* port);
  void calibrate(int numRobots,float fieldLength,float fieldWidth,float camHeight,float robotDiameter,float robotHeight);

  int checkForData();
  float getX(int i);
  float getY(int i);
  float getTime(int i);
  float getPhi(int i);
  bool exists(int i);
  void resetTime();

  int numSearched;
  float robotDiameter;
  int numDetected;
  bool calibrated;

private:
  float tArray[MAX_POSITIONS];
  float xArray[MAX_POSITIONS];
  float yArray[MAX_POSITIONS];
  float pArray[MAX_POSITIONS];
  bool vArray[MAX_POSITIONS];


  CTimer timer;
  int mySocket;
  TLogModule module;
};

#endif
