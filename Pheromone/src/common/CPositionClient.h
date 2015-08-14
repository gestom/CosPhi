#ifndef CPOSITIONCLIENT_H
#define CPOSITIONCLIENT_H

#define MAX_POSITIONS 1000

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "CDump.h" 


/**
@author Tom Krajnik
*/
class CPositionClient
{
public:
  CPositionClient();
  ~CPositionClient();

  int init(const char *ip,const char* port);
  int checkForData();
  float getX(int i);
  float getY(int i);

private:
  float xArray[MAX_POSITIONS];
  float yArray[MAX_POSITIONS];

  int mySocket;
  TLogModule module;
};

#endif
