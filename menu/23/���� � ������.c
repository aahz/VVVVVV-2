#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include "SDL/SDL_ttf.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

SDL_Surface *buttonsheet=NULL;
SDL_Surface *screen=NULL;
SDL_Event event;
SDL_Rect clips [4];
SDL_Rect box;
SDL_Rect *clip;



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
            Uint32 colorkey = SDL_MapRGB (Optimized_Image->format, 0, 0xFF, 0xFF);
            SDL_SetColorKey (Optimized_Image, SDL_SRCCOLORKEY, colorkey);
        }
    }
    return Optimized_Image;
}

void apply_surface (int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface (source, clip, destination, &offset);
}

int init ()
{
    if (SDL_Init (SDL_INIT_EVERYTHING) == -1)
        return 0;
    screen = SDL_SetVideoMode (SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if (screen == NULL)
        return 0;
    SDL_WM_SetCaption ("Foo!", NULL);
    return 1;
}

int load_files ()
{
    buttonsheet = Load_Image ("button.png");
    if (buttonsheet == NULL)
        return 0;
    return 1;
}

void clean_up ()
{
    SDL_FreeSurface (buttonsheet);
    SDL_Quit();
}

void set_clips()
{
    clips[ CLIP_MOUSEOVER ].x = 0;
    clips[ CLIP_MOUSEOVER ].y = 0;
    clips[ CLIP_MOUSEOVER ].w = 320;
    clips[ CLIP_MOUSEOVER ].h = 240;

    clips[ CLIP_MOUSEOUT ].x = 320;
    clips[ CLIP_MOUSEOUT ].y = 0;
    clips[ CLIP_MOUSEOUT ].w = 320;
    clips[ CLIP_MOUSEOUT ].h = 240;

    clips[ CLIP_MOUSEDOWN ].x = 0;
    clips[ CLIP_MOUSEDOWN ].y = 240;
    clips[ CLIP_MOUSEDOWN ].w = 320;
    clips[ CLIP_MOUSEDOWN ].h = 240;

    clips[ CLIP_MOUSEUP ].x = 320;
    clips[ CLIP_MOUSEUP ].y = 240;
    clips[ CLIP_MOUSEUP ].w = 320;
    clips[ CLIP_MOUSEUP ].h = 240;
}

void Button( int x, int y, int w, int h )
{
    //Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;

    //Set the default sprite
    clip = &clips[ CLIP_MOUSEOUT ];
}

void handle_events ()
{
    int x=0, y=0;
    if (event.type == SDL_MOUSEMOTION)
    {
        x = event.motion.x;
        y = event.motion.y;
        if ( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            clip = &clips[ CLIP_MOUSEOVER ];
        }
        else
        {
            clip = &clips[ CLIP_MOUSEOUT ];
        }
    }
    if ( event.type == SDL_MOUSEBUTTONDOWN )
    {
        if (event.button.button == SDL_BUTTON_LEFT )
        {
            x = event.button.x;
            y = event.button.y;
            if ( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                clip = &clips[ CLIP_MOUSEDOWN ];
            }
        }
    }
    if ( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if ( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if ( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_MOUSEUP ];
            }
        }
    }
}

void show ()
{
    apply_surface (box.x, box.y, buttonsheet, screen, clip);
}

int main( int argc, char* args[] )
{
    int quit = 0;
    if (init()!=1)
        return 1;
    if (load_files()==0)
        return 0;
    set_clips();
    Button (170,120,320,240);
    while (quit == 0)
    {
        if (SDL_PollEvent (&event))
        {
            handle_events();
            if (event.type == SDL_QUIT)
                quit = 1;
        }
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
        show();
        if (SDL_Flip (screen) == -1)
            return 1;
    }
    clean_up();
    return 0;
}
