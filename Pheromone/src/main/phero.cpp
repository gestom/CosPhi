#include <stdlib.h>
#include "CGui.h"
#include "CDump.h"
#include "CTimer.h"
#include "CRawImage.h"
#include "CPheroField.h"
#include "CPositionClient.h"
#include <SDL/SDL.h>

#define MAX_PATTERNS 400 
#define CALIB_STEPS 5 

//Adjust camera resolution here
int  imageWidth= 1920;
int  imageHeight = 1080;
int pos = 0;
int numSaved = 0;
bool stop = false;
CGui* gui;
CRawImage *image;
SDL_Event event;
int moveOne = 0;
CTimer globalTimer;
int keyNumber = 1000;
Uint8 lastKeys[1000];
Uint8 *keys = NULL;
bool leftMousePressed = false;
bool rightMousePressed = false;
CPheroField* pherofield[100];
CPositionClient* client;
int numBots = 1;
int pheroStrength = 50;
bool placement = true;
bool eraseHormone = true;
int circleStrength = 255;
int robX,robY;
int trajectoryX[100000];
int trajectoryY[100000];
int trajectoryLength = 0;
int trajectoryPosition = 0;
bool calibration = true;
bool travelled = true;

void loadTrajectory()
{
	FILE* file = fopen("trajectory.txt","r");
	trajectoryLength = 0;
	while (feof(file) == 0)
	{
		fscanf(file,"%i %i\n",&trajectoryX[trajectoryLength],&trajectoryY[trajectoryLength]);
		trajectoryLength++;
	}
	fclose(file);
}

bool getTrajectoryPoint()
{
	float dist = 0;
	do{
		dist = sqrt((trajectoryX[trajectoryPosition]-robX)*(trajectoryX[trajectoryPosition]-robX)+(trajectoryY[trajectoryPosition]-robY)*(trajectoryY[trajectoryPosition]-robY));
		if (dist < 45) trajectoryPosition++;
		if (trajectoryPosition > trajectoryLength/2) travelled = true;
	}while (dist < 45 && trajectoryPosition<trajectoryLength); 
	if (trajectoryPosition<trajectoryLength) return true; else {
		trajectoryPosition = 0;
		if (travelled) eraseHormone = true;
	}
	return false; 
}

bool getlastTrajectoryPoint()
{
	float dist = 0;
	do{
		dist = sqrt((trajectoryX[trajectoryPosition]-robX)*(trajectoryX[trajectoryPosition]-robX)+(trajectoryY[trajectoryPosition]-robY)*(trajectoryY[trajectoryPosition]-robY));
		if (dist > 45) trajectoryPosition++;
		if (trajectoryPosition > trajectoryLength/2) travelled = true;
	}while (dist > 45 && trajectoryPosition<trajectoryLength); 
	if (trajectoryPosition<trajectoryLength) return true; else trajectoryPosition = 0;
	return false; 
}

void processKeys()
{
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_MOUSEBUTTONDOWN){
			if (event.button.button == SDL_BUTTON_LEFT){
				  leftMousePressed = true;
				  //pherofield[0]->add(event.motion.x,event.motion.y,0,pheroStrength);
				  robX = event.motion.x;
				  robY = event.motion.y;
			}
			if (event.button.button == SDL_BUTTON_RIGHT){
				rightMousePressed = true;
				//pherofield[1]->add(event.motion.x,event.motion.y,1,pheroStrength);
			}
		}
		if (event.type == SDL_MOUSEBUTTONUP){
			if (event.button.button == SDL_BUTTON_RIGHT)rightMousePressed = false;
			if (event.button.button == SDL_BUTTON_LEFT)leftMousePressed = false;
		}
		if (leftMousePressed){
			  //pherofield[0]->addTo(event.motion.x,event.motion.y,0,pheroStrength);
			  robX = event.motion.x;
			  robY = event.motion.y;
		}
		if (rightMousePressed) pherofield[1]->addTo(event.motion.x,event.motion.y,1,pheroStrength);
	}
	keys = SDL_GetKeyState(&keyNumber);
	if (keys[SDLK_ESCAPE]) stop = true;
	if (keys[SDLK_KP_PLUS]) circleStrength++;
	if (keys[SDLK_KP_MINUS]) circleStrength--;
	if (keys[SDLK_SPACE] && lastKeys[SDLK_SPACE] == false) placement = placement && false;
	if (keys[SDLK_c]) calibration = false;
	if (keys[SDLK_p] && lastKeys[SDLK_p] == false) moveOne = 1;
	if (keys[SDLK_s] && lastKeys[SDLK_s] == false) image->saveBmp();
	memcpy(lastKeys,keys,keyNumber);
}

int main(int argc,char* argv[])
{
	loadTrajectory();
	dump = new CDump(NULL,256,1000000);
	for (int i = 0;i<3;i++) pherofield[i] = new CPheroField(imageWidth,imageHeight);
	image = new CRawImage(imageWidth,imageHeight);
	client = new CPositionClient();
	gui = new CGui(imageWidth,imageHeight,1);
	client->init(argv[1],"6666");
	image->getSaveNumber();
	float initX[100];
	float initY[100];
	float initA[100];
	for (int i = 0;i<numBots;i++)
	{
		initX[i] = rand()%1880;
		initY[i] = rand()%1040;
	}
	float angle = 0;
	placement = false;
	eraseHormone = false;
	int xa,ya;
	FILE* file = fopen("record.txt","w+");
	while (stop == false){
		//robX = client->getX(0)*1920/0.92;
		//robY = client->getY(0)*1080/0.52;
		//angle = atan2((robY-1080/2)/2,(robX-1920/2)/3)+0.10;
		if (eraseHormone){
			if (getlastTrajectoryPoint())
			{	
				xa = trajectoryX[trajectoryPosition]; 
				ya = trajectoryY[trajectoryPosition];	
				float phi = atan2(ya-trajectoryY[(trajectoryPosition+10)%trajectoryLength],xa-trajectoryX[(trajectoryPosition+10)%trajectoryLength]);
				if (calibration == false) pherofield[0]->remove(xa,ya,phi,pheroStrength);
			}

		}else{
			if (getTrajectoryPoint())
			{
				xa = trajectoryX[trajectoryPosition]; 
				ya = trajectoryY[trajectoryPosition];		
				if (calibration == false) pherofield[0]->addTo(xa,ya,0,pheroStrength);
			}
		}
		printf("Trajectory %i %i %i %i\n",trajectoryPosition,trajectoryLength,robX,robY);
		//pherofield[0]->addTo(robX,robY,0,pheroStrength);
		//for (int i = 0;i<numBots;i++) pherofield[0]->addTo(client->getX(i)*1920/0.92,client->getY(i)*1080/0.52,i,pheroStrength);
		//pherofield[0]->addTo(client->getX(6)/0.92*1920,client->getY(6)*1080/0.52,6,pheroStrength);
		for (int i = 0;i<numBots;i++) printf("Robot %i %i %i \n",i,(int)(client->getX(i)*1920/0.92),(int)(client->getY(i)*1080/0.52));
		//DECAY set here
		//for (int i = 0;i<3;i++) 
		pherofield[0]->recompute(atof(argv[2]),0.00);
		
		image->generate(pherofield[0],0);
		if (calibration) image->displayCalibration();
		for (int i = 0;i<numBots && calibration ==false && placement;i++)
		{
			image->displayRobotFull(initX[i],initY[i],initA[i],circleStrength);
		}
		gui->drawImage(image);
		if (client->checkForData() > 0){
			//fprintf(file,"%i %i\n",robX,robY);
			calibration = false;
		}
		//gui->saveScreen(runs);
		//printf("Success rate: %f (%i of %i) %i %i \n",(float) correct/(correct+fails),correct,correct+fails,moveOne,numBots);
		//printf("Drawing results time: %i ms.\n",globalTimer.getTime());
		gui->update();
		//printf("GUI update time: %i ms.\n",globalTimer.getTime());
		processKeys();
		printf("%i\n",circleStrength);
	}
	fclose(file);
	for (int i = 0;i<3;i++) delete pherofield[i];
	delete image;
	delete gui;
	return 0;
}
