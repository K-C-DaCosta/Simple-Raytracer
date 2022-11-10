#include <basic_math_util.h>
#include <macro_util.h>

void bmuSolveQuadratic(float A,float B, float C,float t[2]){
	const float THRESHOLD  = 0.01f;
	float det = B*B -4.0f*A*C;
	float denom = 2.0f*A;
	if(det < 0.0f || fabsf(denom) < THRESHOLD){
		t[0] = t[1] = INFINITY;
		return;
	}
	B*=-1.0f;
	det = sqrtf(det);
	denom = 1.0f/denom;
	t[0] = (B+det)*denom;
	t[1] = (B-det)*denom;
}

float bmuLerp(float a,float b,float t){
	return (b-a)*t + a;
}

float bmuBilerp(float*f,int fw,int fh, float x,float y){
	int ix  = x;
	int iy  = y;
	if(ix==fw-1)ix--;
	if(iy==fh-1)iy--;
	float t = (x - (float)ix);
	float s = (y - (float)iy);
	return bmuLerp(bmuLerp(f[GI(ix,iy,fw)],f[GI(ix+1,iy,fw)],t),bmuLerp(f[GI(ix,iy+1,fw)],f[GI(ix+1,iy+1,fw)],t),s);
}

void bmuPixelSpaceToWorld(Vec4 * v, int sx,int sy, ScreenBounds *sb){
    float fsx = (float)sx + 0.5f,
          fsy = (float)sy + 0.5f;
	v->x = sb->left + (sb->right-sb->left)*fsx*sb->invWidth;
	v->y = sb->top  + (sb->bottom-sb->top)*fsy*sb->invHeight;
	//v->z = 0.0f;
	v->w = 0.0f;
}
void bmuGenerateRay(Vec4* ray,int sx, int sy, ScreenBounds *sb,CameraBasis *cb){
	float fsx = (float)sx + 0.5f,
	 	  fsy = (float)sy + 0.5f,
		  u = sb->left + (sb->right-sb->left)*fsx*sb->invWidth,
		  v = sb->top  + (sb->bottom-sb->top)*fsy*sb->invHeight;
		  // ray = -depth*R + u*P + v*Q
		  v4S(ray,&cb->P,u);
		  v4IAS(ray,&cb->Q,v);
		  v4IAS(ray,&cb->R,cb->depth*-1.0f);
}
