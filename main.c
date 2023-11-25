#include <stdio.h>
#include "include/SDL2/SDL.h"
#include <stdbool.h>
#include "3Dfunc.h"
#include <math.h>
#include <time.h>


#define DISPLAY_WIDTH 1200
#define DISPLAY_LENGTH 800

void createWindow();
SDL_Window* window;
SDL_Renderer* renderer;
Boundry walls[4];
//SDL_Rect leftViewport = { 0, 0, DISPLAY_WIDTH / 2, DISPLAY_LENGTH };

int main(int argc, char* argv[]) {
    //create SDL window
    createWindow();
    float transX = DISPLAY_WIDTH / 2;
    float transY = DISPLAY_LENGTH / 2;
     //values for mouse input
    float mX = 0;
    float mY = 0;
    time_t t;
    Boundry walls[8];
    srand((unsigned int)time(&t));
    
    SDL_Rect rightViewport = { DISPLAY_WIDTH / 2, 0, DISPLAY_WIDTH / 2, DISPLAY_LENGTH };
    SDL_Rect leftViewport = { 0, 0, DISPLAY_WIDTH / 2, DISPLAY_LENGTH };
    
    SDL_RenderSetViewport(renderer, &leftViewport);
    for(int i = 0; i < 4; i++){ // create walls with random points on screen.
        float x1 = rand() % (int)(leftViewport.w);
        float y1 = rand() % (int)(leftViewport.h);
        float x2 = rand() % (int)(leftViewport.w);
        float y2 = rand() % (int)(leftViewport.h);
        walls[i] = createBoundry(x1,y1,x2,y2); 
    }
    //adding the leftviewport boundries as the correct wall boundries.
    walls[4] = createBoundry(0, 0, leftViewport.w, 0);
    walls[5] = createBoundry(leftViewport.w, 0, leftViewport.w, leftViewport.h);
    walls[6] = createBoundry(0, leftViewport.h, leftViewport.w, leftViewport.h);
    walls[7] = createBoundry(0, 0, 0, leftViewport.h);
    // walls[4] = createBoundry(0,0, DISPLAY_WIDTH,0);
    // walls[5] = createBoundry(DISPLAY_WIDTH,0,DISPLAY_WIDTH,DISPLAY_LENGTH);
    // walls[6] = createBoundry(0,DISPLAY_LENGTH,DISPLAY_WIDTH,DISPLAY_LENGTH);
    // walls[7] = createBoundry(0,0,0,DISPLAY_LENGTH);
    
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

        float scene[40];
    
        Particle particle = createParticle(mX,mY); //particle with the x/y pos being the mouse x/y
        //loop through every ray in the array
        for (int a = 0; a < 40; a++){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //draw the ray + dir
            float rpx = particle.rays[a].pos.x;
            float rpy = particle.rays[a].pos.y;
            float rdx = particle.rays[a].dir.x;
            float rdy = particle.rays[a].dir.y;

            Vector3 closest = {0,0,0}; //setup for working out the closest intercept
            float record = 999999999;
            
            //outer loops is for checking rays against each wall
            for(int j = 0; j < 8; j++){
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
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 110);
                //draw line from particle to contact point that is closest to the emitter
                SDL_RenderDrawLine(renderer,mX,mY,closest.x,closest.y);
            }
            scene[a] = record; //add the record to the scene array
            //we use the ray's position rpx and rpy as the starting point, and we add the ray's direction rdx and rdy to draw the line in the direction of the ray.
            SDL_RenderDrawLine(renderer, rpx, rpy, (rpx + rdx * 20), (rpy + rdy * 20));//with using the mouse x/y we no longer have to use transx/y
        }

        //render the 3d stuff here?
         
        SDL_RenderSetViewport(renderer, &rightViewport);
        float w = rightViewport.w / 40; //40 is the size of the array scene
        for(int i = 0; i < 40; i++){
            float sq = scene[i] * scene[i];
            float wSq = rightViewport.w * rightViewport.w;
            float b = map(sq,0,wSq,255,0);
            float h = map(scene[i], 0, rightViewport.w,rightViewport.h,0);
            SDL_FRect rectuu = ((i * w + w / 2), (rightViewport.h / 2), (w + 1), h);
            //drawFilledSquare(renderer, i * w + w / 2, rightViewport.h / 2, w + 1, h);
        }

        SDL_RenderSetViewport(renderer, &leftViewport);//might not work reseting the render back to the left here but we'll see
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
        "3D Raycasting ARM",
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

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Set the viewport for the first "port" (left side)
    //SDL_Rect leftViewport = { 0, 0, DISPLAY_WIDTH / 2, DISPLAY_LENGTH };
    //SDL_RenderSetViewport(renderer, &leftViewport);

    // Perform rendering for the left side here

    // // Set the viewport for the second "port" (right side)
    //SDL_Rect rightViewport = { DISPLAY_WIDTH / 2, 0, DISPLAY_WIDTH / 2, DISPLAY_LENGTH };
    // SDL_RenderSetViewport(renderer, &rightViewport);

    // Perform rendering for the right side here
}
