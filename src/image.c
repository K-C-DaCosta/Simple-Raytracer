#include "../include/image.h"
#include <IL/il.h>
#include "../include/macro_util.h"
#include "../include/basic_math_util.h"



Image* imgNewD(void *data){
    return NULL;//not implemented yet
}

Image* imgNewP(char* path){
    ILuint imageID;
    int iWidth,iHeight;
    uint32_t* imageBuffer;
    Image* imgPtr;

    ilGenImages(1,&imageID);
    ilBindImage(imageID);

    if(ilLoadImage(path) == IL_FALSE){
       fprintf(stderr,"ERROR:%s failed to load!\n",path);
       return NULL;
    }

    iWidth  = ilGetInteger(IL_IMAGE_WIDTH);
    iHeight = ilGetInteger(IL_IMAGE_HEIGHT);
    imageBuffer = malloc(sizeof(uint32_t)*iWidth*iHeight);
    ilCopyPixels(0,0,0,iWidth,iHeight,1,IL_BGRA,IL_UNSIGNED_BYTE,imageBuffer);

    imgPtr = malloc(sizeof(Image));
    imgPtr->width = iWidth;
    imgPtr->height = iHeight;
    imgPtr->imageBuffer = imageBuffer;

    ilDeleteImage(imageID);
    return imgPtr;
}

void imgRender(Image *imgPtr, int x, int y, int w, int h, uint32_t *buffer, int bw){
    int I,J;
    float sx = 15.3f,
          sy = 15.0f,
          iw = imgPtr->width,
          ih = imgPtr->height;

    ih*=sy;
    iw*=sx;
    for(J = 0; J < ih; J++){
        for(I = 0; I < iw; I++){
            buffer[GIint(I+x,J+y,bw)] = imgSampleLinear(imgPtr,I/sx,J/sy);
        }
    }
}

uint32_t imgSampleNearest(Image *imgPtr,float x, float y){
    return imgPtr->imageBuffer[GIint(x,y,imgPtr->width)];
}

uint32_t lerpChannel(uint32_t *b,int w,int h,int x0, int y0, float u, float v, int mask, int shift){
    int hor = x0 >= w-1;
    int ver = y0 >= h-1;

    float a0 =                _GC(b,x0+0,y0+0,w,mask,shift);
    float a1 = (hor     )? 0: _GC(b,x0+1,y0+0,w,mask,shift);
    float b0 = (ver     )? 0: _GC(b,x0+0,y0+1,w,mask,shift);
    float b1 = (hor||ver)? 0: _GC(b,x0+1,y0+1,w,mask,shift);

    return bmuLerp(bmuLerp(a0,a1,u),bmuLerp(b0,b1,u),v);
}

uint32_t imgSampleLinear(Image *imgPtr,float x, float y){
    float u = (x - (int)x),
          v = (y - (int)y);

    int x0  = x,
        y0  = y,
         w  = imgPtr->width,
         h  = imgPtr->height,
         ac,rc,gc,bc;

    uint32_t *b  = imgPtr->imageBuffer;

    ac = lerpChannel(b,w,h,x0,y0,u,v,0xFF000000,24);
    rc = lerpChannel(b,w,h,x0,y0,u,v,0x00FF0000,16);
    gc = lerpChannel(b,w,h,x0,y0,u,v,0x0000FF00,8);
    bc = lerpChannel(b,w,h,x0,y0,u,v,0x000000FF,0);

    return ac << 24 | rc << 16 | gc << 8 | bc;
}

uint32_t imgSampleTexture(Image *imgPtr,Vec2*texCoord){
    float x = imgPtr->width-1,
          y = imgPtr->height-1,
          u,v;
     int w = imgPtr->width,
         h = imgPtr->height,
         x0,y0,ac,rc,gc,bc;

     uint32_t *b  = imgPtr->imageBuffer;

     x*=texCoord->x;
     y*=texCoord->y;
     x0 = x;
     y0 = y;
     u = x-x0;
     v = y-y0;

     ac = lerpChannel(b,w,h,x0,y0,u,v,0xFF000000,24);
     rc = lerpChannel(b,w,h,x0,y0,u,v,0x00FF0000,16);
     gc = lerpChannel(b,w,h,x0,y0,u,v,0x0000FF00,8);
     bc = lerpChannel(b,w,h,x0,y0,u,v,0x000000FF,0);

     return ac << 24 | rc << 16 | gc << 8 | bc;//imgSampleNearest(imgPtr,x0,y0);
}

uint32_t imgSampleCubic(Image *imgPtr,float x,float y){
    return 0; //not yet implemented 
}

void imgFree(Image *imgPtr){
    free(imgPtr->imageBuffer);
    free(imgPtr);
}



