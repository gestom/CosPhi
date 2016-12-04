#ifndef CCUE_H
#define CCUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <CTimer.h>
#include <math.h>

/**
@author Tom Krajnik
@about class that emulates artificial cue  
*/
class CCue
{
	public:

		CCue(float xi,float yi,float intensity,float diameter, float diameterRate = 0,float intesityRate = 0);
		~CCue();

		/*apply diminishing*/
		void recompute();

		float intensity;		//cue intensity 
		float diameter;			//cue diameter
		
		float diameterRate;		//affects growing/shrinking 
		float intensityRate;		//affects cue intensity 

		float x,y;

		CTimer timer;
};

#endif
