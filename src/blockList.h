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
/*
 * List_init initializes list by setting provided head node to NULL and size to zero
 */
struct Node * List_init(struct Node *head);
/*
 * add() will insert a new node at the front of the list
 * because I want the list to be in increasing order
 * so when we search for the first smallest block it will reduce search time
 */
struct Node * add(struct Node *, struct Block);
/*
 * insert() will insert the new node in front of the provided reference block
 */
struct Node * insert(struct Node *, struct Block, struct Block);
/*
 * deletes the node containing the provided Block struct
 */
struct Node * delete(struct Node *, struct Block);
/*
 * iterates through the provided list and prints contents to output
 */
void print_list(struct Node *);
/*
 * returns size of list
 */
void getSize();


#endif /* BLOCKLIST_H_ */

