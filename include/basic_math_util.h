#include <math.h>
#include "../include/vector.h"
#include "../include/matrix.h"

#ifndef _BASIC_MATH_UTIL_H_
#define _BASIC_MATH_UTIL_H_
    typedef struct CameraBasis_{
        Vec4 E;
        Vec4 P;
        Vec4 Q;
        Vec4 R;
        Vec4 UP;
        float depth;
    }CameraBasis;

    typedef struct ScreenBounds_{
        float top;
        float bottom;
        float left;
        float right;
        float invWidth;
        float invHeight;
    }ScreenBounds;

    float bmuLerp(float a,float b,float t);//ased
    float bmuBilerp(float*f,int fw,int fh, float x,float y);
    void bmuSolveQuadratic(float A,float B, float C,float t[2]);
    void bmuGenerateRay(Vec4* ray,int sx, int sy, ScreenBounds *sb,CameraBasis *cb);
    void bmuPixelSpaceToWorld(Vec4 * v, int sx,int sy, ScreenBounds *sb);
#endif
