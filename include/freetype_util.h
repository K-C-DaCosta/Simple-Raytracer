#include <SDL2/SDL.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

#ifndef _FREETYPE_UTIL_H_
#define _FREETYPE_UTIL_H_

int ftuLoadFont(FT_Library *L, FT_Face *F,SDL_Window *win);
int ftuSetupFreeType(FT_Library *L);
void ftuCopyFTbitmap( FT_Bitmap* bitmap,SDL_Window *win,uint32_t *buffer,int x,int y);
void ftuRenderString(FT_Face F,SDL_Window *win,uint32_t *buff,char *text,int pen_x,int pen_y);

#endif
