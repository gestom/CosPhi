#include "CGui.h"

#define THICK_CROSS

CGui::CGui(int *wi,int *he)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_HWSURFACE|SDL_HWACCEL);
	const SDL_VideoInfo* info = SDL_GetVideoInfo();
	*he = height = info->current_h;
	*wi = width = info->current_w;
	if(TTF_Init() == -1)printf("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
	screen = NULL;
	screen = SDL_SetVideoMode(width,height,24,SDL_FULLSCREEN); 
	if (screen == NULL)fprintf(stderr,"Couldn't set SDL video mode: %s\n",SDL_GetError());
	SDL_WM_SetCaption("Robot revue vision system","Robot revue vision system");
	smallFont =  TTF_OpenFont("../etc/DejaVuSansCondensed.ttf",24);
	if(!smallFont)printf("Unable to open font: %s\n", TTF_GetError());
	TTF_SetFontStyle(smallFont, TTF_STYLE_NORMAL);
}

CGui::~CGui()
{
	TTF_CloseFont(smallFont);
	SDL_FreeSurface(screen);
}

void CGui::drawImage(CRawImage* image)
{
	SDL_Surface *imageSDL = SDL_CreateRGBSurfaceFrom(image->data,image->width,image->height,image->bpp*8,image->bpp*image->width,0x000000ff,0x0000ff00,0x00ff0000,0x00000000);
	if (imageSDL != NULL) SDL_BlitSurface(imageSDL, NULL, screen, NULL);
	SDL_FreeSurface(imageSDL);
}

void CGui::displayInitialPositions(int x, int y,float phi,int id,int radius)
{
	int iix,iiy;
	int pos = 0;
	Uint8 *bufp;
	for (int ix = -radius;ix<radius;ix++)
	{
		iix = ix +x;
		for (int iy = -radius;iy<radius;iy++)
		{
			iiy = iy +y;
			if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
			{
				bufp = (Uint8 *)screen->pixels + iiy*screen->pitch + iix*3;
				float dist = sqrt(ix*ix+iy*iy);
				float ang = atan2(iy,ix)-phi;
				if (ang > +M_PI) ang-=2*M_PI;
				if (ang < -M_PI) ang+=2*M_PI;
				if (dist < radius-10 || (fabs(ang) > 0.1 && dist < radius)) bufp[0]=bufp[1]=bufp[2]=id;
			}
		}
	}
}

void CGui::displayRobot(int x, int y,int phi,int id)
{
	int radius = 40;
	int pos = 0;
	int iix,iiy;
	Uint8 *bufp;
	for (float i  = 0;i<6.28;i+=0.01)
	{
		iix = x+radius*sin(i);
		iiy = y+radius*cos(i);

		if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
		{
			bufp = (Uint8 *)screen->pixels + iiy*screen->pitch + iix*3;
			bufp[0]=bufp[1]=bufp[2]=128;
		}
	}
}

void CGui::displayCalibrationInfo(float camHeight,int numBots,int numVisible)
{
	/*display calibration patterns in the screen corners*/
	Uint8 *bufp;
	int radius = 40;
	int pos = 0;
	int iix,iiy;
	int oX = 0;
	int oY = 0;
	int xx[] = {radius+oX,radius+oX,width-radius-oX,width-radius-oX};
	int yy[] = {radius+oY,height-radius-oY,radius+oY,height-radius-oY};
	for (int corner = 0;corner<4;corner++)
	{
		int x = xx[corner]; 
		int y = yy[corner]; 
		for (int ix = -radius;ix<radius+1;ix++)
		{
			iix = ix +x;
			for (int iy = -radius;iy<radius+1;iy++)
			{
				iiy = iy +y;
				if (iix >= 0 && iix < width && iiy >=0 && iiy < height)
				{
					bufp = (Uint8 *)screen->pixels + iiy*screen->pitch + iix*3;
					float dist = sqrt(ix*ix+iy*iy);
					if ((dist < radius && dist > radius*0.75) || dist < radius * 0.25) bufp[pos]=bufp[pos+1]=bufp[pos+2]=255;
				}
			}
		}
	}

	/*display calibration info*/
	char info[1000];
	SDL_Rect rect;				
	SDL_Surface *text;		
	int fontSize = 32;
	rect.x = width/4;
	rect.y = height/2;
	rect.w = width/2;
	rect.h = 24;
	SDL_Color ok_col = { 0, 255, 0, 0 };

	rect.x = width/4;
	rect.y = height/2-fontSize;
	sprintf(info,"Calibrating the localization system - assuming that camera is %.2f m above the screen.",camHeight);
	text = TTF_RenderUTF8_Blended(smallFont, info, ok_col);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);

	SDL_Color blue = { 0, 255, 0, 0 };
	rect.y += fontSize;
	sprintf(info,"Searching for %i robots and 4 calibration patterns, %i of %i visible.",numBots,numVisible,numBots+4);
	text = TTF_RenderUTF8_Blended(smallFont, info, blue);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);

	SDL_Color red = { 255, 0, 0, 0 };
	rect.y += 2*fontSize;
	sprintf(info,"Place your robots at the designated positions or press P to generate new ones.");
	text = TTF_RenderUTF8_Blended(smallFont, info, red);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);
}

void CGui::drawTimeStats(int evalTime,int numBots)
{
	char info[1000];
	SDL_Rect rect;				
	SDL_Surface *text;		

	rect.x = 0;
	rect.y = 0;
	rect.w = 0;
	rect.h = 0;
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_Color ok_col = { 255, 0, 0, 0 };

	if (evalTime > maxTime)maxTime = evalTime;
	averageTime +=  evalTime;
	numStats++;
	sprintf(info,"Tracking %i robots takes %.3f ms now, %.3f on average and %.3f ms max.",numBots,evalTime/1000.0,averageTime/1000.0/numStats,maxTime/1000.0);
	text = TTF_RenderUTF8_Blended(smallFont, info, ok_col);
	rect.x = 0;
	rect.y = 0;
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);
}

void CGui::displayHelp(bool displayHelp)
{
	const char *helpText[] ={
		"[h] - to display/hide help,"
	};

	SDL_Rect rect;				
	SDL_Surface *text;			
	int numStrings = 10;
	if (displayHelp == false) numStrings = 1;
	rect.x = 0;
	rect.y = height-14*numStrings;
	rect.w = 300; 
	rect.h = 14*numStrings;
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_Color ok_col = { 0, 255, 0, 0 };
	for (int i = 0;i<numStrings;i++){
		rect.y = height-(i+1)*14;
		text = TTF_RenderUTF8_Blended(smallFont, helpText[i], ok_col);
		SDL_BlitSurface(text, NULL, screen, &rect);
		SDL_FreeSurface(text);
	}
}

void CGui::saveScreen(int a)
{
	CRawImage image((unsigned char*)screen->pixels,width,height);
	//image.swapRGB();
	//image.swap();
	char name[1000];
	sprintf(name,"output/%09i.bmp",a);
	image.saveBmp(name);
}

void CGui::drawLine(float sx1,float sx2,float sy1,float sy2)
{
	float d,r;

	r= (sy1-sy2)/(sx1-sx2);
	if (fabs(r) < 1){
		if (sx1 > sx2){
			d = sx1;
			sx1 =sx2;
			sx2 = d;
			d = sy1;
			sy1 =sy2;
			sy2 = d;
		}
		for (float x=sx1;x<sx2;x++){
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + (int)((x-sx1)*r+sy1+0.5)*screen->pitch + (int)(x+0.5)*3;
			*(bufp+screen->format->Rshift/8) = 255;
			*(bufp+screen->format->Gshift/8) = 0;
			*(bufp+screen->format->Bshift/8) = 0;
		}
	}else{
		if (sy1 > sy2){
			d = sx1;
			sx1 =sx2;
			sx2 = d;
			d = sy1;
			sy1 =sy2;
			sy2 = d;
		}
		for (float y=sy1;y<sy2;y++){
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + (int)(y+0.5)*screen->pitch + (int)((y-sy1)/r+sx1+0.5)*3;
			*(bufp+screen->format->Rshift/8) = 255;
			*(bufp+screen->format->Gshift/8) = 0;
			*(bufp+screen->format->Bshift/8) = 0;
		}
	}
}

void CGui::drawCircle(float cx,float cy,float r)
{
	int x,y;
	for (float a = 0;a<6.28;a+=0.01){
		x =r*cos(a)+cx+0.5;	
		y =r*sin(a)+cy+0.5;

		if (x > 0 && y> 0 && x <screen->w && y < screen->h)
		{
//			printf("al %i %i\n",x,y);
			Uint8 *bufp;
			bufp = (Uint8 *)screen->pixels + y*screen->pitch + x*3;
			*(bufp+screen->format->Rshift/8) = 0;
			*(bufp+screen->format->Gshift/8) = 255;
			*(bufp+screen->format->Bshift/8) = 0;
		}
	}
}




void CGui::update()
{
  SDL_UpdateRect(screen,0,0,0,0);
}
