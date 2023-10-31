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

Ray createRay(float x, float y){
    Ray ray;
    ray.pos = createVector3(x,y,0.0);
    ray.dir = createVector3(1.0,0.0,0.0);
    return ray;
}

Boundry createBoundry(float x1, float y1, float x2, float y2){
    Boundry boundry;
    boundry.a = createVector3(x1,y1,0.0);
    boundry.b = createVector3(x2,y2, 0.0);
    return boundry;
}

int cast(Ray ray, Boundry wall, float* interX, float* y interY){
    float x1 = wall.a.x;
    float y1 = wall.a.y;
    float x2 = wall.b.x;
    float y2 = wall.b.y;

    float x3 = ray.pos.x;
    float y3 = ray.pos.y;
    float x3 = ray.pos.x + ray.dir.x;
    float y4 = ray.pos.y + ray.dir.y
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