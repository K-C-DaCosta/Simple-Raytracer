#include "../include/object.h"
#include "../include/triangle.h"
#include "../include/sphere.h"

inline Object * objectGetNearest(SimpleArrayList *objectList, Vec4* ray , Vec4 * E, float *tmin_ret){
	//returns the closest insersecting 'object'
    int I,objectNum = salSize(objectList);
	float tmin = INFINITY,t0;
	Object *obj , *minObject = NULL;

	for(I = 0; I < objectNum; I++){
        obj = salGet(objectList,I);
		t0 = objectRayTest(ray,E,obj);
		if(t0 > 0.0f && t0 != INFINITY && t0 < tmin){
			tmin = t0;
            minObject = salGet(objectList,I);
		}
	}

	if(tmin_ret && minObject) *tmin_ret = tmin;
	return minObject;
}

inline Object * objectIsBlockingLight(SimpleArrayList *objectList, Vec4* ray , Vec4 * E){
    int I,objectNum = salSize(objectList);
    const float TMIN_LBOUND = 0.0125f;
	float t0;
	Object *obj;
    Vec4 NEW_E;

    v4ASA(&NEW_E,ray,TMIN_LBOUND,E);
    E = &NEW_E;

    for(I = 0; I < objectNum; I++){
        obj = salGet(objectList,I);
        t0 = objectRayTest(ray,E,obj);
        if(t0 > 0.0f && t0 < 1.0f){
            return obj;
        }
    }
    return NULL;
}

inline float objectRayTest(Vec4 *ray, Vec4 *e,Object *obj){
	float t = INFINITY;
	switch(obj->tag){
		case OBJ_TRIANGLE:
            t = triangleRayIntersectionTest(ray,e,(Triangle*)obj);//triangleFastRayIntersectionTest(ray,e,(Triangle*)obj);
            //ALTERNATE: triangleRayIntersectionTest(ray,e,(Triangle*)obj);
		break;
		case OBJ_SPHERE:
			t = sphereRayIntersectionTest(ray,e,(Sphere*)obj);
		break;
		default:

		break;
	}
	return t;
}

inline void objectGetNormal(Vec4 *point, Vec4 *normal,Object *obj){
	switch(obj->tag){
		case OBJ_TRIANGLE:
			triangleGetNormal(point,normal,(Triangle*)obj);
		break;
		case OBJ_SPHERE:
			sphereGetNormal(point,normal,(Sphere*)obj);
		break;
		default:
		break;
	}
}

inline void objectGetAmbientColor(Vec4 *point, Vec4 * ambient, Object *obj){
    switch(obj->tag){
        case OBJ_TRIANGLE:
            /*ambient->x = 0.27f;
            ambient->y = 0.27f;
            ambient->z = 0.27f;
            ambient->w = 0.27f;*/
            triangleGetAmbientColor(point,ambient,(Triangle*)obj);
        break;
        case OBJ_SPHERE:
            sphereGetAmbientColor(point,ambient,(Sphere*)obj);
        break;
        default:

        break;
    }
}

inline void objectUpdateMetaData(Object* obj){
    switch(obj->tag){
        case OBJ_TRIANGLE:
            triangleComputeFaceData((Triangle*)obj);
        break;
        case OBJ_SPHERE:
        break;
        default:

        break;
    }
}
