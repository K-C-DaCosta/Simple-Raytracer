#include <sphere.h>
#include <macro_util.h>
#include <basic_math_util.h>

float sphereRayIntersectionTest(Vec4 *ray,Vec4 *e,Sphere *sphere){
    Vec4  ec,
         *c = &sphere->centroid;
    float rad = sphere->radius,
          solTable[2] = {0.0f,0.0f};
    v4Su(&ec,e,c);
    bmuSolveQuadratic(v4DotIW(ray,ray),2.0f*v4DotIW(&ec,ray),v4DotIW(&ec,&ec)-rad*rad,solTable);
    return MIN(solTable[0],solTable[1]);
}

void  sphereGetNormal(Vec4 *point,Vec4 *normal,Sphere * s){
    v4Su(normal,point,&s->centroid);
}

void sphereGetAmbientColor(Vec4 *point, Vec4 *color, Sphere *s){
   *color = s->ambientColor;
}
