#include <stdlib.h>
#include <signal.h>
#include "CGui.h"
#include "CDump.h"
#include "CTimer.h"
#include "CRawImage.h"
#include "CPheroField.h"
#include "CPositionClient.h"
#include <SDL/SDL.h>

#define MAX_ROBOTS 100

/*these values need to be adjusted by the user during the system set-up*/
float arenaLength     = 0.92;	//screen width (or arena length) in meters
float arenaWidth      = 0.52;	//screen height (or arena width) in meters
float cameraHeight    = 1.0;	//camera height above the screen
float robotHeight     = 0.02;	//height of the pattern from robot's base
float robotDiameter   = 0.04;	//robot diameter
char whyconIP[] = "localhost";	//IP of a machine that runs the localization

/*logging and experiment control*/
CTimer globalTimer;		//used to terminate the experiment after a given time
int experimentTime = 180;	//experiment duration is 3 minutes by default
FILE *robotPositionLog = NULL;	//file to log robot positions
bool calibration = true;	//re-calibrate the localization system each time 
bool placement = true;		//randomly generate initial positions of robots at the experiment start
float initX[MAX_ROBOTS];	//initial positions
float initY[MAX_ROBOTS];	//initial positions
float initA[MAX_ROBOTS];	//initial orientations 
int initBorder = 100;		//defines minimal distance of the randomly-generated initial positions from the arena boundary
int initBrightness = 255;	//brightness of the patterns at randomly-generated positions
int initRadius = 50;		//radius of the randomly-generated positions and robots
int pheroStrength = 50;		//default pheromone strength released by the leader robot

/*variables read from the command line*/
int numBots = 1;		//number of robots in the arena
float evaporation = 1;		//main pheromone half-life	[s]
float diffusion = 0.0;		//main pheromone diffusion	- not implemented

/*supporting classes and variables*/
CGui* gui;
CRawImage *image;
CPheroField* pherofield[MAX_ROBOTS];
CPositionClient* client;
char logFileName[1000];

/*GUI-related variables*/
bool stop = false;
SDL_Event event;
int keyNumber = 1000;
Uint8 lastKeys[1000];
Uint8 *keys = NULL;
bool leftMousePressed = false;
bool rightMousePressed = false;
int  imageWidth= 1920;			//autodetected later on
int  imageHeight = 1080;		//autodetected later on

/*CTRL-C handler*/
void ctrl_c_handler(int a)
{
	stop = true;
}

/*randomly generate initial positions of the robots*/
bool randomPlacement()
{	srand(time(NULL));
	int i = 0;
	globalTimer.reset();
	globalTimer.start();
	while (i < numBots)
	{
		//generate position
		initX[i] = rand()%(imageWidth-initBorder*2)+initBorder;
		initY[i] = rand()%(imageHeight-initBorder*2)+initBorder;
		initA[i] = (rand()%628)/100.0;
		//check for overlap
		bool distanceOK = true;
		for (int j = 0;j<i && distanceOK;j++){
			if (pow(initX[i]-initX[j],2)+pow(initY[i]-initY[j],2) < pow(initRadius*2,2)) distanceOK = false; 
		}
		if (distanceOK) i++;
		if (globalTimer.getTime() > 2000000)	//try for 2 seconds
		{
			fprintf(stderr,"Could not find random, non-overlaping placements for %i robots. Reduce the number of robots or try again.\n",numBots);
			exit(-1); 
		}
	}
	globalTimer.reset();
	globalTimer.pause();
	return true;
}

/*process mouse and keyboard events coming from the GUI*/
void processEvents()
{
	keys = SDL_GetKeyState(&keyNumber);
	SDL_ShowCursor(leftMousePressed||rightMousePressed);

	//mouse events - allows to use the mouse to draw pheromone track 
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_MOUSEBUTTONDOWN){
			if (event.button.button == SDL_BUTTON_LEFT)leftMousePressed = true;
			if (event.button.button == SDL_BUTTON_RIGHT) rightMousePressed = true;
		}
		if (event.type == SDL_MOUSEBUTTONUP){
			if (event.button.button == SDL_BUTTON_RIGHT)rightMousePressed = false;
			if (event.button.button == SDL_BUTTON_LEFT)leftMousePressed = false;
		}
		if (leftMousePressed) pherofield[0]->add(event.motion.x,event.motion.y,0,pheroStrength,35);
		if (rightMousePressed) pherofield[1]->addTo(event.motion.x,event.motion.y,1,pheroStrength,35);
	}

	//terminate 
	if (keys[SDLK_ESCAPE]) stop = true;
	if ((keys[SDLK_LCTRL] || keys[SDLK_RCTRL]) && keys[SDLK_c]) stop = true;

	//press space to start the experiment
	if (keys[SDLK_SPACE] && lastKeys[SDLK_SPACE] == false && calibration == false){
		placement = false;
		globalTimer.reset();
		globalTimer.start();
		client->resetTime();
	}

	//clear pheromone fields
	if (keys[SDLK_c]){
	       	pherofield[0]->clear();
	       	pherofield[1]->clear();
	       	pherofield[2]->clear();
	}

	//generate new random positions to start
	if (keys[SDLK_p] && lastKeys[SDLK_p] == false) randomPlacement();

	//save an image
	if (keys[SDLK_s] && lastKeys[SDLK_s] == false) image->saveBmp();
	memcpy(lastKeys,keys,keyNumber);
}

/*initialize logging*/
bool initializeLogging()
{
	//initialize logging system
	dump = new CDump(NULL,256,1000000);

	char timeStr[100];
	time_t timeNow;
	time(&timeNow);
	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d_%H-%M-%S",localtime(&timeNow));
	sprintf(logFileName,"output/Phero_%.3f_%s.txt",evaporation,timeStr);
	robotPositionLog = fopen(logFileName,"w");
	if (robotPositionLog == NULL)
	{
		fprintf(stderr,"Cannot open log file %s. Does the \"output\" directory exist?\n",logFileName);
		return false;
	}
	return true;
}

/*log robot positions for further analysis*/
void logRobotPositions()
{
	/*get robot positions from the localization system*/
	float x[numBots],y[numBots];
	for (int i = 0;i<numBots;i++){
		x[i] = client->getX(i);
		y[i] = client->getY(i);
	}

	/*and save robots' positions and relative distances to each other in a file for later analysis*/
	for (int i = 0;i<numBots;i++){;
		int indexX = (int)(client->getX(i)*imageWidth/arenaLength);
		int indexY = (int)(client->getY(i)*imageHeight/arenaWidth);
		if (client->exists(i) && x[i] > 0 && y[i] > 0 && x[i] < 1 && y[i] < 1 ){
			fprintf(robotPositionLog,"Robot %01i %08.3f %06.3f %06.3f %06.3f %06.0f ",i,client->getTime(i),client->getX(i),client->getY(i),client->getPhi(i),pherofield[0]->get(indexX,indexY));
			for (int j = 0;j<numBots;j++)fprintf(robotPositionLog,"%.3f ",sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
			fprintf(robotPositionLog,"\n");
		}
	}
}

int main(int argc,char* argv[])
{
	//register ctrl+c handler
	signal (SIGINT,ctrl_c_handler);
	//initialize the logging system
	if (initializeLogging()==false) return -1;

	//auto-detect screen resolution and initialize the GUI
	gui = new CGui(&imageWidth,&imageHeight);
	image = new CRawImage(imageWidth,imageHeight);

	//read number of robots and pheromone half-life from the command line
	numBots = atoi(argv[1]);
	float evaporation = atof(argv[2]);

	float diffusion = 0;
	float influence = 1.0;
	/*initialize the pheromone fields
	* pheromone field 0 simulates a longer-decay pheromone that the other robots follow
	* pheromone field 1 is released by the leader if it gets too close to arena boundaries causing the leader to avoid them - this pheromone decays quickly
	* pheromone field 2 is released by the leader to supress pheromone field 0 (this avoids the leader to detect pheromone 0 by its sensors)
	*evaporation defines pheromone's half-life, diffusion its spreading over time and strength determines how the pheromone influences the LCD-displayed image
	for details, see the chapter 2 of paper Arvin, Krajnik, Turgut, Yue: "CosPhi: Artificial Pheromone System for Robotic Swarms Research", IROS 2015*/
	pherofield[0] = new CPheroField(imageWidth,imageHeight,evaporation,diffusion,influence);
	pherofield[1] = new CPheroField(imageWidth,imageHeight,0.1,0,1);
	pherofield[2] = new CPheroField(imageWidth,imageHeight,0.1,0,-2);

	/*connect to the localization system*/
	client = new CPositionClient(numBots,robotDiameter);
	client->init(whyconIP,"6666");
	image->getSaveNumber();

	float nearest = 0.10;
	randomPlacement();

	globalTimer.pause();
	while (stop == false){
		stop = (globalTimer.getTime()/1000000>experimentTime);
		if (calibration==false && placement==false)
		{
			/*cause the leading robot to release the pheromone 0 that other robots follow*/
			for (int i = 0;i<1;i++)pherofield[0]->addTo(client->getX(i)*imageWidth/arenaLength,client->getY(i)*imageHeight/arenaWidth,i,pheroStrength);
	
			/*cause the leading robot to release pheromone 1 that is used for obstacle avoidance and 2 that temporarily suppresses pheromone 0*/
			float dist = 0.030;	//distance of the pheromone release relatively to the leader (controls pheromones 1 and 2 only) 
			float addPhi = 0;	//angle of the pheromone release relatively to the leader (controls pheromones 1 and 2 only) 
			float phi = client->getPhi(0);
		
			/*is the leader close to the arena edge ?*/
			if ((client->getX(0)<nearest && cos(phi)<0) || (client->getX(0)>arenaLength-nearest && cos(phi) > 0 )|| (client->getY(0)<nearest && sin(phi)<0) || (client->getY(0)>arenaWidth-nearest && sin(phi)>0))
			{
				/*leader is close to the arena edge -> release pheromone 1 that causes the robot to turn away */
				pherofield[1]->addTo((client->getX(0)+dist*cos(phi+addPhi))*imageWidth/arenaLength,(client->getY(0)+dist*sin(phi+addPhi))*imageHeight/arenaWidth,0,pheroStrength,35);
			}else{
				/*leader is not close to the arena edge -> release pheromone 2 suporessed pheromone 0, so that the leader does not pick it's own pheromone */
				pherofield[2]->addTo((client->getX(0)+dist*cos(phi+addPhi))*imageWidth/arenaLength,(client->getY(0)+dist*sin(phi+addPhi))*imageHeight/arenaWidth,0,pheroStrength,45);
			}
			/*save positions for later analysis*/
			logRobotPositions();
		}

		//calculate the pheromone decay 
		pherofield[0]->recompute();	//main pheromone half-life (user-settable, usually long)
		pherofield[1]->recompute();		//collision avoidance pheromone with quick decay
		pherofield[2]->recompute();		//suppression pheromone with quick decay

		//convert the pheromone field to grayscale image
		image->combinePheromones(pherofield,3,0);		//the last value determines the color channel - 0 is for grayscale, 1 is red etc. 
		gui->drawImage(image);

		//experiment preparation phase 2: draw initial and real robot positions
		for (int i = 0;i<numBots && placement;i++)
		{
			 gui->displayInitialPositions(initX[i],initY[i],initA[i],initBrightness,initRadius);
			 if (client->exists(i))  gui->displayRobot(client->getX(i)*imageWidth/arenaLength,client->getY(i)*imageHeight/arenaWidth,client->getPhi(i),0);
		}
	
		//experiment preparation phase 1: draw calibration, contact WhyCon to calibrate and draw initial robot positions
		if (calibration){
			 gui->displayCalibrationInfo(cameraHeight,client->numSearched,client->numDetected);
			 client->calibrate(numBots,arenaLength,arenaWidth,cameraHeight,robotDiameter,robotHeight);
		}else if (placement){
			 gui->displayPlacementInfo(client->numSearched,client->numDetected);
		}

		//get the latest data from localization system and check if the calibration finished
		client->checkForData();
		calibration = client->calibrated==false;

		//update GUI etc
		gui->update();
		processEvents();
	}
	fclose(robotPositionLog);
	if (globalTimer.getTime()/1000000<experimentTime) {
		remove(logFileName); 
		printf("EXPERIMENT TERMINATED MANUALLY\n");
	}else{
		printf("EXPERIMENT FINISHED SUCESSFULLY, results saved to %s.\n",logFileName);
	}
	for (int i = 0;i<3;i++) delete pherofield[i];
	delete client;
	delete image;
	delete gui;
	return 0;
}
