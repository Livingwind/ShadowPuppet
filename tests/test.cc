#include <iostream>
#include "SDL.h"
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include <string>

using namespace std;

//Function declarations
void openGamepad();
bool loadTexture(string image);
int initialize();
void quitGame();
bool initializeGL();
void glRender();

//Private variables
//Window dimensions
static const int WINDOW_HEIGHT = 480;
static const int WINDOW_WIDTH = 640;
//Pointers for our window and renderer, and controller
static SDL_Window *window;
static SDL_GameController *controller;
//OpenGL context and other variables
static SDL_GLContext gameContext;
static GLuint textureID = 0;
//Controller deadzone
static const int deadzone = 8000;
//An event to be polled
static SDL_Event event;
//Bool's for if the game is running and if a controller has been connected
static bool gameRunning = true;
static bool gamepadConnected = false;
//Player rect
static SDL_Rect player;
//X velocity for our player
static const int xVel = 1;

//TODO: Clean up main
int main(int argc, char *argv[]){
    if(initialize() == 1){
        cerr << argv[0] << ": Unable to initialize, exiting\n";
        quitGame();
        return 1;
    }
//     int time = 0; //Used for below if statement to keep loop from running infinitely, used when fixing memory leak (lockout situation)
    
    //Main game loop
    while(gameRunning){
        
/*                  Prevent program from running very long (fixing a memory leak from not dealloc-ing gl textures)  */

//         if(time > 300){
//             gameRunning = false;
//         }
//         time++;

        //TODO: Keep/fix delay for smoothness?
        /*
        SDL_Delay(30); <--Commented out because of its unusual response time while a controller is connected
        I believe this bug comes from the OS not allowing such small slices of time to be handed out
        */
        
        //Event Handling
        SDL_PollEvent(&event);
        //In case of quit
        if(event.type == SDL_QUIT){
            gameRunning = false;
        }
        //If a controller is connected during run
        if((event.type == SDL_CONTROLLERDEVICEADDED) && (!gamepadConnected)){
            openGamepad();
        }
        //If controller is disconnected during run
        if(event.type == SDL_CONTROLLERDEVICEREMOVED){
            SDL_GameControllerClose(controller);
            gamepadConnected = false;
            cout << argv[0] << ": Detected that gamepad was disconnected\n";
        }
        
        //Keyboad input (for testing)
        if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_RIGHT:
                    player.x += xVel;
                    break;
                case SDLK_LEFT:
                    player.x -= xVel;
                    break;
                case SDLK_ESCAPE:
                    gameRunning = false;
                    break;
            }
        }
        //Controller input
        if(gamepadConnected){
            if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > deadzone){
                player.x += xVel;
            }
            if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -deadzone){
                player.x -= xVel;
            }
        }
        
        //Check to keep player on screen
        if(player.x < 0){
            player.x = 0;
        }
        if(player.x > 600){
            player.x = 600;
        }
        
        //Rendering to screen
        glRender();
        SDL_GL_SwapWindow(window);
    }
    quitGame();
    return 0;
}

//Function that initializes SDL and other libraries
int initialize(){
    //Starting SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    //Initializing OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    //Creating a window
    window = SDL_CreateWindow("ShadowPuppet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if(window == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create window! Error: %s\n", SDL_GetError());
        return 1;
    }
    
    //Create OpenGL context
    gameContext = SDL_GL_CreateContext(window);
    if(gameContext == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context. Error: %s\n", SDL_GetError());
        return 1;
    }
    
    //Initialize OpenGL
    if(!initializeGL()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize OpenGL! Error: %s\n", SDL_GetError());
        return 1;
    }
    
    //Setting player initial position
    player.x = 0;
    player.y = 440;
    player.w = 40;
    player.h = 40;
    
    //Opening the gamepad if one is connected
    openGamepad();
    return 0;
}

//Function to start up OpenGL via OpenGL code
bool initializeGL(){
    GLenum error = GL_NO_ERROR;
    //Initialize projection matrix
    glMatrixMode(GL_PROJECTION);
    //Replaces current matrix with identity matrix
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL. Error: %s\n", gluErrorString(error));
        return false;
    }
    //Initialize Modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    error = glGetError();
    if(error != GL_NO_ERROR){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL. Error: %s\n", gluErrorString(error));
        return false;
    }
    //Initialize clear color
    glClearColor(0.f, 0.f, 0.f, 1.f);
    error = glGetError();
    if(error != GL_NO_ERROR){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize OpenGL. Error: %s\n", gluErrorString(error));
        return false;
    }
    //Setting up the orthographic coordinate system
    glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, 1.0, -1.0);
    return true;
}

//Function to shut down all libraries and exit
void quitGame(){
    SDL_GL_DeleteContext(gameContext);
    SDL_DestroyWindow(window);
    if(gamepadConnected){
        SDL_GameControllerClose(controller);
    }
    SDL_Quit();
}

/*
Function that takes an image path, creates a surface from the image, and then a texture from the surface via OpenGL
Params: the path to the image we're using as a c++ string
*/
bool loadTexture(string image){
    //Loading SDL surface from bmp TODO: Implement SDL_Image for this process
    SDL_Surface *surface = SDL_LoadBMP(image.data());
    if(!surface){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load surface from bmp: %s Error: %s\n", image.data(), SDL_GetError());
        return false;
    }
    //TODO: Also render player texture
    glGenTextures(1, &textureID);
    //Binding to the texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    //Setting up the texture's settings
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Actually creating the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    //Freeing the surface, since we don't need it anymore
    SDL_FreeSurface(surface);
    return true;
}

/*
Funtion that does the OpenGL side of rendering textures that are loaded from images
*/
void glRender(){
    //Clear the context first
    glClear(GL_COLOR_BUFFER_BIT);
    //Need this to be enabled for this portion
    glEnable(GL_TEXTURE_2D);
    //TODO: Also load player texture
    if(!loadTexture(string("../images/wht-marble24Bit.bmp"))){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load texture from string! Error: %s\n", SDL_GetError());
        return;
    }
    //Render texture onto target
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);                           //Top left
        glTexCoord2f(1, 0); glVertex2f(WINDOW_WIDTH, 0);                //Top right
        glTexCoord2f(1, 1); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);    //Bottom right
        glTexCoord2f(0, 1); glVertex2f(0, WINDOW_HEIGHT);               //Bottom left
    glEnd();
    //Done with rendering, disable this flag
    glDisable(GL_TEXTURE_2D);
    //Delete texture now that we're done with it (and avoiding a very bad memory leak!)
    glDeleteTextures(1, &textureID);
}

//Function to open a gamepad
void openGamepad(){
    int joysticks = SDL_NumJoysticks();
    //Loop through the joysticks, if it's a valid controller, open it
    for(int joystickIndex = 0; joystickIndex < joysticks; ++joystickIndex){
        if(!SDL_IsGameController(joystickIndex)){
            continue;
        }
        cout << "Joystick at index: " << joystickIndex << " is a valid game controller\n";  //TODO: Remove this line at some point
        controller = SDL_GameControllerOpen(joystickIndex);
        gamepadConnected = true;
    }
}