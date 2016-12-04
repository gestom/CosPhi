#include "CCue.h"


CCue::CCue(float xi,float yi,float intens,float diam, float diamRate,float intensRate)
{
	x = xi;
	y = yi;
	diameter = diam;
	intensity = intens;
	diameterRate = diamRate;
	intensityRate = intensRate;
}

CCue::~CCue()
{
}

void CCue::recompute()
{
	float timex = timer.getTime();
	diameter += diameterRate*timex/1000000.0;
	intensity += intensityRate*timex/1000000.0;
	timer.reset();
	timer.start();
}

