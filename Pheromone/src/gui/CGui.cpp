#include "CGui.h"

#define THICK_CROSS

SDL_Texture *bitmapTex = NULL;
CGui::CGui(int *wi,int *he,bool dualMonitor)
{
	SDL_Init(SDL_INIT_VIDEO);
	if(TTF_Init() == -1)printf("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
	screen = NULL;
	sdlWindow = NULL;
	height = *he;
	width = *wi ;
	if (dualMonitor)
	{
		sdlWindow = SDL_CreateWindow( "Pheromone", SDL_WINDOWPOS_CENTERED_DISPLAY(1), SDL_WINDOWPOS_CENTERED_DISPLAY(1), width,height,SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_SHOWN );
	}else{
		sdlWindow = SDL_CreateWindow( "Pheromone", SDL_WINDOWPOS_CENTERED_DISPLAY(0), SDL_WINDOWPOS_CENTERED_DISPLAY(0), width,height, 0 );
	}
	//screen = SDL_GetWindowSurface(sdlWindow);
	renderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC); 
	SDL_GetWindowSize(sdlWindow,wi,he);
	height = *he;
	width = *wi;
	
	if (screen == NULL)fprintf(stderr,"Couldn't set SDL video mode: %s\n",SDL_GetError());
	//SDL_WM_SetCaption("CosPhi-Phero","Artificial Pheromone System");
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
	if (imageSDL != NULL) bitmapTex = SDL_CreateTextureFromSurface(renderer, imageSDL);
	SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
	//SDL_BlitSurface(imageSDL, NULL, screen, NULL);
	SDL_DestroyTexture(bitmapTex);
	SDL_FreeSurface(imageSDL);
}

void CGui::displayCalibrationInfo(float camHeight,int numBots,int numVisible,int refreshTime)
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
	SDL_Color ok_col = { 255, 255, 0, 0 };

	rect.x = width/4;
	rect.y = height/2-fontSize;
	sprintf(info,"Calibrating the localization system - assuming that camera is %.2f m above the screen. GUI refresh: %03i ms",camHeight,refreshTime);
	text = TTF_RenderUTF8_Blended(smallFont, info, ok_col);
	rect.x =  width/2 - text->w/2;rect.w = text->w;rect.h = text->h;	
	if (text != NULL) bitmapTex = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, bitmapTex,NULL,&rect);
	SDL_DestroyTexture(bitmapTex);
	SDL_FreeSurface(text);

	SDL_Color blue = { 255, 255, 0, 0 };
	rect.y += 2*fontSize;
	sprintf(info,"Searching for %02i patterns, %02i are visible.",numBots,numVisible);
	if (numVisible == -1){
		blue.r = 255;
		blue.g = 50;
		blue.b = 50;
		sprintf(info,"NO CONNECTION TO THE LOCALIZATION SYSTEM. Is SwarmCon running ?"); 
	}
	text = TTF_RenderUTF8_Blended(smallFont, info, blue);
	rect.x =  width/2 - text->w/2;rect.w = text->w;rect.h = text->h;	
	bitmapTex = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, bitmapTex,NULL,&rect);
	SDL_DestroyTexture(bitmapTex);
	SDL_FreeSurface(text);

	SDL_Color red = { 0, 255, 0, 0 };
	rect.y += 2*fontSize;
	sprintf(info,"Place your robots at the designated positions or press P to generate new ones.");
	text = TTF_RenderUTF8_Blended(smallFont, info, red);
	rect.x =  width/2 - text->w/2;rect.w = text->w;rect.h = text->h;	
	bitmapTex = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, bitmapTex,NULL,&rect);
	SDL_DestroyTexture(bitmapTex);
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
	rect.x =  width/2 - text->w/2;rect.w = text->w;rect.h = text->h;	
	bitmapTex = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, bitmapTex,NULL,&rect);
	SDL_DestroyTexture(bitmapTex);
	SDL_FreeSurface(text);

	SDL_Color blue = { 0, 255, 0, 0 };
	rect.y += fontSize;
	sprintf(info,"Detected %i out of %i robots.",numVisible,numBots);
	text = TTF_RenderUTF8_Blended(smallFont, info, blue);
	rect.x =  width/2 - text->w/2;rect.w = text->w;rect.h = text->h;	
	bitmapTex = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, bitmapTex,NULL,&rect);
	SDL_DestroyTexture(bitmapTex);
	SDL_FreeSurface(text);

	SDL_Color red = { 255, 0, 0, 0 };
	rect.y += 2*fontSize;
	sprintf(info,"Place your robots at the designated positions or press P to generate new ones.");
	text = TTF_RenderUTF8_Blended(smallFont, info, red);
	rect.x =  width/2 - text->w/2;rect.w = text->w;rect.h = text->h;	
	bitmapTex = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, bitmapTex,NULL,&rect);
	SDL_DestroyTexture(bitmapTex);
	SDL_FreeSurface(text);

	rect.y += fontSize;
	sprintf(info,"Press SPACE BAR to start the experiment.");
	text = TTF_RenderUTF8_Blended(smallFont, info, red);
	rect.x =  width/2 - text->w/2;rect.w = text->w;rect.h = text->h;	
	bitmapTex = SDL_CreateTextureFromSurface(renderer, text);
	SDL_RenderCopy(renderer, bitmapTex,NULL,&rect);
	SDL_DestroyTexture(bitmapTex);
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
	//SDL_Flip(screen);
	// SDL_(screen);
	//SDL_UpdateWindowSurface(sdlWindow);
	SDL_RenderPresent(renderer);
	//SDL_ShowWindow(sdlWindow);
}
