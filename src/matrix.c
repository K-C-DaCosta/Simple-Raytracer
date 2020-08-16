#include "../include/matrix.h"
#include "../include/macro_util.h"
#include <stdio.h>
#include <string.h>


#ifndef _MATPACK
#define _MATPACK(A,V) if(V){A[0]=V->x;A[1]=V->y;A[2]=V->z;A[3]=V->w;}A+=4;
#endif

void mat44SetZero(MAT44 *a){
	memset(a->data,0,sizeof(float)*16);
}

void mat44SetIdentity(MAT44 *a){
	int I = 0;
	for(I = 0; I < 16; I++){
		a->data[I]=(I%5==0)?1:0;
	}
}

void mat44Add(MAT44 *a,MAT44 *b,MAT44 *c){
	int I;
	for(I = 0; I < 16; I++){
		a->data[I] = b->data[I] + c->data[I];
	}
}

void mat44Scale(MAT44 *a,float s){
	int I;
	for(I = 0; I < 16; I++){
		a->data[I]*=s;
	}
}

void mat44Mul(MAT44 *a, MAT44 *b, MAT44 *c){
	int x,y,k;
	float *da = b->data, *db = c->data, *dc = a->data;
	for(y = 0; y < 4; y++){
		for(x = 0; x < 4; x++){
			for(k = 0; k < 4; k++){
				dc[GI(x,y,4)]+= da[GI(k,y,4)]*db[GI(x,k,4)];
			}
		}
	}
}

Vec4 mat44Transform(MAT44 *a, Vec4 *v){
	int x,y,k;
	float *da = a->data, *db = (float*)v;
	float dc[4] = {0,0,0,0};
	for(y = 0; y < 4; y++){
		for(x = 0; x < 1; x++){
			for(k = 0; k < 4; k++){
				dc[GI(x,y,1)]+= da[GI(k,y,4)]*db[GI(x,k,1)];
			}
		}
	}
	return *((Vec4*)dc);
}

void mat44SetRotationOnAxis(MAT44 *m,Vec4 *u,float angle){
	float cos0 = cosf(angle);
	float sin0 = sinf(angle);
	float _1_cos0 = 1.0f-cos0;
	//rotation matrix
	float *d = m->data;
 	d[0] = cos0+(u->x*u->x)*_1_cos0   ; d[1] = u->x*u->y*_1_cos0-u->z*sin0  ; d[2] = u->x*u->z*_1_cos0+u->y*sin0 ; d[3] = 0.0f;
	d[4] = u->y*u->x*_1_cos0+u->z*sin0; d[5] = cos0+(u->y*u->y)*_1_cos0     ; d[6] = u->y*u->z*_1_cos0-u->x*sin0 ; d[7] = 0.0f;
	d[8] = u->z*u->x*_1_cos0-u->y*sin0; d[9] = u->z*u->y*_1_cos0 + u->x*sin0; d[10]= cos0 + u->z*u->z*_1_cos0    ; d[11]= 0.0f;
	d[12]=0.0f                        ; d[13]=0.0f                          ; d[14]=0.0f                         ; d[15]= 1.0f;
}

void mat44SetXRotation(MAT44 *m,float angle){
	float cos0 = cosf(angle);
	float sin0 = sinf(angle);
	//rotation matrix
	float *d = m->data;
 	d[0] = 1.0f; d[1]  = 0.0f ; d[2] = 0.0f  ; d[3] = 0.0f;
	d[4] = 0.0f; d[5]  = cos0 ; d[6] = -sin0 ; d[7] = 0.0f;
	d[8] = 0.0f; d[9]  = sin0 ; d[10]= cos0  ; d[11]= 0.0f;
	d[12]= 0.0f; d[13] = 0.0f ; d[14]= 0.0f  ; d[15]= 1.0f;
}

void mat44SetYRotation(MAT44 *m,float angle){
	float cos0 = cosf(angle);
	float sin0 = sinf(angle);
	//rotation matrix
	float *d = m->data;
 	d[0] = cos0  ; d[1]  = 0.0f ; d[2] = sin0  ; d[3] = 0.0f;
	d[4] = 0.0f  ; d[5]  = 1.0f ; d[6] = 0.0f  ; d[7] = 0.0f;
	d[8] = -sin0 ; d[9]  = 0.0f ; d[10]= cos0  ; d[11]= 0.0f;
	d[12]= 0.0f  ; d[13] = 0.0f ; d[14]= 0.0f  ; d[15]= 1.0f;
}

void mat44Print(MAT44 *a){
	int x,y;
	for(y = 0; y < 4; y++){
		printf("[");
		for(x = 0; x < 4; x++){
			printf((x==0)?"%7.2f":",%7.2f",a->data[GI(x,y,4)]);
		}
		printf("]\n");
	}
}

void mat44PackVertical(MAT44 *m,Vec4 *a, Vec4 *b, Vec4 *c,Vec4 *d){
	float *data = m->data;
	_MATPACK(data,a);
	_MATPACK(data,b);
	_MATPACK(data,c);
	_MATPACK(data,d);
}
