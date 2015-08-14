#include "CPositionServer.h"

CPositionServer::CPositionServer()
{
	sem_init(&dataSem,0,1);	
	sem_init(&connectSem,0,1);	
	debug = true;
	numObjects = 0;
}

CPositionServer::~CPositionServer()
{
}

void* connectLoop(void *serv)
{
	struct sockaddr_in clientAddr;
	socklen_t addrLen = sizeof(clientAddr);
	CPositionServer* server = (CPositionServer*) serv;
	int newServer = 0;
	bool debug = server->debug;
	while (true)
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
	while (connected){
		sem_wait(sem);
		buffer[0] = 0;
		STrackedObject o;
		for (int i=0;i<server->numObjects;i++){
			o=server->object[i];
			//sprintf(buffer,"%s020-000-021-%03i,%.3f,%.3f,%.3f,%.3f\n",buffer,i,o.x,o.y,o.z,o.error);//Ubisense server emulation
			sprintf(buffer,"%s%03i %.3f %.3f %.3f\n",buffer,i,o.x,o.y,o.yaw*180/M_PI);//Ubisense server emulation
		}
		if (send(socket,buffer,strlen(buffer),MSG_NOSIGNAL) != (int)strlen(buffer)) {
			connected = false;
			fprintf(stderr,"Network error");
		} 
		sem_post(sem);
		usleep(30000);
	}
	server->closeConnection(socket);
	return NULL;
}

void CPositionServer::setNumOfPatterns(int num)
{
	if (numObjects != num){
		sem_wait(&dataSem);
		numObjects = num;
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

