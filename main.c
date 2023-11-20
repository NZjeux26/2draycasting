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

int main(int argc, char* argv[]) {
    //create SDL window
    createWindow();
    float transX = DISPLAY_WIDTH / 2;
    float transY = DISPLAY_LENGTH / 2;
     //values for mouse input
    float mX = 0;
    float mY = 0;
   
    Boundry walls[4];
   
    //transX/Y added here so correct checking is done by the cast function, simply translating it in the render doesn't pass that extra information into the cast function which will check against the actual X/Y values not the rendered ones.
    // walls[0] = createBoundry(-250 + transX, -250 + transY, -250 + transX, 250 + transY);
    // walls[1] = createBoundry(-250 + transX, 250 + transY, 250 + transX, 250 + transY);
    // walls[2] = createBoundry(250 + transX, 250 + transY, 250 + transX, -250 + transY);
    // walls[3] = createBoundry(250 + transX, -250 + transY, -250 + transX, -250 + transY);
    for(int i = 0; i < 4; i++){ // create walls with random points on screen.
        float x1 = rand() % (int)(DISPLAY_WIDTH);
        float y1 = rand() % (int)(DISPLAY_LENGTH);
        float x2 = rand() % (int)(DISPLAY_WIDTH);
        float y2 = rand() % (int)(DISPLAY_LENGTH);
        walls[i] = createBoundry(x1,y1,x2,y2); 
    }
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
        
        //values for ray/wall interception.
        float iX = 0;
        float iY = 0;
        
        translate(renderer,transX,transY);

        float x1,x2 = 0;
        float y1,y2 = 0;
        //draw the walls
        for(int w = 0; w < 4; w++){//draw the boundry line
            x1 = walls[w].a.x; 
            y1 = walls[w].a.y;
            x2 = walls[w].b.x;
            y2 = walls[w].b.y;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        }
        
        Particle particle = createParticle(mX,mY); //particle with the x/y pos being the mouse x/y

        for (int a = 0; a < 360; a+=10){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //draw the ray + dir
            float rpx = particle.rays[a].pos.x;
            float rpy = particle.rays[a].pos.y;
            float rdx = particle.rays[a].dir.x;
            float rdy = particle.rays[a].dir.y;

            Vector3 closest = {0,0,0};
            float record = 999999999;
            
            //outer loops is for checking rays against each wall
            for(int j = 0; j < 4; j++){
                //found = cast(particle.rays[a],walls[j],&iX,&iY);
                Vector3 pt = cast(particle.rays[a],walls[j],iX,iY);
                 //if intersection is found, then draw a square in the x/y pos of the interception for that wall and draw. ** Currently not stopping after it hits something but that's intentional for now.
                if(pt.x != 0 || pt.y !=0){ //if the x and y values are blank on return, no intercept and there do not display anything.
                    float d = dist(particle.rays[a].pos,pt);//measure the distance between the interception and the ray and record which is the closest
                    if(d < record){
                        record = d;
                        closest = pt;
                    }
                }
            } 
            if(closest.x != 0 || closest.y != 0){
                //draw line from particle to contact point that is closest to the emitter
                SDL_RenderDrawLine(renderer,mX,mY,closest.x,closest.y);
            }
            //we use the ray's position rpx and rpy as the starting point, and we add the ray's direction rdx and rdy to draw the line in the direction of the ray.
            SDL_RenderDrawLine(renderer, rpx, rpy, (rpx + rdx * 20), (rpy + rdy * 20));//with using the mouse x/y we no longer have to use transx/y
        }
        
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