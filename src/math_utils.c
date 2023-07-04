#include "math_utils.h"

float vector2D_distance(Vector2D p1, Vector2D p2)
{
    return sqrtf(powf(p2.y - p1.y, 2) + powf(p2.x - p1.x, 2));
}

float random(float min, float max)
{
    return (((float)rand() / RAND_MAX) * (max - min + 1.0f) + min);
}

