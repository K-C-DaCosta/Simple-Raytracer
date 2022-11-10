#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector.h>


typedef struct MAT4_4_{
	float data[16];
}MAT44;

void mat44Add(MAT44 *a,MAT44 *b,MAT44 *c);
void mat44Scale(MAT44 *a,float s);
void mat44Mul(MAT44 *a, MAT44 *b, MAT44 *c);
void mat44SetIdentity(MAT44 *a);
void mat44SetZero(MAT44 *a);
void mat44Print(MAT44 *a);
Vec4 mat44Transform(MAT44 *a, Vec4 *v); //basically just multiplacation but matrix vs column matrix
void mat44PackVertical(MAT44 *m,Vec4 *a, Vec4 *b, Vec4 *c,Vec4 *d);
void mat44SetRotationOnAxis(MAT44 *m,Vec4 *u,float angle);
void mat44SetXRotation(MAT44 *m,float angle);
void mat44SetYRotation(MAT44 *m,float angle);

#endif
