#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>

#define M_PI 3.14159265358979323846

typedef struct
{
  float x;
  float y;
} Vector2D;

float vector2D_distance(Vector2D p1, Vector2D p2);

#endif

