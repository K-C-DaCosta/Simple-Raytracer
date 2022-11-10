#include <simple_array_list.h>
#include <stdlib.h>
#include <stdio.h>

SimpleArrayList *salNew(int size){
    SimpleArrayList * sal = malloc(sizeof(SimpleArrayList));
    sal->data = malloc(sizeof(void*)*size);
    sal->recsFilled = 0;
    return sal;
}

void salFree(SimpleArrayList *sal){
    free(sal->data);
    free(sal);
}

void  salAdd(SimpleArrayList *sal,void *data){
    sal->data[sal->recsFilled] = data;
    sal->recsFilled++;
}

void* salRemove(SimpleArrayList *sal,int index){
    void* temp = NULL;
    sal->recsFilled--;
    temp = sal->data[sal->recsFilled];
    sal->data[sal->recsFilled] = NULL;
    return temp;
}

void* salGet(SimpleArrayList *sal,int index){
    return sal->data[index];
}

int salSize(SimpleArrayList *sal){
    return sal->recsFilled;
}
