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

typedef struct Ray
{
    Vector3 pos;
    Vector3 dir;
} Ray;

int cast(Ray ray, Boundry wall, float* interX, float* y interY);
void translate(SDL_Renderer* renderer, float x, float y);
Ray createRay(float x, float y);
Boundry createBoundry(float x1, float y1, float x2, float y2);
Vector3 createVector3(float x, float y, float z);
void matmul(float matrix[3][3], Vector3 *vector);
float map(float value, float fromLow, float fromHigh, float toLow, float toHigh);
void projectPoint(Vector3* point3D, float distance);
void createSphere(const int size, Vector3 globe[size][size], const float r);
//create create rectangle, square and triangle (3d)
#endif
