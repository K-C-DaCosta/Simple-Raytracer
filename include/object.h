#include <vector.h>
#include <simple_array_list.h>

#ifndef _OBJECT_H_
#define _OBJECT_H_
    typedef enum OBJ_TAG_ {
        OBJ_TRIANGLE,
        OBJ_SPHERE,
        OBJ_TYPE_COUNT
    }OBJ_TAG;

    typedef struct Object_{
        OBJ_TAG tag;
    }Object;

    Object *objectGetNearest(SimpleArrayList *objectList,Vec4* ray , Vec4 * E,float *t0);
    float   objectRayTest(Vec4 *ray, Vec4 *e,Object *obj);
    void    objectGetNormal(Vec4 *point, Vec4 *normal,Object *obj);
    Object *objectIsBlockingLight(SimpleArrayList *objectList, Vec4* ray , Vec4 * E);
    void    objectGetAmbientColor(Vec4 *point, Vec4 *ambient, Object *obj);
    void    objectUpdateMetaData(Object* obj);
#endif
