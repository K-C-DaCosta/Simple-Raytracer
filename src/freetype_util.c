#include "../include/freetype_util.h"
#include "../include/macro_util.h"

int ftuLoadFont(FT_Library *L, FT_Face *F,SDL_Window *win){
	uint32_t error = FT_New_Face(*L,"./bin/GotischeMissalschrift.ttf",0,F);
	if ( error == FT_Err_Unknown_File_Format ){
	  printf( "Format is unsupported\n");
	}else if(error){
		printf("Error loading font\n");
	}
	int window_width,window_height;
	SDL_GetWindowSize(win,&window_width,&window_height);

	error = FT_Set_Char_Size(
          *F,
          0*64,
          10*64,
          window_width,
          window_height);

	if(error){
		printf("Error setting font size\n");
		return 1;
	}
	return 0;
}

int ftuSetupFreeType(FT_Library *L){
	int error = FT_Init_FreeType(L);
	if ( error ){
		printf("Freetype failed to initalize\n");
		return 1;
	}
	return 0;
}

void ftuCopyFTbitmap( FT_Bitmap* bitmap,SDL_Window *win,uint32_t *buffer,int x,int y){
	uint8_t *ftbuffer = bitmap->buffer;
	uint32_t *frameBuffer = NULL;
	int h = bitmap->rows;
	int w = bitmap->width;
	int p = bitmap->pitch;
	int I,J,offset,ebv,window_width;

	SDL_GetWindowSize(win,&window_width,NULL);

	for(I = 0; I < h; I++){
		offset = GI(x,y+I,window_width);
		frameBuffer = buffer+offset;
		for(J = 0; J < w; J++){
			ebv = ftbuffer[J];
			frameBuffer[J] = ebv << 16 | ebv << 8 | ebv;
		}
		ftbuffer+=p;
	}
}
void ftuRenderString(FT_Face F,SDL_Window *win,uint32_t *buff,char *text,int pen_x,int pen_y){
	FT_GlyphSlot slot = F->glyph;
	int n,error,num_chars;
	num_chars = strlen(text);

	for ( n = 0; n < num_chars; n++ ){
		FT_UInt  glyph_index;

		// retrieve glyph index from character code
		glyph_index = FT_Get_Char_Index(F, text[n] );

		// load glyph image into the slot (erase previous one)
		error = FT_Load_Glyph(F, glyph_index, FT_LOAD_DEFAULT );
		if ( error )
		continue;  // ignore errors

		// convert to an anti-aliased bitmap
		error = FT_Render_Glyph(F->glyph, FT_RENDER_MODE_NORMAL );
		if ( error )
		continue;

		// now, draw to our target surface
		ftuCopyFTbitmap( &slot->bitmap,win,buff,pen_x+slot->bitmap_left,pen_y-slot->bitmap_top);
		// increment pen position
		pen_x += slot->advance.x >> 6;
		pen_y += slot->advance.y >> 6; // not useful for now
	}
}
