#include "mySDL.h"

bool InitSDL( SDL_Surface * &screen, const int SCREEN_WIDTH, const int SCREEN_HEIGHT, const int SCREEN_BPP, string WindowCaption )
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

    //Set the window caption
    SDL_WM_SetCaption( WindowCaption.c_str(), NULL );

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }


    //If everything initialized fine
    return true;
}

SDL_Surface *LoadImage( string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Return the optimized image
    return optimizedImage;
}

SDL_Surface *LoadImage( string filename, int colorTransparent[] )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );

        //If the image was optimized just fine
        if( optimizedImage != NULL )
        {
            //Map the color key
            Uint32 colorkey = SDL_MapRGB( optimizedImage->format, colorTransparent[0], colorTransparent[1], colorTransparent[2] );

            //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
        }
    }

    //Return the optimized image
    return optimizedImage;
}

void ApplySurface( int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect* clip )
{
    //Temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Get the offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool LoadImageFile( SDL_Surface * &surface, string filename )
{
    //Load the image
    surface = LoadImage( filename.c_str() );

    //If there was an error in loading the image
    if( surface == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

bool LoadImageFile( SDL_Surface * &surface, string filename, int colorTransparent[] )
{
    //Load the image
    surface = LoadImage( filename.c_str(), colorTransparent );

    //If there was an error in loading the image
    if( surface == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

bool LoadFontFile( TTF_Font * &font, string filename, int fontSize )
{
    //Open the font
    font = TTF_OpenFont( filename.c_str(), fontSize );

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void CleanUpSurface( SDL_Surface *surface)
{
    //Free the surface
    SDL_FreeSurface( surface );
}

void CleanUpFont( TTF_Font *font)
{
    //Close the font that was used
    TTF_CloseFont( font );
}

void QuitSDL()
{
    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}
void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}
void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}
void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void Timer::unPause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}
int Timer::getTicks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}
bool Timer::isStarted()
{
    return started;
}
bool Timer::isPaused()
{
    return paused;
}

//Initialize the variables
CButton::CButton( int x, int y, int w, int h )
{
    //Set the button's attributes
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;
}
CButton :: CButton()
{}
//Handles events and set the button's sprite region
MouseButton CButton::IsPress(SDL_Event &event)
{
    //The mouse offsets
    int x = 0, y = 0;

    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                return LEFTPRESS;
            }
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                return LEFTRELEASE;
            }
        }
    }

    //If a mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //If the left mouse button was pressed
        if( event.button.button == SDL_BUTTON_RIGHT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                return RIGHTPRESS;
            }
        }
    }
    //If a mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_RIGHT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                return RIGHTRELEASE;
            }
        }
    }
}
bool CButton::IsMoveIn( SDL_Event &event, int &x, int &y )
{
    //The mouse offsets
    //int x = 0, y = 0;

    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
        //Get the mouse offsets
        x = event.motion.x;
        y = event.motion.y;

        //If the mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            return true;
        }
        //If not
        else
        {
            return false;
        }
    }
}
void CButton::ChangeRect( int x, int y, int w, int h )
{
    this->box.x = x;
    this->box.y = y;
    this->box.w = w;
    this->box.h = h;
}
