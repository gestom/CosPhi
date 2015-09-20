#include "CPheroField.h"

CPheroField::CPheroField(int wi,int he,float evapor,float diffuse,float influ)
{
	evaporation = evapor;
	diffusion = diffuse;
	influence = influ;
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

void CPheroField::clear()
{
	memset(data,0,size*sizeof(float));
}

void CPheroField::addTo(int x, int y,int id,int num,int radius)
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
			add(ix,iy,id,num,radius);
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
			add(ix,iy,id,num,radius);
			ix+=f;
		}

	}

	lastX[id] = x;
	lastY[id] = y;
}


float CPheroField::get(int x, int y)
{
	if (x > 0 && y >0 && x<width && y<height) return data[x+y*width];
	return -1;
}

void CPheroField::add(int x, int y,int id,int num,int radius)
{
	id = id%MAX_ID;
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

void CPheroField::recompute()
{
	float timex = timer.getTime();
	float decay = pow(2,-timex/1000000.0/evaporation);
	int diffV,diffH;
	for (int i = 0;i<size;i++) data[i]=data[i]*decay;
	if (diffusion > 0.0){
		float diffuse = pow(2,-timex/1000000.0/diffusion);
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

