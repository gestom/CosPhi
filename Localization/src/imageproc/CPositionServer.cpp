#include "CPositionServer.h"

#define NETWORK_BLOCK MSG_WAITALL

CPositionServer::CPositionServer()
{
	sem_init(&dataSem,0,1);	
	sem_init(&connectSem,0,1);	
	debug = true;
	numFound = numObjects = 0;
	command = SC_NONE;
	calibration = false;
	calibrationFinished = false;
	cameraHeight=fieldWidth=fieldLength=1.0;
	robotHeight=robotDiameter=0;
	stop = false;
}

CPositionServer::~CPositionServer()
{
	stop = true;
	close(mySocket);
	close(serverSocket);
}

void* connectLoop(void *serv)
{
	struct sockaddr_in clientAddr;
	socklen_t addrLen = sizeof(clientAddr);
	CPositionServer* server = (CPositionServer*) serv;
	int newServer = 0;
	bool debug = server->debug;
	while (server->stop == false)
	{
		newServer = accept(server->serverSocket, (struct sockaddr *)&clientAddr,&addrLen);
		if (newServer > -1){
			if (debug) fprintf(stdout,"Incoming connection from %s.",inet_ntoa(clientAddr.sin_addr));
			if (debug) fprintf(stdout,"Incoming connection accepted on socket level %i.",newServer);
			sem_wait(&server->connectSem);
			server->mySocket = newServer;
			sem_post(&server->connectSem);
			pthread_t* thread=(pthread_t*)malloc(sizeof(pthread_t));
			pthread_create(thread,NULL,&serverLoop,(void*)server);
		}else{
			if (debug) fprintf(stderr,"Accept on listening socked failed.");
		}
	}
	close(server->serverSocket);
	return NULL;
}

int CPositionServer::init(const char* port)
{
	int used_port = atoi(port);
	struct sockaddr_in mySocketAddr;
	mySocketAddr.sin_family = AF_INET;
	mySocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	mySocketAddr.sin_port = htons(used_port);
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0)
	{
		if (debug) fprintf(stderr,"Cannot create socket ");
		return -1;
	}
	int yes = 1;
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	if (bind(serverSocket,( struct sockaddr *)&mySocketAddr,sizeof(mySocketAddr)) < 0)
	{
		if (debug) fprintf(stderr,"Cannot bind socket.");
		return -2;
	}
	if (listen(serverSocket,1) < 0)
	{
		if (debug) fprintf(stderr,"cannot make socket listen.");
	}
	thread=(pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(thread,NULL,&connectLoop,(void*)this);
	return 0;
}

void* serverLoop(void* serv)
{
	char buffer[10000];
	CPositionServer* server = (CPositionServer*) serv;
	sem_wait(&server->connectSem);
	int socket = server->mySocket;
	sem_t *sem = &server->dataSem;
	sem_post(&server->connectSem);
	bool connected = true;
	while (connected && server->stop == false){
		/*send robot positions*/
		sem_wait(sem);
		sprintf(buffer,"Detected %i %i\n",server->numFound,server->numObjects);
		STrackedObject o;
		for (int i=0;i<server->numObjects;i++){
			o=server->object[i];
			sprintf(buffer,"%sRobot %03i %.3f %.3f %.3f\n",buffer,i,o.x,o.y,o.yaw*180/M_PI);
		}
		if (server->calibrationFinished)
		{
			sprintf(buffer,"%sCalibrated\n",buffer);
			server->calibrationFinished = false;
		}
		sem_post(sem);
		if (server->stop) sprintf(buffer,"%sClosed\n",buffer);
		if (send(socket,buffer,strlen(buffer),MSG_NOSIGNAL) != (int)strlen(buffer)) {
			connected = false;
			fprintf(stderr,"Network error - could not send data over a socket \n");
		}

		/*check for incoming commands - specific for communitation with the artificial pheromone system*/
		int numBytes = 0;
		ioctl(socket,FIONREAD,&numBytes);
		if (numBytes > 0){
			char data[numBytes];
			memset(data,0,numBytes);
			int lengthReceived = recv(socket,data,numBytes,NETWORK_BLOCK);
			if (lengthReceived > 0){
				char *token;
				token = strtok(data, "\n");
				while( token != NULL ) 
				{
					if (strncmp(token,"Calibrate",9)==0)
					{
						sem_wait(sem);
						sscanf(token,"Calibrate %i %f %f %f %f %f\n",&server->numObjects,&server->fieldLength,&server->fieldWidth,&server->cameraHeight,&server->robotDiameter,&server->robotHeight);
						server->command = SC_CALIBRATE;
						sem_post(sem);
					}
					token = strtok(NULL, "\n");
				}
			}
		}

		usleep(30000);
	}
	shutdown(socket,SHUT_WR);
	return NULL;
}

EServerCommand CPositionServer::getCommand()
{
	sem_wait(&dataSem);
	EServerCommand result = command;
	command = SC_NONE;
	sem_post(&dataSem);
	return result;
}

void CPositionServer::finishCalibration()
{
	calibrationFinished = true;
}

void CPositionServer::setNumOfPatterns(int numF,int numO)
{
	if ((numObjects != numO || numFound != numF) && command != SC_CALIBRATE){
		sem_wait(&dataSem);
		numFound = numF;
		numObjects = numO;
		if (numObjects > NUM_OBJECTS) numObjects = NUM_OBJECTS;
		if (numObjects < 0) numObjects = 0;
		sem_post(&dataSem);
	}
}

int CPositionServer::updatePosition(STrackedObject o,int num)
{
	if (num < numObjects && num >= 0){
		sem_wait(&dataSem);
		object[num] = o;
		sem_post(&dataSem);
	}
	return 0;
}

int CPositionServer::closeConnection(int socket)
{
	close(socket);
	connected = false;
	return 0;
}

