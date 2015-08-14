#include "CPheroField.h"

CPheroField::CPheroField(int wi,int he)
{

	lastX = (int*)calloc(MAX_ID,sizeof(float));
	lastY = (int*)calloc(MAX_ID,sizeof(float));
	width =  wi;
	height = he;
	size = width*height;
	data = (float*)calloc(size,sizeof(float));
}

CPheroField::~CPheroField()
{
	free(lastX);
	free(lastY);
	free(data);
}

void CPheroField::addTo(int x, int y,int id,int num)
{
	id = id%MAX_ID;
	int dx = x-lastX[id];
	int dy = y-lastY[id];
	if (fabs(dx) > 100 || fabs(dy) > 100) {
		lastX[id] = x;
		lastY[id] = y;
		dx = dy = 0; 
	}
	int sx,ex,sy,ey;
	float ix,iy;
	if (fabs(dx) > fabs(dy)){
		if (dx > 0){
			sx = lastX[id];
			ex = x;
			iy = lastY[id];
		}else{
			sx = x;
			ex = lastX[id];	
			iy = y;
		}
		float f = (float)dy/(float)dx;
		for (int ix = sx;ix<=ex;ix++)
		{
			add(ix,iy,id,num);
			iy+=f;
		}
	}else{
		if (dy > 0){
			sy = lastY[id];
			ey = y;
			ix = lastX[id];
		}else{
			sy = y;
			ey = lastY[id];
			ix = x;
		}
		float f = (float)dx/(float)dy;
		for (int iy = sy;iy<=ey;iy++)
		{
			add(ix,iy,id,num);
			ix+=f;
		}

	}

	lastX[id] = x;
	lastY[id] = y;
}


void CPheroField::add(int x, int y,int id,int num)
{
	id = id%MAX_ID;
	int radius = 25;
	int pos = 0;
	int iix,iiy;
	for (int ix = -radius;ix<radius+1;ix++){
		iix = ix +x;
		for (int iy = -radius;iy<radius+1;iy++){
			iiy = iy +y;
			if (iix >= 0 && iix < width && iiy >=0 && iiy < height){
				pos = (iix+iiy*width);
				data[pos] += num*(1-fmin(sqrt(ix*ix+iy*iy)/radius,1));
			}
		}
	}
	lastX[id] = x;
	lastY[id] = y;
}

void CPheroField::remove(int x, int y,float an,int num)
{
	int radius = 30;
	int pos = 0;
	int iix,iiy;
	for (float r=25;r<31;r+=0.01)
	{
		for (float a=an-M_PI/2;a<an+M_PI/2;a+=0.01)
		{
			iix = x + r*cos(a);
			iiy = y + r*sin(a);
			if (iix>= 0 && iix < width && iiy >=0 && iiy < height)
			{
				pos = (iix+iiy*width);
				data[pos] = 0;//data[pos]*0.999;
			}
		}
	}
}

void CPheroField::recompute(float decay,float diffuse)
{
	float timex = timer.getTime();
	decay = pow(2,-timex/1000000.0/decay);
	int diffV,diffH;
	for (int i = 0;i<size;i++) data[i]=data[i]*decay;
	if (diffuse > 0.0){
		diffuse = pow(2,-timex/1000000.0/diffuse);
		for (int i = width+1;i<size;i++){
			if (i%width == 0) i++;
			diffH = (data[i-1] - data[i])/2;
			data[i] += diffH*(1-diffuse);
			diffV = (data[i-width] - data[i])/2;
			data[i] += diffV*(1-diffuse);
			data[i-1] -= diffH*(1-diffuse);
			data[i-width] -= diffV*(1-diffuse);
		}
	}
	//printf("Recompute took %.0f %f\n",timer.getTime()-timex,diffuse);
	timer.reset();
	timer.start();
}

