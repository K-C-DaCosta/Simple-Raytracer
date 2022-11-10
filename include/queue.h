#include <node.h>

#ifndef _QUEUE_H_
#define _QUEUE_H_
	typedef struct queue_{
		snode* head; 
		snode* tail;
	}queue;

	queue* queue_new();
	int    queue_isempty(queue *q);
	void   queue_insert(queue *q,nodedata nd);
	void   queue_insert_node(queue *q,snode* sn);
	void   queue_insert_sort(queue *q, nodedata nd);
	snode* queue_remove(queue *q);
	void   queue_print(queue *q);
	void   queue_free(queue *q);
#endif