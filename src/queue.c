#include "../include/queue.h"
#include <stdlib.h>
#include <stdio.h>

queue* queue_new(){
	queue *q =  calloc(1,sizeof(queue));
	return q; 
}

void   queue_insert(queue *q,nodedata nd){
	snode* new_node = (snode*)calloc(1,sizeof(snode));
	new_node->dataItem = nd; 

	if( queue_isempty(q) ){
		q->tail = new_node; 
		q->head = new_node; 
	}else{
		q->tail->right = new_node;
		q->tail = new_node; 
	}
}
void queue_insert_node(queue *q,snode* sn){
	snode* new_node = sn;

	if( queue_isempty(q) ){
		q->tail = new_node; 
		q->head = new_node; 
	}else{
		q->tail->right = new_node;
		q->tail = new_node; 
	}
}

void queue_insert_sort(queue *q, nodedata nd){
	snode *new_node = (snode*)calloc(1,sizeof(snode)) , *t;
	new_node->dataItem = nd; 
	
	if( queue_isempty(q) ){
		q->tail = new_node; 
		q->head = new_node; 
	}else{
		t = q->head; 
		if(nd.i < t->dataItem.i ){
			new_node->right = q->head; 
			q->head = new_node; 
			return;
		}else{
			while( t != NULL && t->right !=NULL && nd.i > t->right->dataItem.i )t = t->right; 
			new_node->right = t->right; 
			t->right = new_node;
		} 
	}
}

int queue_isempty(queue *q){
	return q->head == NULL && q->tail == NULL; 
}

snode* queue_remove(queue *q){
	snode *oldhead; 
	if( q->head == NULL)
		return NULL; 
	
	oldhead = q->head; 
	q->head = q->head->right;

	if(q->head == NULL) 
		q->tail = NULL; 
	
	return oldhead;
}
void queue_print(queue *q){
	snode *n = q->head;
    int I = 0;  
	printf("[");
    while(n != NULL){
        printf((I++==0)?"%d":",%d",n->dataItem.i);
        n = n->right; 
    }
    printf("]\n");
}

void  queue_free(queue *q){
	snode *n = q->head ,  *t; 
	while( n != NULL){
		t = n->right;
		free(n);
		n = t;
	}
	free(q);
}