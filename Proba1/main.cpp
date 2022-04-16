#define SDL_MAIN_HANDLED
#include <glm/vec2.hpp>
#include <iostream>
#include <SDL.h>
#include "circle.h"
#include <SDL_image.h>

SDL_Rect wall1,wall2,wall3,paleta;
Circle cerc;
int vitpal=10;
bool start = false;
bool restart = false;
bool wasHit = false;


//define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//define SDL Window related variables
SDL_Window *window = NULL;
SDL_Renderer *windowRenderer = NULL;
SDL_Event currentEvent;

bool quit = false;
int hits = 0;
int mouseX, mouseY;

bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    } else {

        //Try to create the window
        window = SDL_CreateWindow(
                "SDL Hello Window Example",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if(window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        } else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if(windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            } else {

                //Set background color
                SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

                //Apply background color
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}

void processEvents() {

    //Check for events in queue
    SDL_PollEvent(&currentEvent);


    //User requests quit
    if(currentEvent.type == SDL_QUIT) {

        quit = true;
    }

    //Mouse event -> pressed button
    if(currentEvent.type == SDL_MOUSEBUTTONDOWN) {

        if(currentEvent.button.button == SDL_BUTTON_LEFT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse click => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    //Mouse event -> mouse movement
    if(currentEvent.type == SDL_MOUSEMOTION) {

        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Mouse move => " << "x: " << mouseX << ", y: " << mouseY << std::endl;
        }
    }

    //Keyboard event
    if(currentEvent.type == SDL_KEYDOWN) {

        switch(currentEvent.key.keysym.sym) {

            case SDLK_UP:
                if (paleta.y >20) {
                    if (hits == 3) {
                        vitpal = vitpal * 2;
                        hits=0;
                    }
                    paleta.y -= vitpal;
                }
                break;

            case SDLK_DOWN:
                if (paleta.y < 480)
                {
                    if (hits == 3) {
                        vitpal = vitpal * 2;
                    }
                    paleta.y += 10;
                }
                break;

            case SDLK_r:
                if (restart == 0) {
                    restart = 1; 
                    start = 0;
                }
                else  restart = 0; 
                break;

            case SDLK_s:
                if (start== 0) start = 1;
                else start = 0;
                break;

            case SDLK_ESCAPE:

                quit = true;
                break;

            default:
                break;
        }
    }
}
glm::vec2 pos(3.8, 2.8);
glm::vec2 vel(1, 2);
glm::vec2 pospal(0, 2.5);

void animationSetUp() {
    static int lastTime = SDL_GetTicks();
    int now = SDL_GetTicks();
    float dt = (now - lastTime) / 1000.0;
    lastTime = now;
  

    glm::vec2 acc(0, 0);

    pos += dt * vel;
    vel += dt * acc;

    // TOP WALL
    if (pos.y > 4.68 ) {
        vel.y = (-1)*vel.y ;
    } // RIGHT WALL
    else if (pos.x > 6.8)
    {
        vel.x = -(-1) * vel.y;
    } // BOTTOM WALL
    else if (pos.y < 0.2)
    {
        vel.y = (-1) * vel.y;
    }

  

    // BALL HITS OR MISSES PALETTE
    if (cerc.center.x <= 32 && cerc.center.y >= paleta.y && cerc.center.y <= paleta.y + 100) {
        vel.x = abs(vel.x);
        
        if(!wasHit) 
        {
            hits++;

            wasHit = true;
        }
        
        if (hits == 3) {
            vel.x = 2 * vel.x;
            vel.y = 2 * vel.y;
        }
    }
    else wasHit = false;
    if (pos.x< 0.2 && pos.y>0.2 && pos.y<5.8) {
        pos.x = 3.8;
        pos.y = 2.8;
        hits = 0;
        wasHit = false;
    }

    cerc.center.x = pos.x * 100;
    cerc.center.y = 500 - pos.y * 100;
}

void initObject() {
    wall1.x = 0;
    wall1.y = 0;
    wall1.h = 20;
    wall1.w = 800;

    wall2.x = 780;
    wall2.y = 0;
    wall2.h = 600;
    wall2.w = 20;

    wall3.x = 0;
    wall3.y = 580;
    wall3.h = 20;
    wall3.w = 800;


    paleta.x = 0;
    paleta.y = 250;
    paleta.h = 100;
    paleta.w = 20;

    cerc.radius = 12.0f;
    cerc.center.x = 400.0f;
    cerc.center.y = 300.0f;
    cerc.color = glm::vec4(255, 0, 0, 255);
}

void drawFrame() {

    //Clear the background
    SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);
    SDL_RenderClear(windowRenderer);

    //TOP WALL
    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 250, 255);
    SDL_RenderFillRect(windowRenderer, &wall1);

    //RIGHT WALL
    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 250, 255);
    SDL_RenderFillRect(windowRenderer, &wall2);

    //BOTTOM WALL
    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 250, 255);
    SDL_RenderFillRect(windowRenderer, &wall3);

    //PALETTE - Controlled by player
    SDL_SetRenderDrawColor(windowRenderer, 250, 0, 0, 255);
    SDL_RenderFillRect(windowRenderer, &paleta);

    //THE BALL
    SDL_FillCircle(windowRenderer, cerc);
    if (start == 1) {
        animationSetUp();
    }
    if (restart == 1) {
        initObject();
        glm::vec2 pos(3.8, 2.8);
        glm::vec2 vel(1, 2);
        glm::vec2 pospal(0, 2.5);
       }

  

    //Update window
    SDL_RenderPresent(windowRenderer);
}

void cleanup() {

    //Destroy renderer
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }

    //Destroy window
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }

    //Quit SDL
    SDL_Quit();
}

int main(int argc, char * argv[]) {
    //Initialize windo
    initObject();

    if(!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    //Game loop
    while (!quit) {

        processEvents();
        drawFrame();
    }

    cleanup();
    return 0;
}