#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <xmmintrin.h>

#ifndef VECTOR_H
#define VECTOR_H

#define _DOTPP(A,B)((A->x*B->x + A->y*B->y + A->z*B->z))
#define _DOTVV(A,B)((A.x*B.x + A.y*B.y + A.z*B.z))

typedef struct Vec2_{
	float x;
	float y;
}Vec2;

typedef struct Vec4_{
	float x;
	float y;
	float z;
	float w;
} Vec4  __attribute__((aligned(16)));//asdas

typedef struct VecN_{
	float * vec;
	uint8_t len;
}VecN;

/*
   Notation:
   "I"   increment or (does operations and sets to self) or "itself"
   "A"   add
   "S"   scale
   "Su"  subtract
   "NV"  NewVector (byvalue)
   "NVP" NewVector (byreference/pointer)

   All functions are in the form :
   A ('=' or '+=') [some stuff usually with B C sometimes t]
*/

Vec2* v2NVP(float x, float y);
Vec2  v2NV(float x, float y);
void  v2IA(Vec2*a , Vec2*b);
void  v2IAS(Vec2* a, Vec2* b, float t);
void  v2IASA(Vec2*a, Vec2* b, float t, Vec2* c);
void  v2IS(Vec2*a, float t);
void  v2A(Vec2* a, Vec2* b, Vec2* c);
void  v2S(Vec2* a, Vec2* b, float t);
void  v2Su(Vec2*a, Vec2* b, Vec2* c);
float v2Dot(Vec2* a, Vec2* b);
float v2Cross(Vec2* a, Vec2* b);
float v2Mag(Vec2* a);
void  v2Norm(Vec2* a);
void  v2setVector(Vec2* a,float x, float y);


Vec4  v4NV(float x, float y,float z,float w);

 void  v4IA(Vec4*a , Vec4*b);
void  v4IAS(Vec4* a, Vec4* b, float t);
void  v4IASA(Vec4*a, Vec4* b, float t, Vec4* c);
void  v4ASA(Vec4 *a, Vec4*b,float t,Vec4 *c);
void  v4IS(Vec4*a, float t);
void  v4A(Vec4* a, Vec4* b, Vec4* c);
void  v4S(Vec4* a, Vec4* b, float t);
void  v4Su(Vec4*a, Vec4* b, Vec4* c);
float v4Dot(Vec4* a, Vec4* b);
float v4DotIW(Vec4 *a,Vec4 *b); //'IW' == ignore W component
void  v4Cross(Vec4* a, Vec4* b,Vec4* c);
float v4Mag(Vec4* a);
void  v4Norm(Vec4* a);
void  v4setVector(Vec4* a,float x, float y);
void  v4Print(Vec4 *a);
void  v4Clamp(Vec4 *a, float lbound,float ubound);
void  vecNPrint(float *vec, int size);
//asdasd
#endif
