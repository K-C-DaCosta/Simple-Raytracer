#include <node.h>
#include <queue.h>



#ifndef _BST_H_
#define _BST_H_
	typedef struct bst_{
		dnode *root;
		int  (*compare)(nodedata *a, nodedata *b);
		void (*print_nd)(nodedata nd);
	}bst;

	bst*    bst_new( int(*compare)(nodedata*,nodedata*),void(*print_nd)(nodedata)  );
	void    bst_insert(bst *t,nodedata nd);
	dnode** bst_search(bst *t,nodedata nd ,dnode **pn,int *pd);
	dnode*  bst_remove(bst *t,nodedata nd);

	/**returns the largest node on the left subtree*/
	dnode*  bst_left_successor(bst* t, dnode *node,dnode **p);
	
	/*creates a queue in the breadthfirst ordering*/
	queue*  bst_breadthfirst(bst *t);
	
	/*checks whether the tree is correct. On return : 1 == true and 0 == false*/
	int 	bst_validate(dnode *n);
	
	/*prints tree*/
	void    bst_print(bst *t,queue* q);
	
	/*frees all tree and all of its children*/
	void    bst_free(bst* t);
#endif
