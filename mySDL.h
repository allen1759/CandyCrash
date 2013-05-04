#ifndef MYSDL_H_INCLUDED
#define MYSDL_H_INCLUDED

#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_TTF.h"
using namespace std;

bool InitSDL( SDL_Surface * &screen, const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const int SCREEN_BPP, string WindowCaption );
SDL_Surface *LoadImage( string filename );
SDL_Surface *LoadImage( string filename, int colorTransparent[] );
void ApplySurface( int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect* clip=NULL );
bool LoadImageFile( SDL_Surface * &surface, string filename );
bool LoadImageFile( SDL_Surface * &surface, string filename, int colorTransparent[] );
bool LoadFontFile( TTF_Font * &font, string filename, int fontSize );
void CleanUpSurface( SDL_Surface *surface);
void CleanUpFont( TTF_Font *font);
void QuitSDL();

//The timer
class Timer
{
private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unPause();

    //Gets the timer's time
    int getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();
};

enum MouseButton
{
    NOBUTTON,
    LEFTPRESS,
    LEFTRELEASE,
    RIGHTPRESS,
    RIGHTRELEASE
};
class CButton
{
public:
    //The attributes of the button
    SDL_Rect box;
    //Initialize the variables
    CButton( int x, int y, int w, int h );
    CButton();

    //Handles events and set the button's sprite region
    MouseButton IsPress(SDL_Event &event);
    bool IsMoveIn( SDL_Event &event, int &x, int &y );
    void ChangeRect( int x, int y, int w, int h );
};

#endif // MYSDL_H_INCLUDED
