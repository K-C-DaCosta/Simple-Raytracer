#include <raster_util.h>
#include <macro_util.h>

//rasterizes a line in a w by h buffer
void ruRasterLine(uint32_t *buffer,int w,int h,Vec2 va,Vec2 vb){
	const float TOLERANCE = 0.5f;/*detects vertical lines*/
	int I,J;
	float min,max;/*line bounding box*/
	float rise,run,m,b,y; /*slope intercept constants*/

	rise = vb.y - va.y;
	run  = vb.x - va.x;
	if(fabs(run) < TOLERANCE){
		I = MIN(va.y,vb.y);
		J = MAX(va.y,vb.y);
		while(I < J){
			buffer[GI((int)va.x,I,w)] = 0x00ff0000;
			I++;
		}
		return;
	}
	m = rise/run;
	b = va.y - m*va.x;

	if( fabsf(run) > fabsf(rise) ){
		min = MAX(0,MIN(w,MIN(va.x,vb.x)));
		max = MAX(0,MIN(w,MAX(va.x,vb.x)));
		for(I = min; I < max;I++){
			y = m*(float)I + b;
			buffer[GI(I,(int)y,w)] = 0x00ff0000;
		}
	}else{
		min = MAX(0,MIN(w,MIN(va.y,vb.y)));
		max = MAX(0,MIN(w,MAX(va.y,vb.y)));
		for(I = min;I < max;I++){
			y = (float)I/m -b/m;
			buffer[GI((int)y,I,w)] = 0x00ff0000;
		}
	}
}
