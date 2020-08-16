#include "../include/stack.h"
#include <stdlib.h>
#include <stdio.h>

stack* stack_new(){
    stack* s = (stack*)calloc(1,sizeof(stack));
    return s; 
}

void stack_push(stack* s, nodedata item){
    snode *new_node = calloc(1,sizeof(snode));
    new_node->dataItem = item;
    new_node->right = s->top; 
    s->top = new_node;
}

void stack_push_node(stack *s, snode* sn){
    snode *new_node = sn;
    new_node->right = s->top; 
    s->top = new_node;
}

snode* stack_pop(stack* s){
    if(s->top == NULL) return NULL;
    snode *oldtop = s->top;
    s->top = s->top->right; 
    return oldtop; 
}

void stack_free(stack* s){
    snode *n = s->top, *t; 
    while(n != NULL){
        t = n->right;
        free(n);
        n = t;  
    }
    free(s);
}

nodedata stack_peek(stack* s){
    return s->top->dataItem;
}

void stack_print(stack* s){
    snode *n = s->top;
    int I = 0;  
    printf("[");
    while(n != NULL){
        printf((I++==0)?"%d":",%d",n->dataItem.i);
        n = n->right; 
    }
    printf("]\n");
}