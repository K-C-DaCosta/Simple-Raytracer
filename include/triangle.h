#include <stdbool.h>
#include "../include/object.h"
#include "../include/vector.h"
#include "../include/image.h"


#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

typedef struct FaceData_{
    Vec4 normal;
    float _a;
    float _bc;
    float _d;
    float invDet;
    Vec4 v1;
    Vec4 v2;
}FaceData;

typedef struct Triangle_{
    Object header;
    FaceData fd;
    Vec4 points[3];
    Vec2  tOrds[3];
    Image *texture;
}Triangle;

float triangleFastRayIntersectionTest(Vec4 *ray,Vec4 *e,Triangle *tri);
float triangleRayIntersectionTest(Vec4 *ray,Vec4 *e,Triangle *tri);
void  triangleGetNormal(Vec4* point, Vec4 *norm,Triangle *tri);
void  triangleGetAmbientColor(Vec4 *point,Vec4* color,Triangle *tri);
bool  triangleComputeFaceData(Triangle *tri);

#endif
