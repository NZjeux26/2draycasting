#include "3Dfunc.h"
#include <math.h>
#include "include/SDL2/SDL.h"

//3d vector
Vector3 createVector3(float x, float y, float z){
    Vector3 vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return vec;
}
//Function to create a ray with a given position (x, y, z) and a default direction (1.0, 0.0, 0.0).
Ray createRay(float x, float y, float z){
    Ray ray;
    ray.pos = createVector3(x,y,z);
    ray.dir = createVector3(1.0,0.0,0.0);
    return ray;
}
//Function to create a boundary with two points (x1, y1) and (x2, y2) in 2D space.
Boundry createBoundry(float x1, float y1, float x2, float y2){
    Boundry boundry;
    boundry.a = createVector3(x1,y1,0.0);
    boundry.b = createVector3(x2,y2, 0.0);
    return boundry;
}

Particle createParticle(float x, float y, Ray rays[]){
    Particle particle;
    particle.pos = createVector3(x,y,0);
    for (int i = 0; i < 360; i+=10){
        double radians = i * (2.0 * M_PI) / 360;
        particle.rays[i] = createRay(particle.pos.x,particle.pos.y, radians * i);
    }
    return particle;
}

//Function to set the direction of a ray to look at a specified point (x, y).
void lookAt(Ray* ray, float x, float y){
    ray->dir.x = x - ray->pos.x;
    ray->dir.y = y - ray->pos.y;

    float length = sqrt(ray->dir.x * ray->dir.x + ray->dir.y * ray->dir.y);
    //printf("Length %f\n",length);
    if(length > 0){
        ray->dir.x /= length;
        ray->dir.y /= length;
    }
}

//Function to check for intersection between a ray and a boundary, returning the intersection point.
//Returns 1 if there is an intersection and updates interX and interY, otherwise returns 0.
int cast(Ray ray, Boundry wall, float* interX, float* interY){
    float x1 = wall.a.x;
    float y1 = wall.a.y;
    float x2 = wall.b.x;
    float y2 = wall.b.y;

    float x3 = ray.pos.x;
    float y3 = ray.pos.y;
    float x4 = ray.pos.x + ray.dir.x;
    float y4 = ray.pos.y + ray.dir.y;

    //the denomator for working out t and u :https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    //if the den is 0 that means the ray and line(s) are paraellel and do not intersect. 
    if(den == 0.0){
        return 0;
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    float u = -((x1-x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
    if(t > 0.0 && t < 1.0 && u> 0.0){
        *interX = x1 + t * (x2 - x1);
        *interY = y1 + t * (y2 - y1);
        return 1;
    }
    return 0;
}
//stright copied from GPT, needed a function to draw a square at a given point to show where the intersections occur.
void drawFilledSquare(SDL_Renderer* renderer, float x, float y, int size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Calculate the coordinates of the square's corners
    int x1 = x - size / 2;
    int y1 = y - size / 2;

    // Draw a filled square using SDL_RenderFillRect
    SDL_Rect squareRect = {x1, y1, size, size};
    SDL_RenderFillRect(renderer, &squareRect);
}
//matrix multiplication 
void matmul(float matrix[3][3], Vector3 *vector) {
   float x = vector->x;
   float y = vector->y;
   float z = vector->z;

   vector->x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z;
   vector->y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z;
   vector->z = matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z;
}

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
    // Ensure that the value is within the input range
    value = (value < fromLow) ? fromLow : value;
    value = (value > fromHigh) ? fromHigh : value;

    // Calculate the mapped value
    float mappedValue = ((value - fromLow) / (fromHigh - fromLow)) * (toHigh - toLow) + toLow;

    return mappedValue;
}

void projectPoint(Vector3* point3D, float distance){
    float z = 1.0 / (distance / point3D->z);
    point3D->x *= z;
    point3D->y *= z;
}

void translate(SDL_Renderer* renderer, float x, float y){
    SDL_RenderSetClipRect(renderer, &(SDL_Rect){x, y, 0, 0});
}
//takes in a 3d vector along with the total points wanted and the desired radius.
void createSphere(int total, Vector3 globe[total][total], float r){
          for (int i = 0; i < total + 1; i++){
            float lat = map(i, 0,total, 0, M_PI);
            for (int j=0; j < total + 1; j++){
                float lon = map(j,0,total, 0, M_PI * 2);
                float x = r * sin(lat) * cos(lon);
                float y = r * sin(lat) * sin(lon);
                float z = r * cos(lat);
                globe[i][j] = createVector3(x,y,z);
            }
    }
}