#include "CPositionClient.h"

#define NETWORK_BLOCK MSG_WAITALL

CPositionClient::CPositionClient()
{
	module = LOG_MODULE_CLIENT;
}

CPositionClient::~CPositionClient()
{}

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

int CPositionClient::checkForData()
{
	float x,y,phi;
	int id = 0;
	int numBytes = 0;
	ioctl(mySocket,FIONREAD,&numBytes);
	//printf("NumBytes %i\n",numBytes);
	if (numBytes > 0){
		char data[numBytes];
		memset(data,0,numBytes);
		int lengthReceived = recv(mySocket,data,numBytes,NETWORK_BLOCK);
		if (lengthReceived > 0){
			char *token;
			token = strtok(data, "\n");
			while( token != NULL ) 
			{
				sscanf(token,"%i %f %f %f \n",&id,&x,&y,&phi);
				printf("%i %f %f %f \n",id,x,y,phi);
				token = strtok(NULL, "\n");
				if (id >=0 && id < MAX_POSITIONS){
					xArray[id] = x;
					yArray[id] = y;
				}
			}
		}
	}
	//  if ((unsigned int)lengthReceived == len*sizeof(int)) result = 0;
	return numBytes;
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
