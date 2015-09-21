#include "CGui.h"

#define THICK_CROSS

CGui::CGui(int *wi,int *he,bool dualMonitor)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_HWSURFACE|SDL_HWACCEL);
	if(TTF_Init() == -1)printf("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
	screen = NULL;
	if (dualMonitor)
	{
		height = *he;
		width = *wi ;
		screen = SDL_SetVideoMode(width,height,24,SDL_HWSURFACE|SDL_NOFRAME); 
	}else{
		const SDL_VideoInfo* info = SDL_GetVideoInfo();
		*he = height = info->current_h;
		*wi = width = info->current_w;
		screen = SDL_SetVideoMode(width,height,24,SDL_FULLSCREEN); 
	}
	if (screen == NULL)fprintf(stderr,"Couldn't set SDL video mode: %s\n",SDL_GetError());
	SDL_WM_SetCaption("CosPhi-Phero","Artificial Pheromone System");
	smallFont =  TTF_OpenFont("../etc/DejaVuSansCondensed.ttf",14);
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

void CGui::displayRobot(int x, int y,float phi,int id,int radius)
{
	int iix,iiy;
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
				if (dist < radius && fabs(ang) > 0.1 && dist > radius-10)
				{
					bufp[1]=bufp[2]=0;
					bufp[0]=255;
				}
			}
		}
	}

}

void CGui::displayPattern(int x, int y, int radius)
{
	Uint8 *bufp;
	int iix,iiy;
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
				if ((dist < radius && dist > radius*0.75) || dist < radius * 0.25) bufp[0]=bufp[1]=bufp[2]=255; else bufp[0]=bufp[1]=bufp[2]=0;
			}
		}
	}
}

void CGui::displayCalibrationInfo(float camHeight,int numBots,int numVisible,int radius,int refreshTime)
{
	/*display calibration patterns in the screen corners*/
	int oX = 0;
	int oY = 0;
	int xx[] = {radius+oX,radius+oX,width-radius-oX,width-radius-oX};
	int yy[] = {radius+oY,height-radius-oY,radius+oY,height-radius-oY};
	for (int corner = 0;corner<4;corner++)displayPattern(xx[corner],yy[corner],radius); 

	/*display calibration info*/
	char info[1000];
	SDL_Rect rect;				
	SDL_Surface *text;		
	int fontSize = 32;
	rect.x = width/4;
	rect.y = height/2;
	rect.w = width/2;
	rect.h = 24;
	SDL_Color ok_col = { 0, 0, 255, 0 };

	rect.x = width/4;
	rect.y = height/2-fontSize;
	sprintf(info,"Calibrating the localization system - assuming that camera is %.2f m above the screen. GUI refresh: %i ms",camHeight,refreshTime);
	text = TTF_RenderUTF8_Blended(smallFont, info, ok_col);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);

	SDL_Color blue = { 0, 0, 255, 0 };
	rect.y += fontSize;
	sprintf(info,"Searching for %i patterns, %i are visible.",numBots,numVisible);
	if (numVisible == -1){
		blue.r = 255;
		blue.g = 50;
		blue.b = 50;
		sprintf(info,"NO CONNECTION TO THE LOCALIZATION SYSTEM. Is SwarmCon running ?"); 
	}
	text = TTF_RenderUTF8_Blended(smallFont, info, blue);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);

	SDL_Color red = { 0, 255, 0, 0 };
	rect.y += 2*fontSize;
	sprintf(info,"Place your robots at the designated positions or press P to generate new ones.");
	text = TTF_RenderUTF8_Blended(smallFont, info, red);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);
}

void CGui::displayPlacementInfo(int numBots,int numVisible)
{
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
	sprintf(info,"Calibration finished - blue circles now designate detected robot positions.");
	text = TTF_RenderUTF8_Blended(smallFont, info, ok_col);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);

	SDL_Color blue = { 0, 255, 0, 0 };
	rect.y += fontSize;
	sprintf(info,"Detected %i out of %i robots.",numVisible,numBots);
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

	rect.y += fontSize;
	sprintf(info,"Press SPACE BAR to start the experiment.");
	text = TTF_RenderUTF8_Blended(smallFont, info, red);
	rect.x =  width/2 - text->w/2; 
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);
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

void CGui::update()
{
//  SDL_UpdateRect(screen,0,0,0,0);
  SDL_Flip(screen);
}
