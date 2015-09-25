#include "CPositionClient.h"

#define NETWORK_BLOCK MSG_WAITALL

CPositionClient::CPositionClient()
{
	module = LOG_MODULE_CLIENT;
	for (int i =0 ; i < MAX_POSITIONS;i++) vArray[i] = false;
	timer.reset();
	timer.start();
	calibrated = true;
	numDetected = -1;
	numSearched = -1;
	frameTime = -1;
	robotOrder = 0;
	updates = 0;
}

CPositionClient::~CPositionClient()
{
	close(mySocket);
}

int CPositionClient::init(const char *ip,const char* port)
{
	int result = -1;
	mySocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mySocket > 0)
	{
		struct sockaddr_in server_addr;
		struct hostent *host_info;
		host_info =  gethostbyname(ip);
		if (host_info != NULL)
		{
			server_addr.sin_family = host_info->h_addrtype;
			memcpy((char *) &server_addr.sin_addr.s_addr,host_info->h_addr_list[0], host_info->h_length);
			server_addr.sin_port = htons(atoi(port));
			result = connect(mySocket,(struct sockaddr*) &server_addr,sizeof(server_addr));
		}
	}
	return result;
}

void CPositionClient::resetTime()
{
	timer.reset();
	timer.start();
	for (int i =0 ; i < MAX_POSITIONS;i++) vArray[i] = false;
}

void CPositionClient::calibrate(int numRobots,float fieldLength,float fieldWidth,float camHeight,float robotDiameter,float robotHeight)
{
	if (calibrated == true){
		char buffer[1000];
		sprintf(buffer,"Calibrate %i %.3f %.3f %.3f %.3f %.3f \n",numRobots,fieldLength,fieldWidth,camHeight,robotDiameter,robotHeight);
		if (send(mySocket,(void*)buffer,strlen(buffer),MSG_NOSIGNAL) != (int)strlen(buffer)) fprintf(stderr,"Network error - could not send calibrate command\n");
		calibrated = false;
	}
}

int CPositionClient::checkForData()
{
	float x,y,phi;
	int id = 0;
	int numBytes = 0;
	ioctl(mySocket,FIONREAD,&numBytes);
	CTimer lagTimer;
	int64_t detectTime;
	updates=0;
	if (numBytes > 0){
		char data[numBytes+10];
		memset(data,0,numBytes);
		int lengthReceived = recv(mySocket,data,numBytes,NETWORK_BLOCK);
		if (lengthReceived > 0){
			data[lengthReceived] = 0;
			char *token;
			token = strtok(data, "\n");
			while( token != NULL ) 
			{
				if (strncmp(token,"Robot",5)==0){
					sscanf(token,"Robot %i %f %f %f %ld \n",&id,&x,&y,&phi,&detectTime);
					if (id >=0 && id < MAX_POSITIONS){
						xArray[robotOrder] = x;
						yArray[robotOrder] = y;
						pArray[robotOrder] = phi/180*M_PI;
						vArray[robotOrder] = true;
						tArray[robotOrder] = timer.getTime()/1000000.0;
						idArray[robotOrder++] = id;
					}
				}
				else if (strncmp(token,"Detected",6)==0){
				       	sscanf(token,"Detected %i of %i at %ld. \n",&numDetected,&numSearched,&frameTime);
					robotOrder=0;
					updates++;
				}
				else if (strncmp(token,"Calibrated",11)==0) calibrated = true;
				else printf("Unknown incoming data: -%s- %i\n",token,(int)strlen(token));
				token = strtok(NULL, "\n");
			}
		}
	}
	return numBytes;
}

int CPositionClient::getID(int i)
{
	if (i >= 0 && i < MAX_POSITIONS) return idArray[i];
	return -1;
}

float CPositionClient::getX(int i)
{
	if (i >= 0 && i < MAX_POSITIONS) return xArray[i];
	return 0;
}

float CPositionClient::getY(int i)
{
	if (i >= 0 && i < MAX_POSITIONS) return yArray[i];
	return 0;
}

float CPositionClient::getPhi(int i)
{
	if (i >= 0 && i < MAX_POSITIONS) return pArray[i];
	return 0;
}

bool CPositionClient::exists(int i)
{
	return vArray[i];
}

float CPositionClient::getTime(int i)
{
	return tArray[i];
}

