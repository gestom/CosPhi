#include "CGui.h"

#define THICK_CROSS

CGui::CGui(int wi,int he,int sc)
{
	height = he;
	width = wi;
	scale = sc;
	SDL_Init(SDL_INIT_VIDEO|SDL_HWSURFACE|SDL_HWACCEL);
	if(TTF_Init() == -1)printf("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
	screen = NULL;
	screen = SDL_SetVideoMode(wi/sc,he/sc,24,SDL_FULLSCREEN); 
	//screen = SDL_SetVideoMode(wi/sc,he/sc,24,SDL_SWSURFACE); 
	if (screen == NULL)fprintf(stderr,"Couldn't set SDL video mode: %s\r\n",SDL_GetError());
	SDL_WM_SetCaption("Robot revue vision system","Robot revue vision system");
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
	CRawImage *imageSrc = image;

	if (scale != 1){
		int wi = width/scale;
		int he = height/scale;
		imageSrc = new CRawImage(wi,he);
		for (int j = 0;j<he;j++){
			int srp = (j*scale)*wi*scale*3;
			int dep = j*wi*3;
			for (int i = 0;i<wi;i++){
				int dp = dep + i*3;
				int sp = srp + scale*i*3;
				imageSrc->data[dp] = image->data[sp];
				imageSrc->data[dp+1] = image->data[sp+1];
				imageSrc->data[dp+2] = image->data[sp+2];
			}
		}
	}
	SDL_Surface *imageSDL = SDL_CreateRGBSurfaceFrom(imageSrc->data,imageSrc->width,imageSrc->height,imageSrc->bpp*8,imageSrc->bpp*imageSrc->width,0x000000ff,0x0000ff00,0x00ff0000,0x00000000);
	if (imageSDL != NULL) SDL_BlitSurface(imageSDL, NULL, screen, NULL);
	SDL_FreeSurface(imageSDL);
	if (scale !=1) delete imageSrc;
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
	CRawImage image((unsigned char*)screen->pixels,width/scale,height/scale);
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
