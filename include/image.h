#include <stdint.h>
#include <SDL2/SDL.h>
#include "../include/vector.h"

#ifndef  IMAGE_H_
#define  IMAGE_H_

typedef struct Image_{
    int height;
    int width;
    uint32_t *imageBuffer;
}Image;

Image*   imgNewP(char* path);
Image*   imgNewD(void* data);
void     imgRender(Image *imgPtr,int x, int y, int w, int h, uint32_t *buffer,int bw);
uint32_t imgSampleNearest(Image *imgPtr,float x, float y);
uint32_t imgSampleLinear(Image *imgPtr,float x, float y);
uint32_t imgSampleCubic(Image *imgPtr,float x,float y);
uint32_t imgSampleTexture(Image *imgPtr,Vec2*texCoord);
void     imgFree(Image* imgPtr);
void     imgPartialFree(Image *imgPtr);

#endif
