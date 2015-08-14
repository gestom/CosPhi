#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include "CCamera.h"
#include "CGui.h"
#include "CTimer.h"
#include "CCircleDetect.h"
#include "CPosition.h"
#include "CTransformation.h"
#include <SDL/SDL.h>
#include "CMessageClient.h"
#include "CPositionServer.h"

#define CALIB_STEPS 20 
#define AUTO_CALIB_STEPS 30 
#define AUTO_CALIB_INITS 10 

//-----These parameters need to be adjusted by the user -----------------------
//Adjust max GUI dimensions here 
int  screenWidth= 1920;
int  screenHeight = 1080;

//Adjust camera resolution here
int  imageWidth= 1920;
int  imageHeight = 1080;

//Adjust the black circle diameter [m] 
float circleDiameter = 0.03;

//Adjust the X and Y dimensions of the coordinate system 
float circlePositionX = 1.00;
float circlePositionY = 1.00;

//save raw video for postprocessing
bool saveVideo = true;
//----------------------------------------------------------------------------




int numFound = 0;
int numStatic = 0;
ETransformType transformType = TRANSFORM_2D;

bool autotest = false;
bool track = true;
int runs = 0;
int trackOK = 0;
int numBots = 0;
int guiScale = 1;
int wasBots = 1;
int evalTime = 0;
int calibNum = 5;
STrackedObject calib[5];
STrackedObject calibTmp[CALIB_STEPS];
int calibStep = CALIB_STEPS+2;
TLogModule module = LOG_MODULE_MAIN;
bool useGui = false;
bool displayTime = false;
int numSaved = 0;
bool stop = false;
bool displayHelp = false;
bool drawCoords = true;
bool drawOriginal = false;

CCamera* camera;
CGui* gui;
CRawImage *image;
SDL_Event event;
CPositionServer* server;

CCircleDetect *detectorArray[MAX_PATTERNS];
STrackedObject objectArray[MAX_PATTERNS];
SSegment currentSegmentArray[MAX_PATTERNS];
SSegment lastSegmentArray[MAX_PATTERNS];

CTransformation *trans;

int moveVal = 1;
int moveOne = moveVal;
int keyNumber = 10000;
Uint8 lastKeys[1000];
Uint8 *keys = NULL;

void autocalibrate()
{
	int index[4];
	int value[4];
	int signX[] = {-1,+1,-1,+1};
	int signY[] = {+1,+1,-1,-1};
	for (int j = 0;j<4;j++){
		 value[j] = -100000000; 
		 index[j] = 0; 
	}
	for (int i = 0;i<numBots;i++) {
		for (int j = 0;j<4;j++) {
			if ((signX[j]*currentSegmentArray[i].x+signY[j]*currentSegmentArray[i].y)>value[j])
			{
				value[j]=signX[j]*currentSegmentArray[i].x+signY[j]*currentSegmentArray[i].y;
				index[j] = i;
			}
		}
	}
	for (int j = 0;j<4;j++){
		SSegment tmp;
		tmp = currentSegmentArray[j];
		currentSegmentArray[j]= currentSegmentArray[index[j]];
		currentSegmentArray[index[j]] = tmp;
		trans->transformType = TRANSFORM_NONE;
		calib[j] = trans->transform(currentSegmentArray[j],false);
		//calib[j] = objectArray[index[j]]; 
		//printf("Calibrator %i %.5f %.5f \n",index[j],objectArray[index[j]].x,objectArray[index[j]].y);
	}
	trans->calibrate2D(calib,circlePositionX,circlePositionY);
}

void processKeys()
{
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_MOUSEBUTTONDOWN){
			if (calibNum < 4 && calibStep > CALIB_STEPS){
				 calibStep = 0;
				 trans->transformType = TRANSFORM_NONE;
			}
			lastSegmentArray[numBots-1].x = event.motion.x*guiScale; 
			lastSegmentArray[numBots-1].y = event.motion.y*guiScale;
			//currentSegmentArray[numBots-1].x = event.motion.x*guiScale; 
			//currentSegmentArray[numBots-1].y = event.motion.y*guiScale;
			lastSegmentArray[numBots-1].valid = true;
			detectorArray[numBots-1]->localSearch = true;
		}
	}
	keys = SDL_GetKeyState(&keyNumber);
	if (keys[SDLK_ESCAPE]) stop = true;
	if (keys[SDLK_SPACE] && lastKeys[SDLK_SPACE] == false){ moveOne = 10000000; moveVal = 1000000;};
	if (keys[SDLK_p] && lastKeys[SDLK_p] == false) {moveOne = 1; moveVal = 0;}
	if (keys[SDLK_m] && lastKeys[SDLK_m] == false) printf("SAVE %03f %03f %03f %03f %03f %03f %03f\n",objectArray[0].x,objectArray[0].y,objectArray[0].z,objectArray[0].error,objectArray[0].d,currentSegmentArray[0].m0,currentSegmentArray[0].m1);
	if (keys[SDLK_n] && lastKeys[SDLK_n] == false) printf("SEGM %03f %03f %03f\n",currentSegmentArray[0].x,currentSegmentArray[0].y,currentSegmentArray[0].m0);
	if (keys[SDLK_s] && lastKeys[SDLK_s] == false) image->saveBmp();
	if (keys[SDLK_o] && lastKeys[SDLK_o] == false) drawOriginal = drawOriginal == false; 
	if (keys[SDLK_d] && lastKeys[SDLK_d] == false) for (int i = 0;i<MAX_PATTERNS;i++){
			detectorArray[i]->draw = detectorArray[i]->draw==false;
			detectorArray[i]->debug = 10-detectorArray[i]->debug;
			
	}
	if (keys[SDLK_a] && lastKeys[SDLK_a] == false) autocalibrate();
	if (keys[SDLK_l] && lastKeys[SDLK_l] == false) drawCoords = drawCoords == false;
	if (keys[SDLK_r] && lastKeys[SDLK_r] == false) { calibNum = 0; wasBots=numBots; numBots = 1;}; 
	if (keys[SDLK_v] && lastKeys[SDLK_v] == false) for (int i = 0;i<numBots;i++) detectorArray[i]->drawAll = detectorArray[i]->drawAll==false;;
	if (keys[SDLK_1] && lastKeys[SDLK_1] == false) trans->transformType = TRANSFORM_NONE;
	if (keys[SDLK_2] && lastKeys[SDLK_2] == false) trans->transformType = TRANSFORM_2D;
	if (keys[SDLK_3] && lastKeys[SDLK_3] == false) trans->transformType = TRANSFORM_3D;

	if (keys[SDLK_c] && keys[SDLK_RSHIFT] == false) camera->changeContrast(-1);
	if (keys[SDLK_c] && keys[SDLK_RSHIFT] == true) camera->changeContrast(1);
	if (keys[SDLK_g] && keys[SDLK_RSHIFT] == false) camera->changeGain(-1);
	if (keys[SDLK_g] && keys[SDLK_RSHIFT] == true) camera->changeGain(1);
	if (keys[SDLK_e] && keys[SDLK_RSHIFT] == false) camera->changeExposition(-1);
	if (keys[SDLK_e] && keys[SDLK_RSHIFT] == true) camera->changeExposition(1);
	if (keys[SDLK_b] && keys[SDLK_RSHIFT] == false) camera->changeBrightness(-1);
	if (keys[SDLK_b] && keys[SDLK_RSHIFT] == true) camera->changeBrightness(1);

	if (keys[SDLK_h] && lastKeys[SDLK_h] == false){
	//	 useGui = useGui == false;
		 displayHelp = displayHelp == false; 
	}

	if (keys[SDLK_PLUS]) numBots++;
	if (keys[SDLK_EQUALS]) numBots++;
	if (keys[SDLK_MINUS]) numBots--;
	if (keys[SDLK_KP_PLUS]) numBots++;
	if (keys[SDLK_KP_MINUS]) numBots--;

	memcpy(lastKeys,keys,keyNumber);
}

int main(int argc,char* argv[])
{
	dump = new CDump(NULL,256,1000000);
	if (argc < 2) {
		fprintf(stderr,"usage: %s cameraDevice\ne.g. %s /dev/video0 num_tracked\n",argv[0],argv[0]);
		return 0;
	}
	const char* cameraDevice = argv[1];
	camera = new CCamera();
	server = new CPositionServer();
	server->init("6666"); 
	moveOne = moveVal;
	moveOne  = 0;
	if (argc > 2) numBots = atoi(argv[2]);
	if (argc > 2) useGui=true;
	if (argc > 3 && strcmp(argv[3],"nogui")==0) useGui=false;
	camera->init(cameraDevice,&imageWidth,&imageHeight,saveVideo);
	while (imageHeight/guiScale > screenHeight || imageHeight/guiScale > screenWidth) guiScale = guiScale*2;
	if (useGui) gui = new CGui(imageWidth,imageHeight,guiScale);
	image = new CRawImage(imageWidth,imageHeight);
	trans = new CTransformation(imageWidth,imageHeight,circleDiameter,true);
	for (int i = 0;i<MAX_PATTERNS;i++) detectorArray[i] = new CCircleDetect(imageWidth,imageHeight,i);
	image->getSaveNumber();

	CTimer timer;
	timer.reset();
	timer.start();

	CTimer globalTimer;
	globalTimer.reset();
	globalTimer.start();

	int frameID =0;
	while (stop == false){
		//if (camera->renewImage(image,moveOne-->0)==-1)stop = true;
		camera->renewImage(image,moveOne-->0);
		if (drawOriginal) gui->drawImage(image);
		numFound = numStatic = 0;
		timer.reset();

		//track the robots 
		for (int i = 0;i<numBots;i++){
			detectorArray[i]->calibMode = false;
			if (calibNum >= 4){
				lastSegmentArray[i] = currentSegmentArray[i];
				detectorArray[i]->calibMode = false;
			}
			if (lastSegmentArray[i].valid) currentSegmentArray[i] = detectorArray[i]->findSegment(image,lastSegmentArray[i]); else currentSegmentArray[i].valid = false; 
		}

		//search for lost (i.e. untracked) robots 
		for (int i = 0;i<numBots;i++){
			if (currentSegmentArray[i].valid == false){
				lastSegmentArray[i].valid = false;
				currentSegmentArray[i] = detectorArray[i]->findSegment(image,lastSegmentArray[i]);
			}
			if (currentSegmentArray[i].valid == false) break;
		}
		autocalibrate();
		//perform transformations
		for (int i = 0;i<numBots;i++){
			if (currentSegmentArray[i].valid){
				objectArray[i] = trans->transform(currentSegmentArray[i],false);
				numFound++;
				if (currentSegmentArray[i].x == lastSegmentArray[i].x) numStatic++;
			}else{
				trackOK = 0;
			}
		}
		printf("Pattern detection time: %i us. Found: %i Static: %i.\n",globalTimer.getTime(),numFound,numStatic);
		evalTime = timer.getTime();
		server->setNumOfPatterns(numBots);
		for (int i = 0;i<numBots;i++) server->updatePosition(objectArray[i],i);
		if (useGui){
			if (drawOriginal == false) gui->drawImage(image);
			gui->drawTimeStats(evalTime,numBots);
			gui->displayHelp(displayHelp);
			gui->guideCalibration(calibNum,circlePositionX,circlePositionY);
		}
		for (int i = 0;i<numBots && useGui && drawCoords;i++){
			if (currentSegmentArray[i].valid) gui->drawStats(currentSegmentArray[i].minx-30,currentSegmentArray[i].maxy,objectArray[i],trans->transformType == TRANSFORM_2D);
		}

		//establishing the coordinate system
		if (calibNum < 4){
			if (currentSegmentArray[0].valid){
				STrackedObject o = objectArray[0];
				moveOne = moveVal;

				//object found - add to a buffer
				if (calibStep < CALIB_STEPS) calibTmp[calibStep++] = o;

				//is the buffer full enough to calculate the object position ?
				if (calibStep == CALIB_STEPS){
					o.x = o.y = o.z = 0;
					for (int k = 0;k<CALIB_STEPS;k++){
						o.x += calibTmp[k].x;
						o.y += calibTmp[k].y;
						o.z += calibTmp[k].z;
					}
					o.x = o.x/CALIB_STEPS;	
					o.y = o.y/CALIB_STEPS;	
					o.z = o.z/CALIB_STEPS;
					if (calibNum < 4){
						calib[calibNum++] = o;
					}

					//was it the last object needed to establish the transform ?
					if (calibNum == 4){
						//calculate and save transforms
						trans->calibrate2D(calib,circlePositionX,circlePositionY);
						trans->calibrate3D(calib,circlePositionX,circlePositionY);
						calibNum++;
						numBots = wasBots;
						trans->saveCalibration("default.cal");
						trans->transformType = transformType;
						detectorArray[0]->localSearch = false;
					}
					calibStep++;
				}
			}
		}
		//gui->saveScreen(runs);
		if (useGui) gui->update();
		if (useGui) processKeys();
		for (int i = 0;i<numBots;i++){
			//if (currentSegmentArray[i].valid) printf("Object %i %03f %03f %03f %03f %03f\n",i,objectArray[i].x,objectArray[i].y,objectArray[i].z,objectArray[i].error,objectArray[i].esterror);
		}

		if (camera->cameraType == CT_WEBCAM){
			//for real camera, continue with capturing of another frame even if not all robots have been found
			moveOne = moveVal;
		}else{
			//for postprocessing, try to find all robots before loading next frame
			//if (numStatic ==  numBots)
			if (numFound ==  numBots)
			{
				//gui->saveScreen(runs++);
				for (int i = 0;i<numBots;i++) printf("Frame %i Object %i %i %.5f %.5f %.5f \n",frameID,i,currentSegmentArray[i].ID,objectArray[i].x,objectArray[i].y,objectArray[i].yaw);
				moveOne = moveVal; 
				if (moveVal > 0) frameID++;
			}else{
				if (moveOne-- < -100) moveOne = moveVal;
			}
		}
		if (displayTime) printf("Printing results time: %i ms.\n",globalTimer.getTime());
	}
	runs--;
	delete image;
	if (useGui) delete gui;
	for (int i = 0;i<MAX_PATTERNS;i++) delete detectorArray[i];
	delete camera;
	delete trans;
	return 0;
}
