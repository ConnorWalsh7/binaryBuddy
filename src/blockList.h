/*
 * blockList.h
 *
 *  Created on: 2015-03-29
 *      Author: connor
 */

#ifndef BLOCKLIST_H_
#define BLOCKLIST_H_
#include "block.h"

struct Node
{
	struct Block block;
	struct Node *next;
};

typedef struct
{
	struct Node *head;
}blockList;

struct Node * List_init(struct Node *head);
struct Node * add(struct Node *, struct Block);
struct Node * delete(struct Node *, struct Block);
void print_list(struct Node *);
void getSize();


#endif /* BLOCKLIST_H_ */

