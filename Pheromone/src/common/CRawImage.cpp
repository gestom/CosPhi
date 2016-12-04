#include "CRawImage.h"

static unsigned char sth[] = {66,77,54,16,14,0,0,0,0,0,54,0,0,0,40,0,0,0,128,2,0,0,224,1,0,0,1,0,24,0,0,0,0,0,0,16,14,0,18,11,0,0,18,11,0,0,0,0,0,0,0,0,0,0};

CRawImage::CRawImage(int wi,int he)
{
	width =  wi;
	height = he;
	bpp= 3;
	size = bpp*width*height;
	data = (unsigned char*)calloc(size,sizeof(unsigned char));
	memset(header,0,122);
	memcpy(header,sth,122);
	header[18] = width%256;
	header[19] = width/256;
	header[22] = height%256;
	header[23] = height/256;
	header[2] = (size+122)%256;
	header[3] = ((size+122)/256)%256;
	header[4] = ((size+122)/256/256)%256;
	header[5] = ((size+122)/256/256)/256;
	header[34] = (size)%256;
	header[35] = ((size)/256)%256;
	header[36] = ((size)/256/256)%256;
	header[37] = ((size)/256/256)/256;
	header[10] = 122;
	numSaved = 0;
	ownData = true;
}

CRawImage::CRawImage(unsigned char *datai,int wi,int he)
{
	ownData = false;
	width =  wi;
	height = he;
	bpp= 3;
	size = bpp*width*height;
	data = datai; 
	memset(header,0,122);
	memcpy(header,sth,122);
	header[18] = width%256;
	header[19] = width/256;
	header[22] = height%256;
	header[23] = height/256;
	header[2] = (size+54)%256;
	header[3] = ((size+54)/256)%256;
	header[4] = ((size+54)/256/256)%256;
	header[5] = ((size+54)/256/256)/256;
	header[34] = (size)%256;
	header[35] = ((size)/256)%256;
	header[36] = ((size)/256/256)%256;
	header[37] = ((size)/256/256)/256;
	header[10] = 122;
	numSaved = 0;
}

void CRawImage::combinePheromones(CPheroField *p[],int number,int color,float pheroMax)
{
	float v;
	int maxPher = (int)(255*pheroMax);
	if (color > 0){
		for (int i = 0;i<size;i+=3)
		{	
			v = 0;
			for (int j = 0;j<number;j++) v+=p[j]->data[i/3]*p[j]->influence;
			data[i+color%3]=fmax(fmin(v,maxPher),1);
			data[i+(color+1)%3]=0;
			data[i+(color+2)%3]=0;
		}
	}else{
		for (int i = 0;i<size;i+=3)
		{
			v = 0;
			for (int j = 0;j<number;j++) v+=p[j]->data[i/3]*p[j]->influence;
			data[i+1]=data[i+2]=data[i]=fmax(fmin(v,maxPher),1);
		}
	}
}

void CRawImage::addCues(CCue *c[],int number)
{
	for (int i = 0;i<number;i++) displayCue(c[i]->x*width,c[i]->y*height,c[i]->diameter/2.0,c[i]->intensity);
}

void CRawImage::displayCue(int x, int y, int radius,float intensity)
{
	unsigned char *bufp;
	int iix,iiy;
	if (intensity < 0) intensity = 0;
	if (intensity > 255) intensity = 255;
	int intens = intensity;
	for (int ix = -radius;ix<radius+1;ix++)
	{
		iix = ix +x;
		for (int iy = -radius;iy<radius+1;iy++)
		{
			iiy = iy +y;
			if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
			{
				bufp = data + (iiy*width + iix)*3;
				float dist = sqrt(ix*ix+iy*iy);
				intens = intensity*255;
				//if (dist > 0.8*radius) intens = intensity*(1.0-(dist/radius-0.8)/0.2);
				if (dist < radius) bufp[0]=bufp[1]=bufp[2]=intens;// else bufp[0]=bufp[1]=bufp[2]=0;
			}
		}
	}

}

void CRawImage::displayCircle(int x, int y, int radius,int intensity)
{
	unsigned char *bufp;
	int iix,iiy;
	if (intensity < 0) intensity = 0;
	if (intensity > 255) intensity = 255;
	for (int ix = -radius;ix<radius+1;ix++)
	{
		iix = ix +x;
		for (int iy = -radius;iy<radius+1;iy++)
		{
			iiy = iy +y;
			if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
			{
				bufp = data + (iiy*width + iix)*3;
				float dist = sqrt(ix*ix+iy*iy);
				if (dist < radius) bufp[0]=bufp[1]=bufp[2]=intensity;// else bufp[0]=bufp[1]=bufp[2]=0;
			}
		}
	}
}

void CRawImage::displayRobot(int x, int y,float phi,int id,int radius)
{
	int iix,iiy;
	unsigned char *bufp;
	for (int ix = -radius;ix<radius;ix++)
	{
		iix = ix +x;
		for (int iy = -radius;iy<radius;iy++)
		{
			iiy = iy +y;
			if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
			{
				bufp = data + (iiy*width + iix)*3;
				float dist = sqrt(ix*ix+iy*iy);
				float ang = atan2(iy,ix)-phi;
				if (ang > +M_PI) ang-=2*M_PI;
				if (ang < -M_PI) ang+=2*M_PI;
				if (dist < radius && fabs(ang) > 0.1 && dist > radius-10)
				{
					bufp[1]=bufp[2]=0;
					bufp[0]=255;
				}
			}
		}
	}

}


void CRawImage::displayInitialPositions(int x, int y,float phi,int id,int radius)
{
	int iix,iiy;
	unsigned char *bufp;
	for (int ix = -radius;ix<radius;ix++)
	{
		iix = ix +x;
		for (int iy = -radius;iy<radius;iy++)
		{
			iiy = iy +y;
			if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
			{
				bufp = data + (iiy*width + iix)*3;
				float dist = sqrt(ix*ix+iy*iy);
				float ang = atan2(iy,ix)-phi;
				if (ang > +M_PI) ang-=2*M_PI;
				if (ang < -M_PI) ang+=2*M_PI;
				if (dist < radius-10 || (fabs(ang) > 0.1 && dist < radius)) bufp[0]=bufp[1]=bufp[2]=id;
			}
		}
	}
}

void CRawImage::displayCalibrationPatterns(int radius,int offset)
{
	int oX = 0;
	int oY = 0;
	int xx[] = {radius+oX+offset,radius+oX+offset,width-radius-oX-offset,width-radius-oX-offset};
	int yy[] = {radius+oY+offset,height-radius-oY-offset,radius+oY+offset,height-radius-oY-offset};
	for (int corner = 0;corner<4;corner++)displayPattern(xx[corner],yy[corner],radius); 
}

void CRawImage::displayPattern(int x, int y, int radius)
{
	unsigned char *bufp;
	int iix,iiy;
//	int rat = screen->pitch/width; 
	for (int ix = -radius;ix<radius+1;ix++)
	{
		iix = ix +x;
		for (int iy = -radius;iy<radius+1;iy++)
		{
			iiy = iy +y;
			if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
			{
				bufp = data + (iiy*width + iix)*3;
				float dist = sqrt(ix*ix+iy*iy);
				if ((dist < radius && dist > radius*0.75) || dist < radius * 0.25) bufp[0]=bufp[1]=bufp[2]=255; else bufp[0]=bufp[1]=bufp[2]=0;
			}
		}
	}
}

int CRawImage::getSaveNumber()
{
	char name[100];
	FILE* file = NULL;
	do{
		sprintf(name,"%04i.bmp",numSaved++);
		file = fopen(name,"r");
	}
	while (file != NULL);
	numSaved--;
	return numSaved;
}

CRawImage::~CRawImage()
{
	if (ownData) free(data);
}

void CRawImage::swap()
{
  unsigned char* newData = (unsigned char*)calloc(size,sizeof(unsigned char));
  int span = width*bpp;
  for (int j = 0;j<height;j++){
	  memcpy(&newData[span*j],&data[span*(height-1-j)],span);
	  for (int i = 0;i<width;i++){
		  char a = newData[(width*j+i)*3]; 
		  newData[(width*j+i)*3] = newData[(width*j+i)*3+2];
		  newData[(width*j+i)*3+2] = a; 
	  }
  }
  memcpy(data,newData,size);
  free(newData);
}

void CRawImage::saveBmp(const char* inName)
{
	FILE* file = fopen(inName,"wb");
	swap();
	fwrite(header,54,1,file);
	fwrite(header,54,1,file);
	fwrite(header,14,1,file);
	fwrite(data,size,1,file);
	swap();
	printf("Saved size %ix%i - %i.\n",size,width,height);
	fclose(file);
}

void CRawImage::saveBmp()
{
	char name[100];
	sprintf(name,"images/%06i.bmp",numSaved++);
	saveBmp(name);
}

bool CRawImage::loadBmp(const char* inName)
{
	FILE* file = fopen(inName,"rb");
	if (file!=NULL)
	{
		fread(data,54,1,file);
		bpp = 3;
		memcpy(header,data,54);
		int headerWidth = header[18]+header[19]*256;
		int headerHeight = header[22]+header[23]*256;
		if (ownData && (headerWidth != width || headerHeight != height)){
			free(data);
			height = headerHeight;
			width = headerWidth;
			size = height*width*bpp;
			data = (unsigned char*)calloc(size,sizeof(unsigned char));
		}
		int offset = header[10]+header[11]*256;
		fread(data,offset-54,1,file);
		fread(data,size,1,file);
		fclose(file);
		swap();
		return true;
	}
	return false;
}
