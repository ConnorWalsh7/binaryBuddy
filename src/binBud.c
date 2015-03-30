/*
 * binBud.c
 *
 *  Created on: 2015-03-29
 *      Author: connor
 */

#include <stdio.h>
#include <stdlib.h>
#include "block.h"
#include "blockList.h"
#include "blockList.c"

int start_memory(int size);
void *get_memory(int size);
void *grow_memory(int size, void *p);
void *pregrow_memory(int size, void *p);
void *release_memory(void *p);

/* Global Variables */
int *base_address = NULL;
int total_allocations = 0;
int current_allocations = 0;
int initial_size = 0;
struct Node *head;

int main(int argc, char **argv)
{
	head = (struct Node *) malloc(sizeof(struct Node));
	struct Block b1, b2, b3;
	b1.block_base = 10;
	b1.block_size = 2;

	b2.block_base = 13;
	b2.block_size = 4;

	b3.block_base = 18;
	b3.block_size = 1;

	//blockList *bList = (blockList *) malloc (sizeof(blockList));
	List_init(head);
	head = add(head, b1);
	head = add(head, b2);
	head = add(head, b3);

	print_list(head);

	head = delete(head, b2);
	getSize();

	print_list(head);



	return 0;
}

/*
 * Start memory function reads in an int that describes how much total memory will be allocated for the whole program
 * Other functions will work within this block to split up and allocate resources within this block
 * 0 returned if unsuccessful, otherwise it worked
 */
int start_memory(int size)
{
	base_address = (int *) malloc(size);
	if(base_address == NULL)
	{
		printf("Could not initialize memory\n");
		return 0;
	}

	initial_size = size;

	struct Block bigBlock;
	bigBlock.block_base = *base_address;
	bigBlock.block_size = size;

	//Add to LinkedList here

	return 1;
}
