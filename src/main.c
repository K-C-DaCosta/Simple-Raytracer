#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <freetype2/ft2build.h>
#include <string.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <IL/il.h>
#include <IL/ilu.h>

#include "../include/basic_math_util.h"
#include "../include/object.h"
#include "../include/matrix.h"
#include "../include/vector.h"
#include "../include/macro_util.h"
#include "../include/triangle.h"
#include "../include/sphere.h"
#include "../include/simple_array_list.h"

#include FT_FREETYPE_H

/*  NOTE TO SELF:
    This whole program needs to be rewritten in c++ for clearity.
    I really need operator overloading.
*/

int tempCount = 0;
Vec4 lightPointList[] = {{-1,0,0,1} ,{0,0,0,1 },{1,0,0,1 },
                         {-1,0,-1,1},{0,0,-1,1},{1,0,-1,1},
                         {-1,0,1,1} ,{0,0,1,1 },{1,0,1,1}
                        };



int HandleEvents(SDL_Event *event,CameraBasis *cb);

/*GLOBAL VARIABLES*/
int32_t done = 0;
pthread_t thread[4];
sem_t sem_thread[4],sem_main;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
SDL_Joystick *gGameController = NULL;
bool WASD[4] = {false,false,false,false};
float AVG = 0;
float AVG_count = 0;

//light infomation

const float LIGHT_RADIUS = 30.0f;
const int MAX_SOFT_ITERS = 16;
const float INV_MAX_SOFT_ITERS = 1.0f/16.0f;


typedef struct TRACEINFOBODY_{
	int ww;
	int wh;
	ScreenBounds *sb;
	CameraBasis  *cb;
    SimpleArrayList *objectList;
	uint32_t* buffer;
	Vec4 *lightPos;
}TraceInfoBody;

typedef struct TRACEINFO_{
	int tIndex;
	int x0;
	int y0;
	TraceInfoBody tib;
}TraceInfo;


void rayTrace(TraceInfo *ti){
    Vec4 lightPos,point,normal,lightDisp,look,halfVec,ray,ambient,accum;
    int x,y,ww,wh,ybound,xbound;
    SimpleArrayList *objectList;
    Object *minObject;
	CameraBasis *cb;
	ScreenBounds *sb;
	uint32_t *buffer, *buff_offset,pixel_value;
    float tmin,diffuse,specular,lightIntensity;

	ww = ti->tib.ww;
	wh = ti->tib.wh;
	buffer = ti->tib.buffer;
	objectList = ti->tib.objectList;
	cb = ti->tib.cb;
	sb = ti->tib.sb;
	lightPos = *ti->tib.lightPos;
	ybound 	= wh/2 + ti->y0;
	xbound  = ww/2 + ti->x0;


    for(y = ti->y0; y < ybound; y+=1){
        for(x = ti->x0; x < xbound; x+=2){
			//ray generation
			bmuGenerateRay(&ray,x,y,sb,cb);

            minObject = NULL;
            tmin = 0.0f;
            lightIntensity = 0.9f;//(I will use this for softshadows in the future)

            if( (minObject = objectGetNearest(objectList,&ray,&cb->E,&tmin)) != NULL ){
				v4ASA(&point,&ray,tmin,&cb->E);//point = ray*tmin + e
                objectGetNormal(&point,&normal,minObject);//compute normal at point
				v4Su(&lightDisp,&lightPos,&point);//lightDisp = lightpos-point

                objectGetAmbientColor(&point,&ambient,minObject);//initialize ambient color
                accum = ambient;//initalize accumulator

                //Buggy soft shadow code here
                int I;
                float count =0;
                lightIntensity = 0.0f;
                Vec4 tempDisp,lp;
                for(I = 0; I < 1; I++){
                    lp = lightPointList[I];
                    lp.x+=lightPos.x*0.9;
                    lp.y+=lightPos.y*0.9;
                    lp.z+=lightPos.z;
                    lp.w =1.0f;
                    v4Su(&tempDisp,&lp,&point);
                    if(objectIsBlockingLight(objectList,&tempDisp,&point) == NULL  ){
                        lightIntensity = 0.9; 
                    }
                }
                //lightIntensity = count*(1.0f/9.0f);
		//lightIntensity=1.0;


                v4Su(&look,&cb->E,&point);//look = E-point
                v4A(&halfVec,&lightDisp,&look);//halfVec = look+lightDisp
                v4Norm(&normal);
                v4Norm(&lightDisp);
                v4Norm(&halfVec);
                //compute diffuse and specular components
                diffuse = MAX(0.0f,v4DotIW(&normal,&lightDisp))*lightIntensity;
                specular = powf(MAX(v4DotIW(&normal,&halfVec),0),32.0f)*lightIntensity;
                //compute accum+= ambient*diffuse + ambient*specular
                v4IAS(&accum,&ambient,diffuse);
                v4IAS(&accum,&ambient,specular);


//                if( objectIsBlockingLight(objectList,&lightDisp,&point) == NULL){
//                    v4Su(&look,&cb->E,&point);//look = E-point
//                    v4A(&halfVec,&lightDisp,&look);//halfVec = look+lightDisp
//                    v4Norm(&normal);
//                    v4Norm(&lightDisp);
//                    v4Norm(&halfVec);
//                    //compute diffuse and specular components
//                    diffuse = MAX(0.0f,v4DotIW(&normal,&lightDisp));
//                    specular = powf(MAX(v4DotIW(&normal,&halfVec),0),32.0f)*lightIntensity;
//                    //compute accum+= ambient*diffuse + ambient*specular
//                    v4IAS(&accum,&ambient,diffuse);
//                    v4IAS(&accum,&ambient,specular);
//                }

                //Map the color from domain [0,1] to domain [0,255]
                v4IS(&accum,255.0f);
                v4Clamp(&accum,0,255.0f);

                pixel_value = (uint)accum.w << 24 | (uint)accum.x << 16 | (uint)accum.y << 8 | (uint)accum.z;

				buff_offset = buffer + GI(x,y,ww);
                *buff_offset = pixel_value;
                buff_offset++;
                *buff_offset = pixel_value;

//               buff_offset = buffer + GI(x,y+1,ww);
//               *buff_offset = pixel_value;
//               buff_offset++;
//               *buff_offset = pixel_value;
            }
		}
	}
}

void* tracer_start_routine(void *traceInfo){
	TraceInfo * ti  = traceInfo;
	int sem_index = ti->tIndex;

	while(true){
		rayTrace(ti);//trace section of screen
		pthread_mutex_lock(&mutex);
		done++;
        if(done >= 4){
			done = 0;
			sem_post(&sem_main);
		}
		pthread_mutex_unlock(&mutex);

		sem_wait(&sem_thread[sem_index]);
	}
	return NULL;
}

int SetupSDL2(SDL_Window **w,int ww, int wh){
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0){ //SDL_INIT_VIDEO | SDL_INIT_JOYSTICK
		printf("SDL could not initialize! SDL_Error: %s\n",SDL_GetError());
		return 0;
	}
	//Check for joysticks
	if( SDL_NumJoysticks() < 1 ){
		printf( "Warning: No joysticks connected!\n" );
	}else{
		gGameController = SDL_JoystickOpen( 0 );
		if( gGameController == NULL ){
			printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
		}else{
			printf("JOYSTICK ADDED\n");
		}
	}

	*w = SDL_CreateWindow("Raytracer",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,ww,wh,SDL_WINDOW_SHOWN);
	if(*w == NULL){
		printf("Window creation failed! SDL_Error:%s\n",SDL_GetError());
		return 0;
	}

	return 1;
}

int main(int argc,char* argv[]){
    //FT_Library library;
    //FT_Face face;
	SDL_Renderer *renderer = NULL;
	SDL_Texture  *texture  = NULL;
	SDL_Window   *window   = NULL;
	SDL_Event event;
	uint32_t window_width  = 800;
	uint32_t window_height = 640;
	uint32_t *buffer = NULL;
    int32_t failed = 0, I;


    ilInit();
    Image *imgPtr = imgNewP("./bin/tiles.jpg");

	buffer = malloc(window_width*window_height*sizeof(uint32_t));
	//raytacing stuff
    SimpleArrayList *objectList = salNew(32);
	ScreenBounds sb = {.top = 20, .bottom = -20,.left = -20,.right = 20,.invWidth=1.0f/((float)window_width),.invHeight = 1.0f/((float)window_height)};
    CameraBasis  cb = {.E={0,0,10,0},.P = {-1,0,0,0},.Q = {0,1,0,0},.R = {0,0,1,0},.UP = {0,1,0,0}, .depth = 30};
    Vec4 lightPos = v4NV(0,20,10,0);

	float xmax = 100,xmin = -100,zmax = 100,zmin = -100,yl = -1.9;
    // s pos ={0,5,-2,0};
    Triangle tri0 = {.header = {.tag = OBJ_TRIANGLE},.points = {{xmin,yl,zmin,0},{xmax,yl,zmin,0},{xmax,yl,zmax,0}}, .tOrds = {{0,0},{1,0},{1,1}} ,.texture = imgPtr };
    Triangle tri1 = {.header = {.tag = OBJ_TRIANGLE},.points = {{xmin,yl,zmin,0},{xmax,yl,zmax,0},{xmin,yl,zmax,0}}, .tOrds = {{0,0},{1,1},{0,1}} ,.texture = imgPtr };
    Sphere s      = {.header = {.tag = OBJ_SPHERE  },.centroid = {0,5,-2,0},.ambientColor = {0.3f,0.3f,0.3f,0.3f},.radius = 2.0f};
    Sphere s0     = {.header = {.tag = OBJ_SPHERE  },.centroid = {0,2,5,0 },.ambientColor = {0.6f,0.1f,0.01f,0.3f},.radius = 1.0f};
    Sphere s1     = {.header = {.tag = OBJ_SPHERE  },.centroid = {10,2,5,0 },.ambientColor = {0.9f,0.1f,0.5f,0.9f},.radius = 0.1f};
    Sphere sLight     = {.header = {.tag = OBJ_SPHERE  },.centroid = {10,2,5,0 },.ambientColor = {0.9f,0.1f,0.5f,0.9f},.radius = 1.0f};

    TraceInfoBody tib_struct = {.ww = window_width, .wh = window_height, .sb = &sb , .cb = &cb, .objectList = objectList,.buffer = buffer, .lightPos = &lightPos};
	TraceInfo tr0 = {.tIndex = 0, .x0 = 0, .y0 = 0, .tib = tib_struct};
	TraceInfo tr1 = {.tIndex = 1, .x0 = window_width/2, .y0 = 0, .tib = tib_struct};
	TraceInfo tr2 = {.tIndex = 2, .x0 = 0, .y0 = window_height/2, .tib = tib_struct};
	TraceInfo tr3 = {.tIndex = 3, .x0 = window_width/2, .y0 = window_height/2, .tib = tib_struct};

    salAdd(objectList,&s0);
    salAdd(objectList,&s);
    //salAdd(objectList,&s1);
    //salAdd(objectList,&sLight);
    salAdd(objectList,&tri0);
    salAdd(objectList,&tri1);

    for(I = 0; I < salSize(objectList); I++){
        objectUpdateMetaData((Object*)salGet(objectList,I));
    }

    sem_init(&sem_main,0,0);
    for(I = 0; I < 4; I++){
		sem_init(&sem_thread[I],0,0);
	}





	if(SetupSDL2(&window,window_width,window_height)){
		failed = 0;//SetupFreeType(&library);
		//LoadFont(&library,&face,window);
		renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
        texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,window_width,window_height);

        pthread_create(&thread[0],NULL,tracer_start_routine,&tr0);
        pthread_create(&thread[1],NULL,tracer_start_routine,&tr1);
        pthread_create(&thread[2],NULL,tracer_start_routine,&tr2);
        pthread_create(&thread[3],NULL,tracer_start_routine,&tr3);

        SDL_Delay(500);

        Uint32 t0,dt,t2;
		t2 = SDL_GetTicks();
        //t3 = t2;
		while(failed == 0){
			t0 = SDL_GetTicks();

            sem_wait(&sem_main);

            if(HandleEvents(&event,&cb)){
				break;
			}

			if(WASD[0])v4IAS(&cb.E,&cb.R,-1.0f);
			if(WASD[1])v4IAS(&cb.E,&cb.R,1.0f);
			if(WASD[2])v4IAS(&cb.E,&cb.P,1.0f);
			if(WASD[3])v4IAS(&cb.E,&cb.P,-1.0f);

//            imgRender(imgPtr,1,1,0,0,buffer,window_width);

			SDL_UpdateTexture(texture,NULL,buffer,window_width*sizeof(uint32_t));
			SDL_RenderCopy(renderer,texture,NULL,NULL);
			SDL_RenderPresent(renderer);
			memset(buffer,0,sizeof(uint32_t)*window_width*window_height);

            if( (SDL_GetTicks()-t2) > 250 ){
                sLight.centroid = lightPos;
                s1.centroid = lightPointList[tempCount%9];
                v4IA(&s1.centroid,&lightPos);
                tempCount++;
				dt = SDL_GetTicks()-t0;
                float FPS = 1000.0f/((float)dt);
                printf("FPS = %.2f\n",FPS);
				t2 = SDL_GetTicks();
                AVG +=FPS;
                AVG_count++;
            }

            for (I = 0;I<4;I++){
                sem_post(&sem_thread[I]);
            }
		}

        printf("AVG fps = %.2f\n",AVG/AVG_count);

        if(gGameController)SDL_JoystickClose(gGameController);
        imgFree(imgPtr);
		free(buffer);
		//FT_Done_FreeType(library);
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}
	return 0;
}

int HandleEvents(SDL_Event *event,CameraBasis *cb){
	const int JOYSTICK_DEAD_ZONE = 8000;
	while( SDL_PollEvent(event)){
		if(event->type == SDL_JOYAXISMOTION){
			//printf("CONTROLLER DETECTED %d\n",event->jaxis.value );
			if(event->jaxis.which == 0){
				if( event->jaxis.axis == 0 ){
					if( event->jaxis.value < -JOYSTICK_DEAD_ZONE ){
						WASD[3] = true;
					}else if( event->jaxis.value > JOYSTICK_DEAD_ZONE ){
						WASD[2] = true;
					}else{
						WASD[2] = WASD[3] = false;
					}
				}
				if( event->jaxis.axis == 1 ){
					//Below of dead zone
					if( event->jaxis.value < -JOYSTICK_DEAD_ZONE ){
						WASD[0] = true;
					}else if( event->jaxis.value > JOYSTICK_DEAD_ZONE ){
						WASD[1] = true;
					}else{
						WASD[0] = WASD[1] = false;
					}
				}
			}
		}
		if(event->type == SDL_QUIT){
			return 1;
		}
		if(event->key.type == SDL_KEYDOWN){
			switch(event->key.keysym.sym){
				case SDLK_ESCAPE:
					return 1;
				break;
				case SDLK_w:
					WASD[0] = true;
				break;
				case SDLK_s:
					WASD[1] = true;

				break;
				case SDLK_d:
					WASD[2] = true;
				break;
				case SDLK_a:
					WASD[3] = true;
				break;
				default:
				break;
			}
		}
		if(event->key.type == SDL_KEYUP){
			switch(event->key.keysym.sym){
				case SDLK_w:
					WASD[0] = false;
				break;
				case SDLK_s:
					WASD[1] = false;
				break;
				case SDLK_d:
					WASD[2] = false;
				break;
				case SDLK_a:
					WASD[3] = false;
				break;
				default:
				break;
			}
		}
		if(event->type == SDL_MOUSEMOTION){
			MAT44 rotMat;
			const float SX = 0.01f;
			const float SY = 0.01f;
			float xrad = ((float)event->motion.xrel)*SX;
			float yrad = ((float)event->motion.yrel)*SY;

			if ( xrad*xrad + yrad*yrad < 1.0f) {
				//vertical rotation
				mat44SetRotationOnAxis(&rotMat,&cb->P,yrad);
				cb->R = mat44Transform(&rotMat,&cb->R);
				//horizontal rotation
				mat44SetRotationOnAxis(&rotMat,&cb->Q,xrad);
				cb->R = mat44Transform(&rotMat,&cb->R);
				v4Cross(&cb->P,&cb->R,&cb->UP);
				v4Cross(&cb->Q,&cb->P,&cb->R);
				v4Norm(&cb->P);
				v4Norm(&cb->Q);
				v4Norm(&cb->R);
				//printf("P");v4Print(&cb->P);printf("\n");
				//printf("Q");v4Print(&cb->Q);printf("\n");
				//printf("R");v4Print(&cb->R);printf("\n");
			}
		}
	}
	return 0;
}
