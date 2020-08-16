#include "../include/vector.h"
#include "../include/macro_util.h"

Vec2* v2NVP(float x,float y){
	Vec2* v = malloc(sizeof(Vec2));
	v->x = x;
	v->y = y;
	return v;
}

Vec2 v2NV(float x,float y){
	Vec2 v;
	v.x = x;
	v.y = y;
	return v;
}

void  v2IA(Vec2*a ,Vec2*b){
	a->x+=b->x;
	a->y+=b->y;
}

void  v2IAS(Vec2* a,Vec2* b, float t){
	a->x += b->x*t;
	a->y += b->y*t;
}

void  v2IASA(Vec2*a,Vec2* b, float t, Vec2* c){
	a->x+= b->x*t + c->x;
	a->y+= b->y*t + c->y;
}

void  v2IS(Vec2*a,float t){
	a->x*=t;
	a->y*=t;
}

void  v2A(Vec2* a, Vec2* b, Vec2* c){          /* a = b+c */
	a->x = b->x + c->x;
	a->y = b->y + c->y;
}

void v2S(Vec2* a,Vec2* b,float t){          /* a = b*t;*/
	a->x = b->x * t;
	a->y = b->y * t;
}

float v2Dot(Vec2* a, Vec2* b){                  /* a.b*/
	return a->x*b->x + a->y*b->y;
}

float v2Mag(Vec2* a){                           /* |a|*/
	return v2Dot(a,a);
}

void v2Norm(Vec2* a){                          /* a= 1/|a|*/
	v2IS(a,1.0f/v2Mag(a));
}

void  v2setVector(Vec2* a,float x, float y){
	a->x = x;
	a->y = y;
}

void  v2Su(Vec2*a, Vec2* b, Vec2* c){           /*a = b-c*/
	a->x = b->x - c->x;
	a->y = b->y - c->y;
}

float v2Cross(Vec2* a, Vec2* b){
	return ((a->x*b->y) - (a->y*b->x));
}

void vNPrint(float* vec,int size){
	int I;
	printf("<");
	for(I = 0; I < size-1;I++){
		printf("%f,",vec[I]);
	}
	printf("%f>\n",vec[size-1]);
}


Vec4  v4NV(float x, float y,float z,float w){
	Vec4 v ={x,y,z,w};
	return v;
}

inline void v4IA(Vec4*a , Vec4*b){
	a->x+=b->x;
	a->y+=b->y;
	a->z+=b->z;
	a->w+=b->w;
}

inline void v4IAS(Vec4* a, Vec4* b, float t){
	a->x+=b->x*t;
	a->y+=b->y*t;
	a->z+=b->z*t;
	a->w+=b->w*t;
}

inline void  v4IASA(Vec4*a, Vec4* b, float t, Vec4* c){
	a->x+=b->x*t + c->x;
	a->y+=b->y*t + c->y;
	a->z+=b->z*t + c->z;
	a->w+=b->w*t + c->w;
}

inline void v4ASA(Vec4 *a, Vec4*b,float t,Vec4 *c){
	a->x=b->x*t + c->x;
	a->y=b->y*t + c->y;
	a->z=b->z*t + c->z;
	a->w=b->w*t + c->w;
}

inline void  v4IS(Vec4*a, float t){
	a->x*=t;
	a->y*=t;
	a->z*=t;
	a->w*=t;
}

inline void  v4A(Vec4* a, Vec4* b, Vec4* c){
	a->x = b->x + c->x;
	a->y = b->y + c->y;
	a->z = b->z + c->z;
	a->w = b->w + c->w;
}

inline void  v4S(Vec4* a, Vec4* b, float t){
	a->x = b->x*t;
	a->y = b->y*t;
	a->z = b->z*t;
	a->w = b->w*t;
}

inline void  v4Su(Vec4*a, Vec4* b, Vec4* c){
	a->x = b->x - c->x;
	a->y = b->y - c->y;
	a->z = b->z - c->z;
	a->w = b->w - c->w;
}

float v4Dot(Vec4* a, Vec4* b){
	return a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;
}

float v4DotIW(Vec4 *a,Vec4 *b){
    return a->x*b->x + a->y*b->y + a->z*b->z;
}

void  v4Cross(Vec4* a, Vec4* b,Vec4* c){ /*does a  3d cross product obviously*/
	a->x = b->y*c->z-b->z*c->y;
	a->y = b->z*c->x-b->x*c->z;
	a->z = b->x*c->y-b->y*c->x;
    a->w = 0.0f;
}

float v4Mag(Vec4* a){
	return sqrtf(v4Dot(a,a));
}

void  v4Norm(Vec4* a){
	v4IS(a,1.0f/v4Mag(a));
}

void v4Clamp(Vec4 *a,float lbound,float ubound){
    a->x = CLAMP(a->x,lbound,ubound);
    a->y = CLAMP(a->y,lbound,ubound);
    a->z = CLAMP(a->z,lbound,ubound);
    a->w = CLAMP(a->w,lbound,ubound);
}

void  v4Print(Vec4 *a){
	printf("[%.2f,%.2f,%.2f,%.2f]",a->x,a->y,a->z,a->w);
}

