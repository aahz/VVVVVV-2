#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_net.h"
#include "SDL/SDL_ttf.h"

#define false 0
#define true 1

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *on=NULL;
SDL_Surface *off=NULL;
SDL_Surface *dot=NULL;
SDL_Surface *dot2=NULL;
SDL_Surface *screen=NULL;
SDL_Surface *message = NULL;
SDL_Surface *messageC = NULL;

TTF_Font *font = NULL;

SDL_Color textColor = { 255, 255, 255 };
SDL_Color redColor = { 201, 23, 23 };

SDLNet_SocketSet socketSet=NULL;
SDL_Event event;
TCPsocket clientD , hostD;
IPaddress ip, *remoteIP;

SDL_Surface *Load_Image (char *filename)
{
    SDL_Surface *Loaded_Image=NULL;
    SDL_Surface *Optimized_Image=NULL;
    Loaded_Image = IMG_Load (filename);
    if (Loaded_Image != NULL)
    {
        Optimized_Image = SDL_DisplayFormat (Loaded_Image);
        SDL_FreeSurface (Loaded_Image);
        if (Optimized_Image != NULL)
        {
            Uint32 colorkey = SDL_MapRGB (Optimized_Image->format, 0xFF, 0xFF, 0xFF);
            SDL_SetColorKey (Optimized_Image, SDL_SRCCOLORKEY, colorkey);
        }
    }
    return Optimized_Image;
}

void apply_surface (int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface (source, NULL, destination, &offset);
}

int init ()
{
    if (SDL_Init (SDL_INIT_EVERYTHING) == -1)
        return 0;
    screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if (screen == NULL)
        return 0;
    if (SDLNet_Init() == -1)
        return 0;
    if( TTF_Init() == -1 )
        return false;
    SDL_WM_SetCaption ("VvVvVv2 server", NULL);
    return 1;
}

int load_files ()
{
    off = Load_Image ("server-bg.png");
    if (off == NULL)
        return 0;
    font = TTF_OpenFont( "font.ttf", 50 );
    on = Load_Image ("server-green.png");
    dot = Load_Image ("data/img/dot.png");
    return 1;
}

void clean_up ()
{
    SDL_FreeSurface (on);
    SDL_FreeSurface (dot);
    SDL_FreeSurface (dot2);
    SDL_FreeSurface( message );
    SDL_FreeSurface( messageC );
    TTF_CloseFont( font );
    TTF_Quit();
    SDLNet_TCP_Close(clientD);
    SDLNet_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    init();
    load_files();
    int quit = 0;
    int host = 1;
    char DeathScore[3];
    int DeathCount=0;
    SDLNet_ResolveHost(&ip, NULL, 2000);
    hostD = SDLNet_TCP_Open(&ip);
    socketSet = SDLNet_AllocSocketSet(1);
    while (clientD==NULL)
    {
        clientD = SDLNet_TCP_Accept(hostD);
        apply_surface (0,0,off,screen);
        SDL_Flip (screen);
    }
    SDLNet_TCP_AddSocket(socketSet, clientD);
    while (quit == 0)
    {
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;
        }
        SDLNet_CheckSockets(socketSet, 0);
        if (SDLNet_SocketReady(clientD) != 0)
        {
            SDLNet_TCP_Recv(clientD, &DeathCount, sizeof(int));
        }
        snprintf (DeathScore, sizeof (DeathScore), "%d", DeathCount);
        messageC = TTF_RenderText_Solid (font, DeathScore, redColor);
        message = TTF_RenderText_Solid (font, "Your death score is:", textColor);
        apply_surface (0,0,off,screen);
        apply_surface (0,0,on,screen);
        apply_surface( SCREEN_WIDTH/2-messageC->w/2, SCREEN_HEIGHT/2-messageC->h/2+190, messageC, screen );
        apply_surface( SCREEN_WIDTH/2-message->w/2, SCREEN_HEIGHT/2-message->h/2+130, message, screen );
		SDL_Flip (screen);
    }
    clean_up();
    return 0;
}
