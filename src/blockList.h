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

void init();
void add(struct Block);
void delete(struct Block);
void print_list();


#endif /* BLOCKLIST_H_ */

