/*
 * File name: CPositionsServer.h
 * Date:      2013/06/06
 * Author:   Tom Krajnik 
 */

#ifndef __CPOSITIONSERVER_H__
#define __CPOSITIONSERVER_H__

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "CTransformation.h"
#include <semaphore.h>
#include <pthread.h>

#define NUM_CONNECTIONS 100
#define NUM_OBJECTS 100

void* serverLoop(void* serv);

class CPositionServer{
	public:

		CPositionServer();
		~CPositionServer();
		int init(const char* port);
		int updatePosition(STrackedObject object,int i);
		int sendPosition(int socket);
		int closeConnection(int socket);
		void setNumOfPatterns(int num);

		int connected;
		int serverSocket;
		int mySocket;
		STrackedObject object[NUM_OBJECTS];
		sem_t dataSem,connectSem;
		bool debug;
		int numObjects;
		pthread_t* thread;
};
#endif
/* end of CPositionServer.h */

