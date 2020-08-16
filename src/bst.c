#include <stdio.h> 
#include <stdlib.h>
#include "../include/bst.h"
#include "../include/stack.h"
#include "../include/queue.h"

const int LEFT = 0;
const int RIGHT = 1;

bst* bst_new( int(*cmp)(nodedata*,nodedata*) ,void (*pnd)(nodedata) ){
	if(cmp == NULL || pnd == NULL )return NULL;
	bst* t = calloc(1,sizeof(bst));
	t->compare = cmp; 
	t->print_nd = pnd;
	return t; 
}

dnode** bst_search(bst *t, nodedata nd,dnode **pn,int *pdir){
	dnode **n = &t->root;
	int direction = -1;
	int compare; 
	dnode *parent =  NULL;

	while(*n != NULL ){
		/*calculate direction of next node*/
		compare = t->compare(&nd,&(*n)->dataItem);
		/*advance to next child node if current node doesnt match nd*/
		if (compare == 0){
			break;
		}else{
			direction = (compare==1)?RIGHT:LEFT;
			parent = *n;/*Store parent address before advancing to child*/
			n = (*n)->child_list+direction;
		}
	}
	if(pn != NULL)*pn = parent; 
	if(pdir != NULL)*pdir = direction; 
	return n; 
}

void bst_insert(bst *t,nodedata nd){
	dnode **result = bst_search(t,nd,NULL,NULL);
	if(*result == NULL){
		*result = (dnode*)calloc(1,sizeof(dnode));
		(*result)->dataItem = nd;
	}else{
		printf("Error: Item already exists\n");
	}
}

dnode* bst_remove(bst *t,nodedata nd){
	int child_count = 0,direction = -1 ,I; 
	dnode *parent = NULL , *target = NULL , *targ_child = NULL;

	target = *bst_search(t,nd,&parent,&direction); 
	if( target == NULL){
		printf("node not found!\n");
		return NULL;
	}
	
	for(I = 0; I < 2; I++){
		if( target->child_list[I] != NULL){
			targ_child = target->child_list[I];
			child_count++;
		}
	}

	switch(child_count){
		case 0:{
			if(parent == NULL){
				t->root = NULL;
			}else{
				parent->child_list[direction] = NULL;
			}
		}break;
		case 1:{
			if(parent == NULL){
				t->root = targ_child;
			}else{
				parent->child_list[direction] = targ_child; 
			}
		}break;
		case 2:{
			/*note that: 'ltn' == "left tree node"  'ltnp' ==  "left tree node's parent"*/
			dnode *maxltnp = target;/*this must be initalized*/
			dnode *maxltn = bst_left_successor(t,target->child_list[LEFT],&maxltnp);
			direction = (maxltnp->child_list[LEFT]==maxltn)?LEFT:RIGHT;
			target->dataItem = maxltn->dataItem;
			if( (targ_child=maxltn->child_list[LEFT]) != NULL || (targ_child =maxltn->child_list[RIGHT]) != NULL){
				maxltnp->child_list[direction] = targ_child;
			}else{
				maxltnp->child_list[direction] = NULL;
			}
			target = maxltn;
		}break;

		default:
		break;
	}
	return target;
}

queue* bst_breadthfirst(bst *t){/*generates queue of tree nodes in level order*/
	nodedata td;
	snode *popped_node;  
	dnode *tdn;
	queue *search_queue = queue_new(), *return_queue = queue_new(); 
	int I;
	
	/*Push tree root,of type dnode*,into the queue*/
	if(t->root != NULL){ 
		td.vp = (void*)t->root;
		queue_insert(search_queue,td);
	}

	while( !queue_isempty(search_queue) ){
		popped_node = queue_remove(search_queue);
		queue_insert_node(return_queue,popped_node);
		/*Cast the void* in the queuenode,called popped_node, to the type dnode*.*/
		tdn = (dnode*)popped_node->dataItem.vp;
		for( I = 0; I < 2 ; I++){
			if(tdn->child_list[I] != NULL){ 
				td.vp = (void*)tdn->child_list[I];
				queue_insert(search_queue,td);
			}
		}
	}
	/*At this point the search_queue should be empty so queue_free is called to clean things up.*/
	queue_free(search_queue);
	return return_queue; 
}


void bst_print(bst *t,queue *return_queue){/*Prints the binary tree from left to right in level order and frees return_queue*/
	snode *popped_node;
	dnode *tdn; 
	int I =0; 

	if(return_queue == NULL) return;
	printf("[");
	while(!queue_isempty(return_queue) ){
		popped_node = queue_remove(return_queue);
		tdn = (dnode*)popped_node->dataItem.vp; 
		if(I++ != 0)printf(",");
		t->print_nd(tdn->dataItem);
		free(popped_node);
	}
	printf("]\n");
	queue_free(return_queue);
}

void  bst_free(bst *t){
	snode* tsn;
	dnode* tdn; 
	queue* q = bst_breadthfirst(t);
	while(!queue_isempty(q)){
		tsn = queue_remove(q);
		tdn = (dnode*)tsn->dataItem.vp;
		free(tdn);/*free dnode from the tree*/
		free(tsn);/*free snode from the queue*/
	}
	queue_free(q);/*clean up remaining queue*/
	free(t);/*clean up remainding tree*/
}

dnode* bst_left_successor(bst *t,dnode *node,dnode **p){/*parent direction always right*/
	nodedata max_val  = (*p)->dataItem;
	dnode *max_node = node , *max_parent_node = *p, *pn = *p; 

	while(node != NULL){
		if(t->compare(&node->dataItem,&max_val)==1){
			max_val = node->dataItem;
			max_parent_node = pn; 
			max_node = node; 
		}
		pn = node; 
		node = node->child_list[RIGHT];
	}

	if(p != NULL) *p = max_parent_node;
	return max_node;
}

int bst_validate(dnode *n){
	const int MAX = 999999999;
	int l = 0, rt = 0,r=0 , leftBranch = 1, rightBranch = 1;

	if(n->child_list[LEFT] != NULL)
		leftBranch=bst_validate(n->child_list[LEFT]);

	l = (n->child_list[LEFT]==NULL)?-MAX: n->child_list[LEFT]->dataItem.i;
	rt= n->dataItem.i;

	if(n->child_list[RIGHT]!=NULL)
		rightBranch=bst_validate(n->child_list[RIGHT]);

	r = (n->child_list[RIGHT]==NULL)?MAX:n->child_list[RIGHT]->dataItem.i;
	printf("%d < %d < %d\n",l,rt,r);

	if((l<rt)&&(rt<r)&&leftBranch&&rightBranch){
		return 1;
	}else{ 		
		return 0;
	}
}