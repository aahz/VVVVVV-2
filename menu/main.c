//Заголовочные файлы
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>


//Параметры экрана
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

//Поверхности
SDL_Surface *background = NULL;
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
SDL_Surface *screen = NULL;
SDL_Surface *buttonsheet=NULL;
SDL_Surface *buttonSur=NULL;

SDL_Surface *addSur=NULL;

SDL_Surface *HowTo1 = NULL;
SDL_Surface *HowTo2 = NULL;

SDL_Surface *Credits = NULL;
SDL_Surface *Name1 = NULL;
SDL_Surface *Name2 = NULL;

//Структура событий
SDL_Event event;
SDL_Rect clips [4];
SDL_Rect box;


//Шрифты
TTF_Font *font = NULL;
TTF_Font *fontSmall = NULL;

//Варианты цветов
SDL_Color textColor = { 255, 255, 255 };
SDL_Color currentColor1 = { 48, 70, 68 };
SDL_Color currentColor2 = { 55, 113, 93 };
SDL_Color currentColor3 = { 76, 179, 93 };
SDL_Color currentColor4 = { 127, 226, 106 };
SDL_Color currentColor5 = { 184, 234, 87 };
SDL_Color redColor = { 201, 23, 23 };


SDL_Surface *load_image( char* filename ){

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

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ){

    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, NULL, destination, &offset );
}

int init(){

    //Инициализируем подсистему SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ){
        return 1;
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

    //Устанавливаем экран
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //Проверка на наличие ошибок при установке экрана
    if( screen == NULL ){
        return 1;
    }

    //Инициализируем SDL_ttf
    if( TTF_Init() == -1 ){
        return 1;
    }

    //Устанавливаем заголовок окна
    SDL_WM_SetCaption( "VVV VVV 2", NULL );

    //Если все инициализированно правильно
    return 0;
}

int load_files(){
    //Подгрузка фонового изображения
    background = load_image( "bg.png" );

    //Подгрузка шрифтов
    font = TTF_OpenFont( "font.ttf", 50 );
    fontSmall = TTF_OpenFont( "font.ttf", 38 );

    //Наличие ошибой при загрузке фона
    if( background == NULL ){
        return 1;
    }

    //Наличие обибок при загрузке шрифтов
    if( font == NULL ){
        return 1;
    }

    if( fontSmall == NULL ){
        return 1;
    }

    //Если все загружено верно
    return 0;
}

void clean_up(){
    //Выгр поверхности
    SDL_FreeSurface( background );
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

    //Выгрузить SDL_ttf
    TTF_Quit();

    //Выгрузить SDL
    SDL_Quit();
}

int main( int argc, char* args[] ){
    //Условие выхода
    int quit = 1;

    //Инициализация
    if( init() == 1 ){
        return 1;
    }

    //Подгрузка файлов
    if( load_files() == 1 ){
        return 1;
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
    apply_surface( 0, 0, background, screen );
    apply_surface( 50, 30, ButNewGame, buttonSur );
    apply_surface( 50, 90, ButContinue, buttonSur );
    apply_surface( 50, 150, ButHowTo, buttonSur );
    apply_surface( 50, 210, ButCredits, buttonSur );
    apply_surface( 50, 270, ButExit, buttonSur );

    //До тех пор, пока пользователь не вышел
    while( quit == 1 ){

        //Наличие событий для исполнения
        if( SDL_PollEvent( &event ) && event.type == SDL_KEYDOWN ){

            //Применяем фон
            apply_surface( 0, 0, background, screen );

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
            apply_surface( 50, 30, ButNewGame, buttonSur );
            apply_surface( 50, 90, ButContinue, buttonSur );
            apply_surface( 50, 150, ButHowTo, buttonSur );
            apply_surface( 50, 210, ButCredits, buttonSur );
            apply_surface( 50, 270, ButExit, buttonSur );

            //Новая игра
            if(x==1){
                apply_surface( 50, 30, ButNewGameA, buttonSur );
                apply_surface( 50, 90, ButContinue, buttonSur );
                apply_surface( 50, 270, ButExit, buttonSur );
                if( keystates[ SDLK_SPACE ] ){
                    quit = 0;
                }
            }

            //Продолжить
            if(x==2){
                apply_surface( 50, 90, ButContinueA, buttonSur );
                apply_surface( 50, 30, ButNewGame, buttonSur );
                apply_surface( 50, 150, ButHowTo, buttonSur );
                if( keystates[ SDLK_SPACE ] ){
                    quit = 0;
                }
            }

            //Инструкция
            if(x==3){
                apply_surface( 50, 150, ButHowToA, buttonSur );
                apply_surface( 50, 90, ButContinue, buttonSur );
                apply_surface( 50, 210, ButCredits, buttonSur );
                if( keystates[ SDLK_SPACE ] ){
                    //SDL_BlitSurface( HowTo1, NULL, addSur, NULL );
                    //SDL_BlitSurface( HowTo2, NULL, addSur, NULL );
                    //SDL_BlitSurface( Credits, NULL, screen, NULL );
                    //SDL_BlitSurface( Name1, NULL, screen, NULL );
                    //SDL_BlitSurface( Name2, NULL, screen, NULL );
                    apply_surface( 50, 340, HowTo1, addSur );
                    apply_surface( 50, 380, HowTo2, addSur );
                }
            }

            //Создатели
            if(x==4){
                apply_surface( 50, 210, ButCreditsA, buttonSur );
                apply_surface( 50, 150, ButHowTo, buttonSur );
                apply_surface( 50, 270, ButExit, buttonSur );
                if( keystates[ SDLK_SPACE ] ){
                    //SDL_BlitSurface( Credits, NULL, addSur, NULL );
                    //SDL_BlitSurface( Name1, NULL, addSur, NULL );
                    //SDL_BlitSurface( Name2, NULL, addSur, NULL );
                    //SDL_BlitSurface( HowTo1, NULL, screen, NULL );
                    //SDL_BlitSurface( HowTo2, NULL, screen, NULL );
                    apply_surface( 50, 340, Credits, addSur );
                    apply_surface( 150, 400, Name1, addSur );
                    apply_surface( 150, 440, Name2, addSur );
                }
            }

            //Выход
            if(x==5){
                apply_surface( 50, 270, ButExitA, buttonSur );
                apply_surface( 50, 210, ButCredits, buttonSur );
                apply_surface( 50, 30, ButNewGame, buttonSur );
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


    //Очистка и высвобожение памяти
    clean_up();

    return 0;
}
