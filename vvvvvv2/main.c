#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include "SDL/SDL_ttf.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int false = 0;
const int true = 1;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *capp = NULL;
SDL_Surface *sky = NULL;
SDL_Surface *cap = NULL;
SDL_Surface *up = NULL;
SDL_Surface *down = NULL;
SDL_Surface *left = NULL;
SDL_Surface *right = NULL;
SDL_Surface *screen = NULL;

//The event structure
SDL_Event event;

SDL_Rect clipRightD [2];
SDL_Rect clipLeftD [2];
SDL_Rect clipRightU [2];
SDL_Rect clipLeftU [2];



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

void apply_surface (int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface (source, clip, destination, &offset);
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
    SDL_WM_SetCaption( "VVVVVV2", NULL );

    //If everything initialized fine
    return true;
}

int load_files()
{
    //Load the background image
    background = Load_Image( "level1.png" );
    sky = Load_Image( "sky.png" );
    capp = Load_Image( "cap!.png" );
    cap = Load_Image( "Cap2.png" );

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
    SDL_FreeSurface( cap );
    SDL_FreeSurface( capp );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

int main( int argc, char* args[] )
{
    static int Xor = 100;
    static int Yor = 100;
    static int Speed = 0;
    static int velosityH = 0;
    static int velosityW = 0;
    static int frame = 0;
    static int skyx = 0;
    static int skys = 2;
    int Gol1[31][23], j, i;
    int Xst;
    int Yst;
    int i1;
    int j1;

    FILE *lev1;
    FILE *lev2;
    if ((lev1 = fopen("level1.txt", "r")) == NULL){
         puts("Can not open lev1.txt!\n");
         return false;
    }

    if ((lev2 = fopen("level2.txt", "w")) == NULL){
         puts("Can not open lev1.txt!\n");
         return false;
    }



    for (i=0;i<=23;i++){
        for(j=0;j<=31;j++){
            fscanf(lev1, "%1d ", &Gol1[j][i]);
            //fprintf(lev2, "%1d ", &Gol1[i][j]);
        }
        //fputs("\n", lev2);
    }
    for (i=0;i<=23;i++){
        for(j=0;j<=31;j++){
            //fscanf(lev1, "%1d ", &Gol1[i][j]);
            fprintf(lev2, "%1d ", Gol1[j][i]);
        }
        fputs("\n", lev2);
    }

    //int fclose(FILE *lev1);

    fclose(lev2);

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

    //Clip range for the top left
    clipRightD[ 0 ].x = 0;
    clipRightD[ 0 ].y = 44;
    clipRightD[ 0 ].w = 24;
    clipRightD[ 0 ].h = 44;

    //Clip range for the top right
    clipRightD[ 1 ].x = 24;
    clipRightD[ 1 ].y = 44;
    clipRightD[ 1 ].w = 24;
    clipRightD[ 1 ].h = 44;

    //Clip range for the top left
    clipRightU[ 0 ].x = 0;
    clipRightU[ 0 ].y = 0;
    clipRightU[ 0 ].w = 24;
    clipRightU[ 0 ].h = 44;

    //Clip range for the top right
    clipRightU[ 1 ].x = 24;
    clipRightU[ 1 ].y = 0;
    clipRightU[ 1 ].w = 24;
    clipRightU[ 1 ].h = 44;

    //Clip range for the top left
    clipLeftD[ 0 ].x = 72;
    clipLeftD[ 0 ].y = 44;
    clipLeftD[ 0 ].w = 24;
    clipLeftD[ 0 ].h = 44;

    //Clip range for the top right
    clipLeftD[ 1 ].x = 48;
    clipLeftD[ 1 ].y = 44;
    clipLeftD[ 1 ].w = 24;
    clipLeftD[ 1 ].h = 44;

    //Clip range for the top left
    clipLeftU[ 0 ].x = 72;
    clipLeftU[ 0 ].y = 0;
    clipLeftU[ 0 ].w = 24;
    clipLeftU[ 0 ].h = 44;

    //Clip range for the top right
    clipLeftU[ 1 ].x = 48;
    clipLeftU[ 1 ].y = 0;
    clipLeftU[ 1 ].w = 24;
    clipLeftU[ 1 ].h = 44;

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
        skyx = skyx - 3*skys;
        if (skyx < -600)
        {
            skyx=0;
        }
        //Apply the background
        apply_surface( skyx, 0, sky, screen , NULL);
        apply_surface( 0, 0, background, screen , NULL);

        //Get the keystates
        Uint8 *keystates = SDL_GetKeyState( NULL );

        j=Xor/20;
        i=Yor/20;
        j1=Xor%20;
        i1=Yor%20;

        //If up is pressed
        if( keystates[ SDLK_w ] || keystates[ SDLK_UP ] )
        {
            //apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen ,NULL);
            if(Speed==0)
            {
                if(Gol1[j][i+1]==0
                &&(i1!=0||(Gol1[j][i]==0)))
                {
                Speed-=1;
                velosityH=1;
                if(Speed<-1)
                {
                Speed=-1;
                }
                }
            }
        }
        //If down is pressed
        if( keystates[ SDLK_s ] || keystates[ SDLK_DOWN ] )
        {
            //apply_surface( ( SCREEN_WIDTH - down->w ) / 2, ( SCREEN_HEIGHT / 2 - down->h ) / 2 + ( SCREEN_HEIGHT / 2 ), down, screen , NULL);
            if(Speed==0)
            {
                Speed=Speed+1;
                velosityH=0;
                if(Speed>1)
                {
                Speed=1;
                }
            }
        }
        //If left is pressed
        if( keystates[ SDLK_a ] || keystates[ SDLK_LEFT ] )
        {
            if(Gol1[j][i+1]==0
                &&(j1!=0||(Gol1[j-1][i+1]==0)))
                {
            //apply_surface( ( SCREEN_WIDTH / 2 - left->w ) / 2, ( SCREEN_HEIGHT - left->h ) / 2, left, screen ,NULL);
            Xor=Xor-4;
            if(Xor%50>=0&&Xor%50<=24)
            {
                frame=1;
            }else
            {
                frame=0;
            }
            velosityW=0;
            }
            if(Gol1[j][i]==0&&velosityH==1)
                Speed=-1;
            if(Gol1[j][i]==0&&velosityH==0)
                Speed=1;
        }

        //If right is pressed
        if( keystates[ SDLK_d ] || keystates[ SDLK_RIGHT ] )
        {
            if(Gol1[j][i+1]==0
                &&(j1!=0||(Gol1[j+1][i+1]==0)))
                    {
            //apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen ,NULL);
            Xor=Xor+4;
            if((Xor%51>=0&&Xor%51<=24))
            {
                frame=1;
            }else
            {
                frame=0;
            }
            velosityW=1;
            }
            if(Gol1[j][i]==0&&velosityH==1)
                Speed=-1;
            if(Gol1[j][i]==0&&velosityH==0)
                Speed=1;
        }
        if(Speed==1){
            if(Gol1[j][i+2]==1)
            Speed=0;
        }else if(Speed==-1) {
            if(Gol1[j][i]==1)
            Speed=0;
        }


        Yor=Yor+5*Speed;

        if( velosityH==0)
        {
            if( velosityW==1)
            {
            apply_surface (Xor, Yor, cap, screen, &clipRightD[frame]);
            } else
            {
            apply_surface (Xor, Yor, cap, screen, &clipLeftD[frame]);
            }
        } else if( velosityH==1)
        {
            if( velosityW==1)
            {
            apply_surface (Xor, Yor, cap, screen, &clipRightU[frame]);
            } else
            {
            apply_surface (Xor, Yor, cap, screen, &clipLeftU[frame]);
            }
        }
        frame=0;
        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }
    fclose(lev2);

    //Clean up
    clean_up();

    return 0;
}
