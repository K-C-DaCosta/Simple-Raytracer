#include <node.h>

#ifndef _STACK_H_
#define _STACK_H_

typedef struct stack_
{
    snode *top;
} stack;

stack *stack_new();                        /*creates a new stack object on heap returns said object*/
void stack_push(stack *s, nodedata item);  /*create node carrying a value 'item' and pushes onto stack*/
void stack_push_node(stack *s, snode *sn); /*pushes node onto stack -- no creation*/
snode *stack_pop(stack *s);                /*'removes a node from the top of a stack'*/
void stack_free(stack *s);                 /*frees stack -- frees all nodes still on stackt*/
nodedata stack_peek(stack *s);             /*returns the value at the top of the stack*/
void stack_print(stack *s);                /*prints the stack in list form*/

#endif