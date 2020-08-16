#ifndef _NODE_H_
#define _NODE_H_

	typedef union data_{
		int i;
		float f;
		char *cBuf;
		void *vp; 
	}nodedata;

	typedef struct snode_{
		nodedata dataItem;
		struct snode_ *right;
	}snode;

	typedef struct dnode_{
		nodedata dataItem; 
		struct dnode_ *child_list[2];
	}dnode;
#endif