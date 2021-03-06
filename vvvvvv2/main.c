#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_net.h"
#include <stdio.h>
#include <stdlib.h>

#define false 0
#define true 1

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *mbackground = NULL;
SDL_Surface *ButNewGame = NULL;
SDL_Surface *ButContinue = NULL;
SDL_Surface *ButHowTo = NULL;
SDL_Surface *ButCredits = NULL;
SDL_Surface *ButExit = NULL;
SDL_Surface *ButNewGameA = NULL;
SDL_Surface *ButContinueA = NULL;
SDL_Surface *ButHowToA = NULL;
SDL_Surface *ButCreditsA = NULL;
SDL_Surface *ButExitA = NULL;
SDL_Surface *ButBack = NULL;
SDL_Surface *ButBackA = NULL;
SDL_Surface *message = NULL;
SDL_Surface *buttonsheet=NULL;
SDL_Surface *buttonSur=NULL;
SDL_Surface *addSur=NULL;
SDL_Surface *HowTo1 = NULL;
SDL_Surface *HowTo2 = NULL;
SDL_Surface *Credits = NULL;
SDL_Surface *Name1 = NULL;
SDL_Surface *Name2 = NULL;

SDL_Surface *level1 = NULL;
SDL_Surface *level2 = NULL;
SDL_Surface *level3 = NULL;
SDL_Surface *level4 = NULL;
SDL_Surface *level5 = NULL;
SDL_Surface *level6 = NULL;
SDL_Surface *level7 = NULL;
SDL_Surface *sky = NULL;
SDL_Surface *cap = NULL;
SDL_Surface *death = NULL;
SDL_Surface *up = NULL;
SDL_Surface *down = NULL;
SDL_Surface *left = NULL;
SDL_Surface *right = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *dieshow = NULL;
SDL_Surface *final = NULL;
SDL_Surface *finalText = NULL;
SDL_Surface *cartoon = NULL;

//The event structure
SDL_Event event;

SDL_Rect clipRightD [2];
SDL_Rect clipLeftD [2];
SDL_Rect clipRightU [2];
SDL_Rect clipLeftU [2];
SDL_Rect clips [4];
SDL_Rect clipdeath [4];
SDL_Rect clipcartoon [3];
SDL_Rect box;

Mix_Music *music=NULL;
Mix_Chunk *jump = NULL;
Mix_Chunk *death0 = NULL;

//The font
TTF_Font *font = NULL;
TTF_Font *fontSmall = NULL;

//The color of the font
SDL_Color textColor = { 255, 255, 255 };
SDL_Color currentColor1 = { 48, 70, 68 };
SDL_Color currentColor2 = { 55, 113, 93 };
SDL_Color currentColor3 = { 76, 179, 93 };
SDL_Color currentColor4 = { 127, 226, 106 };
SDL_Color currentColor5 = { 184, 234, 87 };
SDL_Color redColor = { 201, 23, 23 };

TCPsocket hostD;
IPaddress ip, *remoteIP;
SDLNet_SocketSet socketSet=NULL;



SDL_Surface *Load_Image( char* filename ){

    //����������� �����������
    SDL_Surface* loadedImage = NULL;

    //������������ ���������������� �����������
    SDL_Surface* optimizedImage = NULL;

    //��������� �����������
    loadedImage = IMG_Load( filename );

    //���������, ��������� �� �����������
    if( loadedImage != NULL ){
        //������� ���������������� �����������
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //����������� ������ �����������
        SDL_FreeSurface( loadedImage );

        //�������� ����������� �����������
        if( optimizedImage != NULL ){
            //��������
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //���������� ���������������� �����������
    return optimizedImage;
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

    if (SDLNet_Init() == -1)
        return false;

    //������������� �����
    buttonSur = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    addSur = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //�������� �� ������� ������ ��� ��������� ������
    if( buttonSur == NULL )
    {
        return 1;
    }

    if( addSur == NULL ){
        return 1;
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

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        return 0;
    if (Mix_Init(MIX_INIT_MP3) == -1)
        return 0;

    //If everything initialized fine
    return true;
}

int load_files()
{
    //��������� �������� �����������
    mbackground = Load_Image( "bg.png" );

    //��������� �������
    font = TTF_OpenFont( "font.ttf", 50 );
    fontSmall = TTF_OpenFont( "font.ttf", 38 );

    //������� ������ ��� �������� ����
    if( mbackground == NULL ){
        return 1;
    }

    music = Mix_LoadMUS ("Feel Good.wav");

    if( music == NULL )
        return 1;

    jump = Mix_LoadWAV( "jump.wav" );
    death0 = Mix_LoadWAV( "death1.wav" );

    //������� ������ ��� �������� �������
    if( font == NULL ){
        return 1;
    }

    if( fontSmall == NULL ){
        return 1;
    }
    //Load the background image
    level1 = Load_Image( "level1.png" );
    level2 = Load_Image( "level2.png" );
    level3 = Load_Image( "level3.png" );
    level4 = Load_Image( "level4.png" );
    level5 = Load_Image( "level5.png" );
    level6 = Load_Image( "level6.png" );
    level7 = Load_Image( "level7.png" );
    sky = Load_Image( "sky.png" );
    cap = Load_Image( "Cap2.png" );
    death = Load_Image( "IGLA.png" );
    cartoon = Load_Image( "comix.png" );

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( level1 );
    SDL_FreeSurface( level2 );
    SDL_FreeSurface( level3 );
    SDL_FreeSurface( level4 );
    SDL_FreeSurface( level5 );
    SDL_FreeSurface( level6 );
    SDL_FreeSurface( level7 );
    SDL_FreeSurface( up );
    SDL_FreeSurface( down );
    SDL_FreeSurface( left );
    SDL_FreeSurface( right );
    SDL_FreeSurface( cap );
    SDL_FreeSurface( death );
    SDL_FreeSurface( final );
    SDL_FreeSurface( finalText );
    SDL_FreeSurface( cartoon );

    Mix_FreeChunk(jump);
    Mix_FreeMusic (music);
    Mix_CloseAudio();

    SDLNet_TCP_Close(hostD);
    SDLNet_Quit();

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

int menu (){
    //������� ������
    int quit = 1;
    static int saveGame;

    //�������������
    if( init() != 1 ){
        return 0;
    }

    //��������� ������
    if( load_files() != 1 ){
        return 0;
    }
    Mix_PlayMusic (music, 0);

    //������� ������� ����
    int x = 0;

    //���������� ����������� � �������
    ButNewGame = TTF_RenderText_Solid( font, "New Game", textColor );
    ButContinue = TTF_RenderText_Solid( font, "Continue", textColor );
    ButHowTo = TTF_RenderText_Solid( font, "How to Play", textColor );
    ButCredits = TTF_RenderText_Solid( font, "Credits", textColor );
    ButExit = TTF_RenderText_Solid( font, "Exit", textColor );
    ButBack = TTF_RenderText_Solid( font, "Back to menu", textColor );
    ButNewGameA = TTF_RenderText_Solid( font, "New Game", currentColor1 );
    ButContinueA = TTF_RenderText_Solid( font, "Continue", currentColor2 );
    ButHowToA = TTF_RenderText_Solid( font, "How to Play", currentColor3 );
    ButCreditsA = TTF_RenderText_Solid( font, "Credits", currentColor4 );
    ButExitA = TTF_RenderText_Solid( font, "Exit", currentColor5 );
    ButBack = TTF_RenderText_Solid( font, "Back to menu", currentColor5 );

    HowTo1 = TTF_RenderText_Solid( fontSmall, "Use ARROW KEYS to move", redColor );
    HowTo2 = TTF_RenderText_Solid( fontSmall, "Use SHIFT button to change gravity", redColor );

    Credits = TTF_RenderText_Solid( fontSmall, "The VVV VVV 2 game was created by:", redColor );
    Name1 = TTF_RenderText_Solid( fontSmall, "Suvorkin Alexander", currentColor1 );
    Name2 = TTF_RenderText_Solid( fontSmall, "Ozdion Andrew", currentColor1 );

    //��������� ���
    apply_surface( 0, 0, mbackground, screen , NULL);
    apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
    apply_surface( 50, 90, ButContinue, buttonSur , NULL);
    apply_surface( 50, 150, ButHowTo, buttonSur , NULL);
    apply_surface( 50, 210, ButCredits, buttonSur , NULL);
    apply_surface( 50, 270, ButExit, buttonSur , NULL);

    //Grubing save
    FILE *save;

    if ((save = fopen("save.txt", "r")) == NULL){
        puts("Can not open save.txt!\n");
        return false;
    }

    fscanf(save, "%d", &saveGame);

    fclose(save);

    //�� ��� ���, ���� ������������ �� �����
    while( quit == 1 ){

        //������� ������� ��� ����������
        if( SDL_PollEvent( &event ) && event.type == SDL_KEYDOWN ){

            //��������� ���
            apply_surface( 0, 0, mbackground, screen , NULL);

            //�o������ ��������� ������
            Uint8 *keystates = SDL_GetKeyState( NULL );

            //������ ������� "�����" ��� "�����"
            if( keystates[ SDLK_UP ] || keystates[ SDLK_LEFT ] ){
                x--;
                if (x<1){
                    x=5;
                }
                //printf("%d\n", x);
            }
            //������ ������� "����" ��� "������"
            if( keystates[ SDLK_DOWN ] || keystates[ SDLK_RIGHT ] ){
                x++;
                if (x>5){
                    x=1;
                }
                //printf("%d\n", x);
            }
            if( keystates[ SDLK_ESCAPE ] ){
                quit = 0;
            }
            if( keystates[ SDLK_F1 ] ){
                x = 3;
            }


            //����
            apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
            apply_surface( 50, 90, ButContinue, buttonSur , NULL);
            apply_surface( 50, 150, ButHowTo, buttonSur , NULL);
            apply_surface( 50, 210, ButCredits, buttonSur , NULL);
            apply_surface( 50, 270, ButExit, buttonSur , NULL);

            //����� ����
            if(x==1){
                apply_surface( 50, 30, ButNewGameA, buttonSur , NULL);
                apply_surface( 50, 90, ButContinue, buttonSur , NULL);
                apply_surface( 50, 270, ButExit, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    return 1;
                }
            }

            //����������
            if(x==2){
                apply_surface( 50, 90, ButContinueA, buttonSur , NULL);
                apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
                apply_surface( 50, 150, ButHowTo, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    if (saveGame == 0)
                        saveGame = 1;
                    else if (saveGame == 9 || saveGame == 8)
                        saveGame = 1;
                    return saveGame;
                }
            }

            //����������
            if(x==3){
                apply_surface( 50, 150, ButHowToA, buttonSur , NULL);
                apply_surface( 50, 90, ButContinue, buttonSur , NULL);
                apply_surface( 50, 210, ButCredits, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    apply_surface( 50, 340, HowTo1, addSur , NULL);
                    //apply_surface( 50, 380, HowTo2, addSur );
                }
            }

            //���������
            if(x==4){
                apply_surface( 50, 210, ButCreditsA, buttonSur , NULL);
                apply_surface( 50, 150, ButHowTo, buttonSur , NULL);
                apply_surface( 50, 270, ButExit, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    apply_surface( 50, 340, Credits, addSur , NULL);
                    apply_surface( 150, 400, Name1, addSur , NULL);
                    apply_surface( 150, 440, Name2, addSur , NULL);
                }
            }

            //�����
            if(x==5){
                apply_surface( 50, 270, ButExitA, buttonSur , NULL);
                apply_surface( 50, 210, ButCredits, buttonSur , NULL);
                apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    quit = 0;
                }
            }

            //���� ������������ ������� �� �
            else if( event.type == SDL_QUIT ){
                //������� ������
                quit = 0;
            }
        }

        //���������� ������
        if( SDL_Flip( screen ) == -1 ){
            return 1;
        }
    }


    SDL_FreeSurface( mbackground );
    SDL_FreeSurface( ButNewGame );
    SDL_FreeSurface( ButContinue );
    SDL_FreeSurface( ButHowTo );
    SDL_FreeSurface( ButCredits );
    SDL_FreeSurface( ButExit );
    SDL_FreeSurface( ButNewGameA );
    SDL_FreeSurface( ButContinueA );
    SDL_FreeSurface( ButHowToA );
    SDL_FreeSurface( ButCreditsA );
    SDL_FreeSurface( ButExitA );
    SDL_FreeSurface( HowTo1 );
    SDL_FreeSurface( HowTo2 );
    SDL_FreeSurface( Credits );
    SDL_FreeSurface( Name1 );
    SDL_FreeSurface( Name2 );
    SDL_FreeSurface( ButBack );
    SDL_FreeSurface( ButBackA );
    SDL_FreeSurface( buttonsheet );
    SDL_FreeSurface( buttonSur );
    SDL_FreeSurface( addSur );

    //��������� �����
    TTF_CloseFont( font );
    TTF_CloseFont( fontSmall );

    return 0;
}

int main( int argc, char* args[] )
{
    int goOn = menu();
    if(goOn == 0)
        return false;
    else{
        Mix_HaltMusic();
        dieshow = TTF_RenderText_Solid( font, "DEAD", textColor );
        static int Xor, Xd;
        static int Yor, Yd;
        static int Speed = 0;
        static int velosityH = 0;
        static int velosityW = 0;
        static int frame = 0;
        static int skyx = 0;
        static int skys = 1;
        int Gol1[32][24], j, i;
        int z2=0,y2=0;
        int Dtime=150;
        int Dspeed;
        int i1;
        int j1;
        int l = goOn;
        int die=0;
        int DeathCount=-1;
        int FinalTime=0;
        int FinalSpeed=1;
        char DeathScore[10];
        int frameCartoon = 0;
        int SoundTime=0;
        int SoundSpeed=1;
        int nettest = -1;

        SDLNet_ResolveHost(&ip, "127.0.0.1", 2000);
        hostD = SDLNet_TCP_Open(&ip);

        socketSet = SDLNet_AllocSocketSet(1);
        nettest = SDLNet_TCP_AddSocket(socketSet, hostD);

        //Quit flag
        int quit = false;

        //Initialize

        //Load the files
        if( load_files() == false )
        {
            return 1;
        }
        //0-up 1-left 2-right 3-down
        clipdeath[ 0 ].x = 0;
        clipdeath[ 0 ].y = 0;
        clipdeath[ 0 ].w = 20;
        clipdeath[ 0 ].h = 20;

        clipdeath[ 1 ].x = 20;
        clipdeath[ 1 ].y = 0;
        clipdeath[ 1 ].w = 20;
        clipdeath[ 1 ].h = 20;

        clipdeath[ 2 ].x = 0;
        clipdeath[ 2 ].y = 20;
        clipdeath[ 2 ].w = 20;
        clipdeath[ 2 ].h = 20;

        clipdeath[ 3 ].x = 20;
        clipdeath[ 3 ].y = 20;
        clipdeath[ 3 ].w = 20;
        clipdeath[ 3 ].h = 20;

        //Clip range for the top left
        clipRightD[ 0 ].x = 0;
        clipRightD[ 0 ].y = 40;
        clipRightD[ 0 ].w = 20;
        clipRightD[ 0 ].h = 40;

        //Clip range for the top right
        clipRightD[ 1 ].x = 20;
        clipRightD[ 1 ].y = 40;
        clipRightD[ 1 ].w = 20;
        clipRightD[ 1 ].h = 40;

        //Clip range for the top left
        clipRightU[ 0 ].x = 0;
        clipRightU[ 0 ].y = 0;
        clipRightU[ 0 ].w = 20;
        clipRightU[ 0 ].h = 40;

        //Clip range for the top right
        clipRightU[ 1 ].x = 20;
        clipRightU[ 1 ].y = 0;
        clipRightU[ 1 ].w = 20;
        clipRightU[ 1 ].h = 40;

        //Clip range for the top left
        clipLeftD[ 0 ].x = 60;
        clipLeftD[ 0 ].y = 40;
        clipLeftD[ 0 ].w = 20;
        clipLeftD[ 0 ].h = 40;

        //Clip range for the top right
        clipLeftD[ 1 ].x = 40;
        clipLeftD[ 1 ].y = 40;
        clipLeftD[ 1 ].w = 20;
        clipLeftD[ 1 ].h = 40;

        //Clip range for the top left
        clipLeftU[ 0 ].x = 60;
        clipLeftU[ 0 ].y = 0;
        clipLeftU[ 0 ].w = 20;
        clipLeftU[ 0 ].h = 40;

        //Clip range for the top right
        clipLeftU[ 1 ].x = 40;
        clipLeftU[ 1 ].y = 0;
        clipLeftU[ 1 ].w = 20;
        clipLeftU[ 1 ].h = 40;

        //Final movie
        clipcartoon[ 0 ].x = 0;
        clipcartoon[ 0 ].y = 0;
        clipcartoon[ 0 ].w = 246;
        clipcartoon[ 0 ].h = 250;

        clipcartoon[ 1 ].x = 246;
        clipcartoon[ 1 ].y = 0;
        clipcartoon[ 1 ].w = 246;
        clipcartoon[ 1 ].h = 250;

        clipcartoon[ 2 ].x = 492;
        clipcartoon[ 2 ].y = 0;
        clipcartoon[ 2 ].w = 246;
        clipcartoon[ 2 ].h = 250;

        //Render the text
        up = TTF_RenderText_Solid( font, "Up", textColor );
        down = TTF_RenderText_Solid( font, "Down", textColor );
        left = TTF_RenderText_Solid( font, "Left", textColor );
        right = TTF_RenderText_Solid( font, "Right", textColor );

        //While the user hasn't quit
        while( quit == false )
        {
            do{
                FILE *lev1;
                FILE *lev1d;
                FILE *lev2;
                FILE *lev3;
                FILE *lev4;
                FILE *lev5;
                FILE *lev6;
                FILE *lev7;
                FILE *lev8;
                FILE *saveW;

                if ((saveW = fopen("save.txt", "w")) == NULL){
                    puts("Can not open save.txt!\n");
                return false;
                }
                if ((lev1 = fopen("level1.txt", "r")) == NULL){
                    puts("Can not open lev1.txt!\n");
                    return false;
                }

                if ((lev1d = fopen("level1debug.txt", "w")) == NULL){
                    puts("Can not open level1debug.txt!\n");
                    return false;
                }

                if ((lev2 = fopen("level2.txt", "r")) == NULL){
                    puts("Can not open level2.txt!\n");
                    return false;
                }
                if ((lev3 = fopen("level3.txt", "r")) == NULL){
                    puts("Can not open level3.txt!\n");
                    return false;
                }
                if ((lev4 = fopen("level4.txt", "r")) == NULL){
                    puts("Can not open level4.txt!\n");
                    return false;
                }
                if ((lev5 = fopen("level5.txt", "r")) == NULL){
                    puts("Can not open level5.txt!\n");
                    return false;
                }
                if ((lev6 = fopen("level6.txt", "r")) == NULL){
                    puts("Can not open level6.txt!\n");
                    return false;
                }
                if ((lev7 = fopen("level7.txt", "r")) == NULL){
                    puts("Can not open level7.txt!\n");
                    return false;
                }
                if ((lev8 = fopen("level8.txt", "r")) == NULL){
                    puts("Can not open level8.txt!\n");
                    return false;
                }

                for (i=0;i<=23;i++){
                    for(j=0;j<=31;j++){
                        if(l==1){
                            fscanf(lev1, "%1d", &Gol1[j][i]);
                        }
                        if(l==2){
                            fscanf(lev2, "%1d", &Gol1[j][i]);
                        }
                        if(l==3){
                            fscanf(lev3, "%1d", &Gol1[j][i]);
                        }
                        if(l==4){
                            fscanf(lev4, "%1d", &Gol1[j][i]);
                        }
                        if(l==5){
                            fscanf(lev5, "%1d", &Gol1[j][i]);
                        }
                        if(l==6){
                            fscanf(lev6, "%1d", &Gol1[j][i]);
                        }
                        if(l==7){
                            fscanf(lev7, "%1d", &Gol1[j][i]);
                        }
                        if(l==8){
                            fscanf(lev8, "%1d", &Gol1[j][i]);
                        }
                        //fprintf(lev2, "%1d ", &Gol1[i][j]);
                    }
                    //fputs("\n", lev2);
                }

                for (i=0;i<=23;i++){
                    for(j=0;j<=31;j++){
                        //fscanf(lev1, "%1d ", &Gol1[i][j]);
                        fprintf(lev1d, "%1d ", Gol1[j][i]);
                    }
                    fputs("\n", lev1d);
                }

                //int fclose(FILE *lev1);

                fclose(lev1);
                fclose(lev1d);
                fclose(lev2);
                fclose(lev3);
                fclose(lev4);
                fclose(lev5);
                fclose(lev6);
                fclose(lev7);
                fclose(lev8);

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
                SoundTime=SoundTime+SoundSpeed;
                //Apply the background
                apply_surface( skyx, 0, sky, screen , NULL);
                if(l==1)
                    apply_surface( 0, 0, level1, screen , NULL);
                if(l==2)
                    apply_surface( 0, 0, level2, screen , NULL);
                if(l==3)
                    apply_surface( 0, 0, level3, screen , NULL);
                if(l==4)
                    apply_surface( 0, 0, level4, screen , NULL);
                if(l==5)
                    apply_surface( 0, 0, level5, screen , NULL);
                if(l==6)
                    apply_surface( 0, 0, level6, screen , NULL);
                if(l==7)
                    apply_surface( 0, 0, level7, screen , NULL);

                for (i=0;i<=23;i++){
                    for(j=0;j<=31;j++){
                        if (Gol1[j][i] == 2){
                            apply_surface (z2, y2, death, screen, &clipdeath[0]);
                            z2=z2+20;
                        }
                        else if (Gol1[j][i] == 3){
                            apply_surface (z2, y2, death, screen, &clipdeath[1]);
                            z2=z2+20;
                        }
                        else if (Gol1[j][i] == 4){
                            apply_surface (z2, y2, death, screen, &clipdeath[2]);
                            z2=z2+20;
                        }
                        else if (Gol1[j][i] == 5){
                            apply_surface (z2, y2, death, screen, &clipdeath[3]);
                            z2=z2+20;
                        }
                        else if (Gol1[j][i] == 9) {
                            Xd=j*20;
                            Yd=i*20;
                            z2=z2+20;
                        }
                        else z2=z2+20;
                    }
                    z2=0;
                    y2=y2+20;
                }
                y2=0;

                Uint8 *keystates = SDL_GetKeyState( NULL );

                j=Xor/20;
                i=Yor/20;
                j1=Xor%20;
                i1=Yor%20;


                //If up is pressed
                if( keystates[ SDLK_ESCAPE ] )
                {
                    fprintf(saveW, "%d", l);
                    return false;
                }

                if( keystates[ SDLK_w ] || keystates[ SDLK_UP ] )
                {
                    //apply_surface( ( SCREEN_WIDTH - up->w ) / 2, ( SCREEN_HEIGHT / 2 - up->h ) / 2, up, screen ,NULL);
                    if(Speed==0)
                    {

                        if((Gol1[j][i+1]!=1
                        &&(i1!=0||(Gol1[j][i]!=1)))&&die==0)
                        {
                            Speed-=1;
                            velosityH=1;
                            if(Speed<-1)
                            {
                                Speed=-1;
                            }
                            if(Gol1[j][i-1]!=1){
                                Mix_PlayChannel( -1, jump, 0 );
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
                        if((Gol1[j][i+1]!=1
                        &&(i1!=0||(Gol1[j][i]!=1)))&&die==0)
                        {
                            Speed=Speed+1;
                            velosityH=0;
                            if(Speed>1)
                            {
                            Speed=1;
                            }
                            if(Gol1[j][i+2]!=1){
                                Mix_PlayChannel( -1, jump, 0 );
                            }
                        }
                    }
                }
                //If left is pressed
                if( keystates[ SDLK_a ] || keystates[ SDLK_LEFT ] )
                {
                    if((Gol1[j][i+1]!=1
                        &&(j1!=0||(Gol1[j-1][i+1 ]!=1)))&&die==0)
                        {
                    if(Speed==0)
                    Xor=Xor-4;
                    if(Speed==-1)
                        if((j1!=0||(Gol1[j-1][i-1]!=1&&Gol1[j-1][i]!=1)))
                            Xor=Xor-4;
                    if(Speed==1)
                        if(j1!=0||Gol1[j-1][i+2]!=1)
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
                    if(Gol1[j-1][i]!=1&&velosityH==1)
                        Speed=-1;
                    if(Gol1[j-1][i]!=1&&velosityH==0)
                        Speed=1;
                }
                //If right is pressed
                if( keystates[ SDLK_d ] || keystates[ SDLK_RIGHT ] )
                {
                    if((Gol1[j][i+1]!=1&&Gol1[j][i]!=1
                        &&(j1!=0||(Gol1[j+1][i+1]!=1&&Gol1[j+1][i]!=1)))&&die==0)
                            {
                    if(Speed==0)
                    Xor=Xor+4;
                    if(Speed==-1)
                        if((j1!=0||(Gol1[j+1][i-1]!=1&&Gol1[j+1][i]!=1)))
                            Xor=Xor+4;
                    if(Speed==1)
                        if(j1!=0||Gol1[j+1][i+2]!=1)
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
                    if(Gol1[j+1][i]!=1&&velosityH==1)
                        Speed=-1;
                    if(Gol1[j+1][i]!=1&&velosityH==0)
                        Speed=1;
                    }

                    if(Speed==1){
                        if(Gol1[j][i+2]==1||(j1!=0&&Gol1[j+1][i+2]==1))
                        Speed=0;
                    }else if(Speed==-1) {
                        i=(Yor-5)/20;
                        if(Gol1[j][i]==1||(j1!=0&&Gol1[j+1][i]==1))
                        Speed=0;
                    }
                    if((Gol1[j][i]>=2&&Gol1[j][i]<=5)||(Gol1[j][i+1]>=2&&Gol1[j][i+1]<=5)||(j1!=0&&((Gol1[j+1][i]>=2&&Gol1[j+1][i]<=5)||(Gol1[j+1][i+1]>=2&&Gol1[j+1][i+1]<=5)))){
                        die=1;
                    }

                    if(die==1){
                        Mix_PlayChannel( -1, death0, 0 );
                        Dtime=Dtime+Dspeed;
                        Dspeed=1;
                        Speed=0;
                        apply_surface( SCREEN_WIDTH/2-dieshow->w/2, SCREEN_HEIGHT/2-dieshow->h/2, dieshow, buttonSur , NULL);
                    }
                    if(Dtime>=100){
                        die=0;
                        Dspeed=0;
                        Xor=Xd;
                        Yor=Yd;
                        velosityH=0;
                        DeathCount=DeathCount+1;
                        snprintf (DeathScore, sizeof (DeathScore), "%d", DeathCount);
                        if(hostD != NULL)
                            SDLNet_TCP_Send(hostD, &DeathCount, sizeof(int));
                        Dtime=0;
                    }

                    if(l==8){
                        final = TTF_RenderText_Solid (font, DeathScore, redColor);
                        finalText = TTF_RenderText_Solid (font, "Your death score is:", textColor);
                        apply_surface( SCREEN_WIDTH/2-finalText->w/2, SCREEN_HEIGHT/2-finalText->h/2-50, finalText, buttonSur , NULL);
                        apply_surface( SCREEN_WIDTH/2-final->w/2, SCREEN_HEIGHT/2-final->h/2, final, buttonSur , NULL);
                        FinalTime=FinalTime+FinalSpeed;
                    }

                    if(FinalTime==1000){
                    l=9;
                    }

                    if (l == 9) {
                        finalText = TTF_RenderText_Solid (fontSmall, "Press ESC button to leave the game...", textColor);
                        FinalTime=FinalTime+FinalSpeed;
                        apply_surface( SCREEN_WIDTH/2-finalText->w/2, SCREEN_HEIGHT/2-finalText->h/2+150, finalText, buttonSur , NULL);
                        if (FinalTime == 1700)
                            frameCartoon = 1;
                        else if (FinalTime == 2400){
                            frameCartoon = 2;
                            FinalSpeed = 0;
                        }
                        apply_surface( SCREEN_WIDTH/2-cartoon->w/6, SCREEN_HEIGHT/2-cartoon->h/2, cartoon, buttonSur , &clipcartoon[frameCartoon]);

                    }

                    if(l!=9)
                        Yor=Yor+5*Speed;

                    if(Gol1[j][i]==8&&Speed==-1){
                            Speed=1;
                            velosityH=0;
                            Mix_PlayChannel( -1, jump, 0 );
                    }
                    if(Gol1[j][i+2]==8&&Speed==1){
                            Speed=-1;
                            velosityH=1;
                            Mix_PlayChannel( -1, jump, 0 );
                    }

                    if(Gol1[j][i]==6||Gol1[j][i+1]==6){
                            l=l+1;
                            if (l==5){
                                Yor=20;
                                Speed=1;
                            }else if (l==8)
                                Yor=420;
                            else{
                                if(Xor>=620)
                                Xor=20;
                            else Xor=616;
                            }
                    }
                    if(Gol1[j][i]==7||Gol1[j][i+1]==7){
                            l=l-1;
                            if (l==4)
                                Yor=420;
                                else{
                                    if(Xor>=620)
                                    Xor=20;
                                else Xor=616;
                                }
                    }

                    if( velosityH==0&&l!=9)
                    {
                        if( velosityW==1)
                        {
                            apply_surface (Xor, Yor, cap, screen, &clipRightD[frame]);
                        } else
                        {
                        apply_surface (Xor, Yor, cap, screen, &clipLeftD[frame]);
                        }
                    } else if( velosityH==1&&l!=9)
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

                    fclose(saveW);
                }while(l!=10);
            }
        clean_up();

        return 0;
        }
        return 0;
}
