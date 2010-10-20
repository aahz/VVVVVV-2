#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include "SDL/SDL_ttf.h"
#include "clean_up.c"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int false = 0;
const int true = 1;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *dot = NULL;
SDL_Surface *gdot = NULL;
SDL_Surface *up = NULL;
SDL_Surface *down = NULL;
SDL_Surface *left = NULL;
SDL_Surface *right = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };



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

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, NULL, destination, &offset );
}

int init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Good Boy", NULL );

    //If everything initialized fine
    return true;
}

int load_files()
{
    //Load the background image
    background = Load_Image( "background.png" );
    dot = Load_Image( "Capitan Rstay.png" );
    gdot = Load_Image( "Capitan Rgo.png" );

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 72 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( up );
    SDL_FreeSurface( down );
    SDL_FreeSurface( left );
    SDL_FreeSurface( right );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    int j = 100;
    int k = 100;
    int t = 0;
    //Quit flag
    int quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Render the text
    up = TTF_RenderText_Solid( font, "Up", textColor );
    down = TTF_RenderText_Solid( font, "Down", textColor );
    left = TTF_RenderText_Solid( font, "Left", textColor );
    right = TTF_RenderText_Solid( font, "Right", textColor );

    //While the user hasn't quit
    while( quit == false )
    {
        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //If the user has Xed out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //Apply the background
        apply_surface( 0, 0, background, screen );

        //Get the keystates
        Uint8 *keystates = SDL_GetKeyState( NULL );

        //If up is pressed
        if( keystates[ SDLK_UP ] )
        {
            apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen );
            if(t==0)
            {
                t=t-1;
                if(t<-1)
                {
                t=-1;
                }
            }
        }
        //If down is pressed
        if( keystates[ SDLK_DOWN ] )
        {
            apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen );
            if(t==0)
            {
                t=t+1;
                if(t>1)
                {
                t=1;
                }
            }
        }
        //If left is pressed
        if( keystates[ SDLK_LEFT ] )
        {
            apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen );
            j=j-1;
            j=j-1;
        }

        //If right is pressed
        if( keystates[ SDLK_RIGHT ] )
        {
            apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen );
            j=j+1;
            j=j+1;
        }
        k=k+5*t;
        if ( j <= 0 )
        {
            j = 0;
        }
        if ( j >= SCREEN_WIDTH - dot->w)
        {
            j = SCREEN_WIDTH - dot->w;
        }
        if ( k <= 0 )
        {
            k = 0;
            t = 0;
        }
        if ( k >= SCREEN_HEIGHT - dot->h)
        {
            k = SCREEN_HEIGHT - dot->h;
            t = 0;
        }
        apply_surface( j, k, dot, screen );

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Clean up
    clean_up();

    return 0;
}
