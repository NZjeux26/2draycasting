#include <stdio.h>
#include "include/SDL2/SDL.h"
#include <stdbool.h>
#include "3Dfunc.h"
#include <math.h>


#define DISPLAY_WIDTH 1200
#define DISPLAY_LENGTH 800

void createWindow();
SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char* argv[]) {
    //create SDL window
    createWindow();
    Boundry wall = createBoundry(300,100,300,300);
    Ray r = createRay(100,200,0);
    float mX = 0;
    float mY = 0;
    
    //Main loop

    int is_running = 1;
  
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = 0;
            }  
            else if(event.type == SDL_MOUSEMOTION){
                mX = event.motion.x;
                mY = event.motion.y;
            }
        }
        
        //Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        float transX = DISPLAY_WIDTH / 2;
        float transY = DISPLAY_LENGTH / 2;
        float iX = 0;
        float iY = 0;

        translate(renderer,transX,transY);
        
        lookAt(&r,mX,mY);

        //draw the boundry line
        float x1 = wall.a.x; 
        float y1 = wall.a.y;
        float x2 = wall.b.x;
        float y2 = wall.b.y;
        //draw the ray + dir
        float rpx = r.pos.x;
        float rpy = r.pos.y;
        float rdx = r.dir.x;
        float rdy = r.dir.y;

        int found = cast(r,wall,&iX,&iY);

        SDL_Color sqcolour = {255,255,0,255};

        //if intersection is found, then draw a square in the x/y pos of the interception.
        if(found == 1){
           drawFilledSquare(renderer,iX + transX,iY + transY,10,sqcolour);
           //printf("X: %0.2f, Y: %0.2f\n", iX, iY);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, x1 + transX, y1 + transY, x2 + transX, y2 + transY);
        
        //we use the ray's position rpx and rpy as the starting point, and we add the ray's direction rdx and rdy to draw the line in the direction of the ray.
        SDL_RenderDrawLine(renderer, rpx + transX, rpy + transY, (rpx + rdx * 10) + transX, (rpy + rdy * 10) + transY);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); //Cap the frame rate
    }
    //Clean up and quit
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void createWindow(){
     // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    // Create a window
    window = SDL_CreateWindow(
        "2D Raycasting ARM",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH, DISPLAY_LENGTH,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        SDL_Log("Failed to create ERROR: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}