#include "../include/vector.h"
#include "../include/object.h"
#include "../include/image.h"

#ifndef SPHERE_H_
#define SPHERE_H_
    typedef struct Sphere_{
        Object header;
        Vec4 centroid;
        Vec4 ambientColor; //only used if texture == NULL
        float radius;
        Image *texture;
    }Sphere;
    float sphereRayIntersectionTest(Vec4 *ray,Vec4 *e,Sphere *s);
    void  sphereGetNormal(Vec4 *point,Vec4 *normal,Sphere * s);
    void  sphereGetAmbientColor(Vec4 *point,Vec4* color, Sphere *s);
#endif
