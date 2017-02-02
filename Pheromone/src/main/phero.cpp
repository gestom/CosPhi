#include <stdlib.h>
#include <signal.h>
#include "CGui.h"
#include "CDump.h"
#include "CTimer.h"
#include "CRawImage.h"
#include "CPheroField.h"
#include "CPositionClient.h"

#define MAX_ROBOTS 6 

/*
author: Tom Krajnik tkrajnik@lincoln.ac.uk,
This program is a pheromone simulator intended for swarm robotics.
Its thorough description and use-cases are provided in a conference article:
Arvin, Krajnik, Turgut, Yue: "CosPhi: Artificial Pheromone System for Robotic Swarms Research", In International Conference on Intelligent Robotic Systems (IROS), 2015.
if you will use this software for your research, please cite the aforementioned paper
*/

/*---------The following values need to be adjusted by the user during the system set-up -------------*/
float arenaLength     	= 0.92;		//screen width (or arena length) in meters
float arenaWidth      	= 0.52;		//screen height (or arena width) in meters
float cameraHeight    	= 0.85;		//camera height above the screen
float robotHeight     	= 0.025;	//height of the pattern from robot's base
float robotDiameter   	= 0.04;		//robot diameter
char whyconIP[] 	= "localhost";	//IP of a machine that runs the localization
bool dualMonitor      	= true;		//do you want the pheromone system to be displayed on a secondary screen?
int  imageWidth		= 1920;		//adjust manually in case of dualMonitor = true, otherwise leave for auto-detection
int  imageHeight 	= 1080;		//adjust manually in case of dualMonitor = true, otherwise leave for auto-detection
float  calibOffset 	= 0.02;		//move the calibration patterns by calibOffset pixels inside of the arena
/*---------The previous values need to be adjusted by the user during the system set-up -------------*/

/*---------Adjust the following variables to define your experiment duration, initial conditions etc.------------*/
int pheroRadius = 30;		//default pheromone radius released by the leader robot
int pheroStrength = 55;		//default pheromone strength released by the leader robot
int experimentTime = 600;	//experiment duration is 3 minutes by default
bool calibration = true;	//re-calibrate the localization system at each start 
bool placement = true;		//randomly generate initial positions of robots at the experiment start
int initBrightness = 255;	//brightness of the patterns at randomly-generated positions
int initBorder = 100;		//defines minimal distance of the randomly-generated initial positions from the arena boundary
int initRadius = 50;		//radius of the randomly-generated positions on the display 
float avoidDistance = 0.10;	//minimal distance to trigger pheromone 2 release - this pheromone causes the leading robot to turn away from an obstacle  
float pheroTime = 10;		
int cueSizeArray[2] 		= {100,100};	
float cueIntensityArray[2] 	= {255,255};	
float cueX[2] 	= {1000,500};	
float cueY[2] 	= {500,200};
bool randomCues = true;
float pheromoneIntensity = 0.9;		
float pheromoneMultiplier = 2.0;
float cueMultiplier = 1.0;
int numCues = 2;
/*---------Adjust the previous variables to define your experiment duration, initial conditions etc.-------------*/

/*variables read from the command line*/
int numBots = 1;		//number of robots in the arena
float evaporation = 1;		//main pheromone half-life	[s]
float diffusion = 0.0;		//main pheromone diffusion	- not implemented

/*supporting classes and variables*/
CCue *cueArray[10];
CTimer globalTimer;		//used to terminate the experiment after a given time
FILE *robotPositionLog = NULL;	//file to log robot positions
float initX[MAX_ROBOTS];	//initial positions
float initY[MAX_ROBOTS];	//initial positions
float initA[MAX_ROBOTS];	//initial orientations 
int robotCue[MAX_ROBOTS];	//cue a robot is on 
int64_t pheroTimeArray[MAX_ROBOTS];	
float pheroCount[MAX_ROBOTS];	
CPheroField* pherofield[MAX_ROBOTS];//pheromone matrices
CGui* gui;
CRawImage *image;
CPositionClient* client;
char logFileName[1000];

/*GUI-related variables*/
bool stop = false;
SDL_Event event;
int keyNumber = 1000;
Uint8 lastKeys[1000];
const Uint8 *keys = NULL;
bool leftMousePressed = false;
bool rightMousePressed = false;

/*CTRL-C handler*/
void ctrl_c_handler(int a)
{
	stop = true;
}




void setPheroTime(int i, int64_t t)
{
	if (i>=0 && i<MAX_ROBOTS) pheroTimeArray[i] = t; 
}

int64_t getPheroTime(int i)
{
	if (i>=0 && i<MAX_ROBOTS) return pheroTimeArray[i];
       return 0;	
}

float getPheroStrength(int i)
{
	if (i>=0 && i<MAX_ROBOTS){
	       	return  pheroCount[i];
	       	return  pheroStrength*(pheroTimeArray[i]-globalTimer.getTime()+pheroTime*1000000)/(pheroTime*1000000);
	}
       return 0;	
}

void addPheroCount(int i,float val)
{
	if (i>=0 && i<MAX_ROBOTS) pheroCount[i]+=pheromoneMultiplier*val;
}

void reducePheroCount(int i)
{
	if (i>=0 && i<MAX_ROBOTS){
	       	pheroCount[i]--;
		if (pheroCount[i] < 0) pheroCount[i] = 0; 
	}
}

int getPheroCount(int i)
{
	if (i>=0 && i<MAX_ROBOTS) return pheroCount[i];
	return 0;
}

void resetPheroCount(int i)
{
	if (i>=0 && i<MAX_ROBOTS) pheroCount[i] = 0;
}


/*randomly generate initial positions of the robots*/
bool randomPlacement()
{	srand(time(NULL));
	int i = 0;
	globalTimer.reset();
	globalTimer.start();
	bool distanceOK = true;
	if (randomCues){
		while (i < numCues){
			initX[i] = rand()%(imageWidth-6*cueSizeArray[i]/4)+3*cueSizeArray[i]/4;
			initY[i] = rand()%(imageHeight-6*cueSizeArray[i]/4)+3*cueSizeArray[i]/4;
			distanceOK = true;
			for (int j = 0;j<i && distanceOK;j++){
				if (pow(initX[i]-initX[j],2)+pow(initY[i]-initY[j],2) < pow(cueSizeArray[i]/2+cueSizeArray[j]/2,2)) distanceOK = false; 
			}
			if (distanceOK) i++;
			if (globalTimer.getTime() > 2000000)	//try for 2 seconds
			{
				fprintf(stderr,"Could not find random, non-overlaping placements for %i robots. Reduce the number of robots or try again.\n",numBots);
				exit(-1); 
			}
		}
		for (int i = 0;i<numCues;i++){
			delete cueArray[i];
			cueArray[i] = new CCue(initX[i]/imageWidth,initY[i]/imageHeight,cueIntensityArray[i],cueSizeArray[i]);
		}
	}
	i = 0;
	while (i < numBots)
	{
		//generate position
		initX[i] = rand()%(imageWidth-initBorder*2)+initBorder;
		initY[i] = rand()%(imageHeight-initBorder*2)+initBorder;
		initA[i] = (rand()%628)/100.0;
		//check for overlap
		distanceOK = true;
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
//	SDL_PumpEvents();
	keys = SDL_GetKeyboardState(&keyNumber);
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
		if (leftMousePressed) pherofield[0]->add(event.motion.x,event.motion.y,0,pheroStrength,pheroRadius,0);
		if (rightMousePressed) pherofield[1]->addTo(event.motion.x,event.motion.y,1,pheroStrength,pheroRadius,0);
	}

	//terminate 
	if (keys[SDL_SCANCODE_ESCAPE]) stop = true;
	if ((keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL]) && keys[SDL_SCANCODE_C]) stop = true;

	//press space to start the experiment
	if (keys[SDL_SCANCODE_SPACE] && lastKeys[SDL_SCANCODE_SPACE] == false && calibration == false){
		placement = false;
		globalTimer.reset();
		globalTimer.start();
		client->resetTime();
		for (int i = 0;i<MAX_ROBOTS;i++){
		       	pheroCount[i] = 0;
		       	setPheroTime(i,-2*pheroTime*1000000);
		}
	}
	//clear pheromone fields
	if (keys[SDL_SCANCODE_C]){
	       	pherofield[0]->clear();
	       	pherofield[1]->clear();
	       	pherofield[2]->clear();
	}

	//generate new random positions to start
	if (keys[SDL_SCANCODE_P] && lastKeys[SDL_SCANCODE_P] == false) randomPlacement();
	if (keys[SDL_SCANCODE_C] && lastKeys[SDL_SCANCODE_C] == false) calibration = true;

	//save an image
	if (keys[SDL_SCANCODE_S] && lastKeys[SDL_SCANCODE_S] == false) image->saveBmp();
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
	fprintf(robotPositionLog,"experimentTime: %i evaporation: %.3f numRobots: %i pheromoneMultiplier: %.3f pheromoneIntensity: %.3f cueMultiplier: %.3f cue0Diameter: %.3f cue0Intensity: %.3f cue1Diameter: %.3f cue1Intensity: %.3f  cue0X: %.3f cue0Y: %.3f cue1X: %.3f cue1Y: %.3f\n",experimentTime,evaporation,numBots,pheromoneMultiplier,pheromoneIntensity,cueMultiplier,cueSizeArray[0]*arenaLength/imageWidth,cueIntensityArray[0],cueSizeArray[1]*arenaLength/imageWidth,cueIntensityArray[1],cueX[0],cueY[0],cueX[1],cueY[1]);
	return true;
}

/*log robot positions for further analysis*/
void logRobotPositions()
{
	for (int i = 0;i<numCues;i++){
	       	fprintf(robotPositionLog,"Cue %01i %08.3f %.3f %.3f %.3f %.3f %i\n",i,client->getTime(0),cueArray[i]->x*arenaLength,cueArray[i]->y*arenaWidth,cueArray[i]->diameter*arenaLength/imageWidth,cueArray[i]->intensity,cueArray[i]->getRobots());
	       	fprintf(stdout,"Cue %01i %08.3f %.3f %.3f %.3f %.3f %i\n",i,client->getTime(0),cueArray[i]->x*arenaLength,cueArray[i]->y*arenaWidth,cueArray[i]->diameter*arenaLength/imageWidth,cueArray[i]->intensity,cueArray[i]->getRobots());
	}
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
			fprintf(robotPositionLog,"Robot %01i %08.3f %06.3f %06.3f %06.3f %06.0f %i\n",i,client->getTime(i),client->getX(i),client->getY(i),client->getPhi(i),pherofield[0]->get(indexX,indexY),robotCue[i]);
			fprintf(stdout,"Robot %01i %08.3f %06.3f %06.3f %06.3f %06.0f %i\n",i,client->getTime(i),client->getX(i),client->getY(i),client->getPhi(i),pherofield[0]->get(indexX,indexY),robotCue[i]);
			//for (int j = 0;j<numBots;j++)fprintf(robotPositionLog,"%.3f ",sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
			//fprintf(robotPositionLog,"\n");
		}
	}
}

int main(int argc,char* argv[])
{
	//register ctrl+c handler
	signal (SIGINT,ctrl_c_handler);
	//initialize the logging system

	//auto-detect screen resolution (in case of a single display) and initialize the GUI
	gui = new CGui(&imageWidth,&imageHeight,dualMonitor);
	image = new CRawImage(imageWidth,imageHeight);

	//read number of robots and pheromone half-life from the command line
	experimentTime = atoi(argv[1]);	
	evaporation = atof(argv[2]);
	numBots = atoi(argv[3]);
	pheromoneMultiplier = atof(argv[4]);
	pheromoneIntensity = atof(argv[5]);
	cueMultiplier = atof(argv[6]);

	cueSizeArray[0] 	= atof(argv[7])*imageWidth/arenaLength;
	cueIntensityArray[0] 	= atof(argv[8]);
	cueSizeArray[1] 	= atof(argv[9])*imageWidth/arenaLength;;
	cueIntensityArray[1] 	= atof(argv[10]);

	if (argc > 11){
		cueX[0] = atof(argv[11]);
		cueY[0] = atof(argv[12]);
		cueX[1] = atof(argv[13]);
		cueY[1] = atof(argv[14]);
		randomCues = false;
		for (int i = 0;i<numCues;i++) cueArray[i] = new CCue(cueX[i],cueY[i],cueIntensityArray[i],cueSizeArray[i]);
	} 
	if (initializeLogging()==false) return -1;
	float diffusion = 0.0;
	float influence = 1.0;

	/*initialize the pheromone fields
	* pheromone field 0 simulates a longer-decay pheromone that the other robots follow
	* pheromone field 1 is released by the leader if it gets too close to arena boundaries causing the leader to avoid them - this pheromone decays quickly
	* pheromone field 2 is released by the leader to supress pheromone field 0 (this avoids the leader to detect pheromone 0 by its sensors)
	*evaporation defines pheromone's half-life, diffusion its spreading over time and strength determines how the pheromone influences the LCD-displayed image
	for details, see the chapter 2 of paper Arvin, Krajnik, Turgut, Yue: "CosPhi: Artificial Pheromone System for Robotic Swarms Research", IROS 2015*/
	pherofield[0] = new CPheroField(imageWidth,imageHeight,evaporation,diffusion,influence);
	pherofield[1] = new CPheroField(imageWidth,imageHeight,0.1,0,-5);

	/*connect to the localization system*/
	client = new CPositionClient();
	client->init(whyconIP,"6666");
	image->getSaveNumber();
	randomPlacement();

	globalTimer.pause();
	CTimer performanceTimer;
	performanceTimer.start();
	while (stop == false){
		//get the latest data from localization system and check if the calibration finished
		stop = (globalTimer.getTime()/1000000>experimentTime);

		/*PHEROMONE DECAY*/ 
		pherofield[0]->recompute();	//main pheromone half-life (user-settable, usually long)
		pherofield[1]->recompute();	//main pheromone half-life (user-settable, usually long)

		client->checkForData();

		/*PHEROMONE INJECTION*/
		if (calibration==false && placement==false)
		{

			/*PHEROMONE 1 - released by the leading robot*/
			for (int i = 0;i<numBots;i++)
			{
				int robotID = client->getID(i);
				float pheroStr = getPheroStrength(robotID);
				if (pheroStr > 0){
					pherofield[0]->addTo(client->getX(i)*imageWidth/arenaLength,client->getY(i)*imageHeight/arenaWidth,i,pheroStr,pheroRadius,robotID);
				}

				/*cause the leading robot to release pheromone 1 that is used for obstacle avoidance and 2 that temporarily suppresses pheromone 0*/
				float dist = 0.030;	//distance of the pheromone release relatively to the leader (controls pheromones 1 and 2 only) 
				float addPhi = 0;	//angle of the pheromone release relatively to the leader (controls pheromones 1 and 2 only) 
				float phi = client->getPhi(i);
				pherofield[1]->addTo((client->getX(i)+dist*cos(phi+addPhi))*imageWidth/arenaLength,(client->getY(i)+dist*sin(phi+addPhi))*imageHeight/arenaWidth,0,pheroStrength,45,robotID);
			}

			logRobotPositions();
		}
		//convert the pheromone field to grayscale image
		image->combinePheromones(pherofield,2,0,pheromoneIntensity);		//the last value determines the color channel - 0 is for grayscale, 1 is red etc.

		//CUES HANDLING
		for (int j = 0;j<numCues;j++)
		{	
			cueArray[j]->recompute();
			image->addCues(cueArray,numCues);
			cueArray[j]->resetRobots();
		}

		//is a given robot inside of a cue
		for (int i = 0;i<numBots;i++)
		{
			bool onCue = false;
			robotCue[i] = -1;
			for (int j = 0;j<numCues;j++)
			{
				float dx = client->getX(i)*imageWidth/arenaLength-cueArray[j]->x*imageWidth;
				float dy = client->getY(i)*imageHeight/arenaWidth-cueArray[j]->y*imageHeight;
				float dist = sqrt(dx*dx+dy*dy);

				if (dist < cueArray[j]->diameter/2)
				{
					if (client->getSpeed(i)<0.003) addPheroCount(client->getID(i),cueArray[j]->intensity);
					cueArray[j]->addRobot();
					robotCue[i] = j;
					onCue = true;
				}
			}
			if (onCue == false) reducePheroCount(client->getID(i));
		}
		//printf("PHERO: ");for (int i = 0;i<MAX_ROBOTS;i++) printf("%.3f ",pheroCount[i]);printf("\n");

		//experiment preparation phase 2: draw initial and real robot positions
		initRadius = robotDiameter/arenaLength*imageWidth/2;	//calculate robot radius in pixels, so that it matches the real robot dimensions
		for (int i = 0;i<numBots&&placement;i++) image->displayInitialPositions(initX[i],initY[i],initA[i],initBrightness,initRadius+10);
		for (int i = 0;i<numBots && placement && calibration == false;i++){
		       	if (client->exists(i))  image->displayRobot(client->getX(i)*imageWidth/arenaLength,client->getY(i)*imageHeight/arenaWidth,client->getPhi(i),0,initRadius+10);
		}

		/*this chunk of code is used to determine lag*/
		/*float t = globalTimer.getTime()/1000000.0;	
		  for (int i = 0;i<numBots;i++){
		  if (client->exists(i)){
		  gui->displayRobot(client->getX(i)*imageWidth/arenaLength,client->getY(i)*imageHeight/arenaWidth,client->getPhi(i),0,initRadius+10);
		  float dx = fabs(100+50*t-client->getX(i)*imageWidth/arenaLength);
		  float dy = fabs(imageHeight/2-client->getY(i)*imageHeight/arenaWidth);
		  printf("Distance: %.3f Lag: %f \n",sqrt(dx*dx+dy*dy),sqrt(dx*dx+dy*dy)/50);
		  }
		  }
		  gui->displayPattern(100+t*50,imageHeight/2,initRadius);*/


		//experiment preparation phase 1: draw calibration, contact WhyCon to calibrate and draw initial robot positions
		if (calibration){
			int calibRadius = initRadius/(cameraHeight-robotHeight)*cameraHeight;		//slightly enlarge to compensate for the higher distance from the camera
			image->displayCalibrationPatterns(calibRadius,calibOffset/arenaLength*imageWidth);
			client->calibrate(numBots,arenaLength,arenaWidth,cameraHeight,robotDiameter,robotHeight,calibOffset);
			client->checkForData();
		}
		calibration = client->calibrated==false;
		gui->drawImage(image);
		if (calibration) gui->displayCalibrationInfo(cameraHeight,client->numSearched,client->numDetected,performanceTimer.getTime()/1000);
	       	if (placement && calibration == false)	gui->displayPlacementInfo(client->numSearched,client->numDetected);
		gui->update();
		processEvents();
		//printf("GUI refresh: %i ms, updates %i frame delay %.0f ms %i %i\n",performanceTimer.getTime()/1000,client->updates,(performanceTimer.getRealTime()-client->frameTime)/1000.0,calibration);
		performanceTimer.reset();
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
