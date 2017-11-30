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

void CRawImage::combinePheromones(CPheroField *p[],int number,int color)
{
	float v;
	unsigned char* dat;
	int scale = p[0]->scale; 
	if (scale == 1){
		dat = data;
	} else{
		 dat = (unsigned char*)malloc(size/scale/scale);
	}
	if (color > 0){
		for (int i = 0;i<size/scale/scale;i+=3)
		{	
			v = 0;
			for (int j = 0;j<number;j++) v+=p[j]->data[i/3]*p[j]->influence;
			dat[i+color%3]=fmax(fmin(v,255),1);
			dat[i+(color+1)%3]=0;
			dat[i+(color+2)%3]=0;
		}
	}else{
		for (int i = 0;i<size/scale/scale;i+=3)
		{
			v = 0;
			for (int j = 0;j<number;j++) v+=p[j]->data[i/3]*p[j]->influence;
			dat[i+1]=dat[i+2]=dat[i]=fmax(fmin(v,255),1);
		}
		if (scale != 1)
		{
			int ix,iy,wi;
			wi = width/scale;
			float fx,fy;
			for (int x = 0;x<width-scale;x++){
				for (int y = 0;y<height-scale;y++){
					int pos0 = 3*(y*width+x);
					ix = x/scale;	
					iy = y/scale;
					fx = (float)x/scale-ix;
					fy = (float)y/scale-iy;
					int pos00 = 3*(iy*wi+ix);
					int pos10 = pos00+3; 
					int pos01 = pos00+wi*3;
					int pos11 = pos01+3;
					data[pos0+0] = dat[pos00]*(1-fx)*(1-fy)+dat[pos10]*fx*(1-fy)+dat[pos01]*(1-fx)*fy+dat[pos11]*fx*fy; 
					data[pos0+1] = dat[pos00+1]*(1-fx)*(1-fy)+dat[pos10+1]*fx*(1-fy)+dat[pos01+1]*(1-fx)*fy+dat[pos11+1]*fx*fy; 
					data[pos0+2] = dat[pos00+2]*(1-fx)*(1-fy)+dat[pos10+2]*fx*(1-fy)+dat[pos01+2]*(1-fx)*fy+dat[pos11+2]*fx*fy; 
				}
			}
		}
	}


	if (scale != 1){
		 free(dat);
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
