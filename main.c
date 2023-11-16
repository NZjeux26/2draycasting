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
Boundry walls[4];

void initializeWalls() {
    walls[0] = createBoundry(-25, -25, -25, 25);
    walls[1] = createBoundry(-25, 25, 25, 25);
    walls[2] = createBoundry(25, 25, 25, -25);
    walls[3] = createBoundry(25, -25, -25, -25);
}

int main(int argc, char* argv[]) {
    //create SDL window
    createWindow();
     //values for mouse input
    float mX = 0;
    float mY = 0;
    SDL_Color sqcolour = {255,255,0,255};
    SDL_Color sqcolour2 = {250,155,10,5};
    //Boundry wall = createBoundry(0,-300,0,0);
    Boundry walls[4];
    //Ray r = createRay(100,200,0);
    walls[0] = createBoundry(-250, -250, -250, 250);
    walls[1] = createBoundry(-250, 250, 250, 250);
    walls[2] = createBoundry(250, 250, 250, -250);
    walls[3] = createBoundry(250, -250, -250, -250);
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
        //values for ray/wall interception.
        float iX = 0;
        float iY = 0;
        
        translate(renderer,transX,transY);

        float x1,x2 = 0;
        float y1,y2 = 0;
        for(int w = 0; w < 4; w++){//draw the boundry line
            x1 = walls[w].a.x; 
            y1 = walls[w].a.y;
            x2 = walls[w].b.x;
            y2 = walls[w].b.y;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, x1 + transX, y1 + transY, x2 + transX, y2 + transY);
        }
        
        //draw userpoint
        
        Particle particle = createParticle(mX,mY); //particle with the x/y pos being the mouse x/y
       // drawFilledSquare(renderer, mX, mY, 7, sqcolour2);

        for (int a = 0; a < 360; a+=10){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //draw the ray + dir
            float rpx = particle.rays[a].pos.x;
            float rpy = particle.rays[a].pos.y;
            float rdx = particle.rays[a].dir.x;
            float rdy = particle.rays[a].dir.y;
            
            //we use the ray's position rpx and rpy as the starting point, and we add the ray's direction rdx and rdy to draw the line in the direction of the ray.
            SDL_RenderDrawLine(renderer, rpx, rpy, (rpx + rdx * 20), (rpy + rdy * 20));//with using the mouse x/y we no longer have to use transx/y
            int found = 2;
            //outer loops is for checking rays against each wall
            for(int j = 0; j < 4; j++){
                found = cast(particle.rays[a],walls[j],&iX,&iY);
                 //if intersection is found, then draw a square in the x/y pos of the interception for that wall and draw. ** Currently not stopping after it hits something but that's intentional for now.
                if(found == 1){
                    drawFilledSquare(renderer,iX + transX,iY + transY,5,sqcolour);
                    printf("Angle: %d, Pos: (%f, %f), Dir: (%f, %f)\n", a, rpx, rpy, rdx, rdy);

                    //draw line from particle to contact point.
                    SDL_RenderDrawLine(renderer,mX,mY,iX + transX, iY + transY);
                }
            } 
        }

        //we use the ray's position rpx and rpy as the starting point, and we add the ray's direction rdx and rdy to draw the line in the direction of the ray.
        //SDL_RenderDrawLine(renderer, rpx + transX, rpy + transY, (rpx + rdx * 10) + transX, (rpy + rdy * 10) + transY);
        
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