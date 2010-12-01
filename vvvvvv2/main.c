#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
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
SDL_Rect clips [4];
SDL_Rect box;



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



SDL_Surface *Load_Image( char* filename ){

    //Загруженное изображение
    SDL_Surface* loadedImage = NULL;

    //Используемая оптимизированная поверхность
    SDL_Surface* optimizedImage = NULL;

    //Подгрузка изображения
    loadedImage = IMG_Load( filename );

    //Проверяем, загружено ли изображение
    if( loadedImage != NULL ){
        //Создаем оптимизированную поверхность
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Освобождаем старую поверхность
        SDL_FreeSurface( loadedImage );

        //Проверка оптимизации поверхности
        if( optimizedImage != NULL ){
            //Хромокей
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Возвращаем оптимизированную поверхность
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

    //Устанавливаем экран
    buttonSur = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    addSur = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //Проверка на наличие ошибок при установке экрана
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

    //If everything initialized fine
    return true;
}

int load_files()
{
    //Подгрузка фонового изображения
    mbackground = Load_Image( "bg.png" );

    //Подгрузка шрифтов
    font = TTF_OpenFont( "font.ttf", 50 );
    fontSmall = TTF_OpenFont( "font.ttf", 38 );

    //Наличие ошибой при загрузке фона
    if( mbackground == NULL ){
        return 1;
    }

    //Наличие обибок при загрузке шрифтов
    if( font == NULL ){
        return 1;
    }

    if( fontSmall == NULL ){
        return 1;
    }
    //Load the background image
    background = Load_Image( "level1.png" );
    sky = Load_Image( "sky.png" );
    capp = Load_Image( "cap!.png" );
    cap = Load_Image( "Cap2.png" );

    //If there was a problem in loading the background
    if( background == NULL )
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

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

int menu (){
    //Условие выхода
    int quit = 1;

    //Инициализация
    if( init() != 1 ){
        return 0;
    }

    //Подгрузка файлов
    if( load_files() != 1 ){
        return 0;
    }

    //Счетчик пунктов меню
    int x = 0;

    //Генерируем поверхности с текстом
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

    //Применяем фон
    apply_surface( 0, 0, mbackground, screen , NULL);
    apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
    apply_surface( 50, 90, ButContinue, buttonSur , NULL);
    apply_surface( 50, 150, ButHowTo, buttonSur , NULL);
    apply_surface( 50, 210, ButCredits, buttonSur , NULL);
    apply_surface( 50, 270, ButExit, buttonSur , NULL);

    //До тех пор, пока пользователь не вышел
    while( quit == 1 ){

        //Наличие событий для исполнения
        if( SDL_PollEvent( &event ) && event.type == SDL_KEYDOWN ){

            //Применяем фон
            apply_surface( 0, 0, mbackground, screen , NULL);

            //Пoлучаем состояния клавиш
            Uint8 *keystates = SDL_GetKeyState( NULL );

            //Нажата клавиша "ВВЕРХ" или "ВЛЕВО"
            if( keystates[ SDLK_UP ] || keystates[ SDLK_LEFT ] ){
                x--;
                if (x<1){
                    x=5;
                }
                //printf("%d\n", x);
            }
            //Нажата клавиша "ВНИЗ" или "ВПРАВО"
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


            //Меню
            apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
            apply_surface( 50, 90, ButContinue, buttonSur , NULL);
            apply_surface( 50, 150, ButHowTo, buttonSur , NULL);
            apply_surface( 50, 210, ButCredits, buttonSur , NULL);
            apply_surface( 50, 270, ButExit, buttonSur , NULL);

            //Новая игра
            if(x==1){
                apply_surface( 50, 30, ButNewGameA, buttonSur , NULL);
                apply_surface( 50, 90, ButContinue, buttonSur , NULL);
                apply_surface( 50, 270, ButExit, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    return 1;
                }
            }

            //Продолжить
            if(x==2){
                apply_surface( 50, 90, ButContinueA, buttonSur , NULL);
                apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
                apply_surface( 50, 150, ButHowTo, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    quit = 0;
                }
            }

            //Инструкция
            if(x==3){
                apply_surface( 50, 150, ButHowToA, buttonSur , NULL);
                apply_surface( 50, 90, ButContinue, buttonSur , NULL);
                apply_surface( 50, 210, ButCredits, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    apply_surface( 50, 340, HowTo1, addSur , NULL);
                    //apply_surface( 50, 380, HowTo2, addSur );
                }
            }

            //Создатели
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

            //Выход
            if(x==5){
                apply_surface( 50, 270, ButExitA, buttonSur , NULL);
                apply_surface( 50, 210, ButCredits, buttonSur , NULL);
                apply_surface( 50, 30, ButNewGame, buttonSur , NULL);
                if( keystates[ SDLK_SPACE ] ){
                    quit = 0;
                }
            }

            //Если пользователь жмякнул по Х
            else if( event.type == SDL_QUIT ){
                //Условие выхода
                quit = 0;
            }
        }

        //Обновление экрана
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

    //Выгрузить шрифт
    TTF_CloseFont( font );
    TTF_CloseFont( fontSmall );

    return 0;
}

int main( int argc, char* args[] )
{
    if(menu()==0)
    return false;
    else{
        static int Xor = 100;
        static int Yor = 100;
        static int Speed = 0;
        static int velosityH = 0;
        static int velosityW = 0;
        static int frame = 0;
        static int skyx = 0;
        static int skys = 2;
        int Gol1[32][24], j, i;
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

        fclose(lev1);
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
            if( keystates[ SDLK_ESCAPE ] )
                return false;

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
                if(j1==0&&Gol1[j-1][i]==0&&velosityH==1)
                    Speed=-1;
                if(Gol1[j-1][i]==0&&velosityH==0)
                    Speed=1;
            }

            //If right is pressed
            if( keystates[ SDLK_d ] || keystates[ SDLK_RIGHT ] )
            {
                if(Gol1[j][i+1]==0
                    &&(j1!=0||(Gol1[j+1][i+1]==0)))
                        {
                //apply_surface( ( SCREEN_WIDTH / 2 - right->w ) / 2 + ( SCREEN_WIDTH / 2 ), ( SCREEN_HEIGHT - right->h ) / 2, right, screen ,NULL);
                if(Speed==0)
                Xor=Xor+4;
                if(Speed==-1)
                    if((j1!=0||(Gol1[j+1][i-1]==0&&Gol1[j+1][i]==0)))
                        Xor=Xor+4;
                if(Speed==1)
                    if(j1!=0||Gol1[j+1][i+2]==0)
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
                if(Gol1[j+1][i]==0&&velosityH==1)
                    Speed=-1;
                if(Gol1[j+1][i]==0&&velosityH==0)
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

        //Clean up
        clean_up();

        return 0;
    }
    return 0;
}
