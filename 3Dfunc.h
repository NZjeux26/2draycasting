#ifndef _3DFUNC_H_
#define _3DFUNC_H_

#include "include/SDL2/SDL.h"

typedef struct{
    float x;
    float y;
    float z;
} Vector3;

typedef struct{
    Vector3 a;
    Vector3 b;
} Boundry;

typedef struct Ray{
    Vector3 pos;
    Vector3 dir;
} Ray;

typedef struct Particle{
    Vector3 pos;
    Ray rays[360];//temp value repalce with vector later 1 ray per * of a circle.
} Particle;

int cast(Ray ray, Boundry wall, float* interX, float* interY);
void lookAt(Ray* ray, float x, float y);
void translate(SDL_Renderer* renderer, float x, float y);
void drawFilledSquare(SDL_Renderer* renderer, float x, float y, int size, SDL_Color color);
Ray createRay(float x, float y, float z);
Boundry createBoundry(float x1, float y1, float x2, float y2);
Vector3 createVector3(float x, float y, float z);
Particle createParticle(float x, float y, Ray rays[]);
void matmul(float matrix[3][3], Vector3 *vector);
float map(float value, float fromLow, float fromHigh, float toLow, float toHigh);
void projectPoint(Vector3* point3D, float distance);
void createSphere(const int size, Vector3 globe[size][size], const float r);
//create create rectangle, square and triangle (3d)
#endif
