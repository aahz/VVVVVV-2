#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_net.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

SDL_Surface *background=NULL;
SDL_Surface *level1=NULL;
SDL_Surface *dot=NULL;
SDL_Surface *dot2=NULL;
SDL_Surface *screen=NULL;

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
    SDL_WM_SetCaption ("VvVvVv2 server", NULL);
    return 1;
}

int load_files ()
{
    level1 = Load_Image ("level1.png");
    if (level1 == NULL)
        return 0;
    background = Load_Image ("bg.png");
    dot = Load_Image ("data/img/dot.png");
    return 1;
}

void clean_up ()
{
    SDL_FreeSurface (background);
    SDL_FreeSurface (dot);
    SDL_FreeSurface (dot2);
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
    int skys = 0;
    Uint8 *keystates = SDL_GetKeyState(NULL);
    SDLNet_ResolveHost(&ip, NULL, 2000);
    hostD = SDLNet_TCP_Open(&ip);
    socketSet = SDLNet_AllocSocketSet(1);
    while (clientD==NULL)
    {
        clientD = SDLNet_TCP_Accept(hostD);
    }
    SDLNet_TCP_AddSocket(socketSet, clientD);
    if (clientD!=NULL)
        apply_surface (0,0,background,screen);
    while (quit == 0)
    {
        while (SDL_PollEvent (&event))
        {
            if (event.type == SDL_QUIT)
                quit = 1;
            if (host == 1)
            {
                /* Now we can communicate with the client using hostD socket
                * clientD will remain opened waiting other connections */
                /* Get the remote address */
                if( keystates[ SDLK_SPACE ] ){
                    apply_surface (100,100,background,screen);
                    skys=skys+100;
                    SDLNet_TCP_Send(clientD, &skys, sizeof(int));
                }
            }
        }
		SDL_Flip (screen);
    }
    clean_up();
    return 0;
}
