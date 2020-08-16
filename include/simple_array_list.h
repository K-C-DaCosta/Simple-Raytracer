#ifndef SIMPLE_ARRAY_LIST_H_
#define SIMPLE_ARRAY_LIST_H_
typedef struct SimpleArrayList_{
    void** data;
    int recsFilled;
}SimpleArrayList;

SimpleArrayList *salNew(int size);
void  salFree(SimpleArrayList *sal);
void  salAdd(SimpleArrayList *sal,void *data);
void* salRemove(SimpleArrayList *sal,int index);
void* salGet(SimpleArrayList *sal,int index);
int   salSize(SimpleArrayList *sal);

void  salFreeC(SimpleArrayList *sal,void(*fp)(void*));
void  salAddC(SimpleArrayList *sal,void(*fp)(void*),void* data);
void* salRemoveC(SimpleArrayList *sal,void(*fp)(int),int index);
void* salGetC(SimpleArrayList *sal,void(*fp)(int),int index);

#endif
