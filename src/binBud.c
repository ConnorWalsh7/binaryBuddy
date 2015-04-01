/*
 * binBud.c
 *
 *  Created on: 2015-03-29
 *      Author: connor
 */

#include <stdio.h>
#include <stdlib.h>
#include "block.h"
//#include "blockList.h"
#include "blockList.c"

int start_memory(int size);
void *get_memory(int size);
void *grow_memory(int size, void *p);
void *pregrow_memory(int size, void *p);
void *release_memory(void *p);
void end_memory(void);
void testList();

/* Global Variables */
int *base_address = NULL;
int total_allocations = 0;
int current_allocations = 0;
/*Does initial size need to be of a power of 2? */
int initial_size = 0;

blockList freeBlocks;
blockList usedBlocks;

int main(int argc, char **argv)
{
	/* Setup two linked lists for Block structs to keep track of what memory is free and what memory is used */
	freeBlocks.head = (struct Node *) malloc(sizeof(struct Node));
	freeBlocks.head = List_init(freeBlocks.head);

	usedBlocks.head = (struct Node *) malloc(sizeof(struct Node));
	usedBlocks.head = List_init(usedBlocks.head);

	if(argc < 2)
	{
		printf("How much memory do you need(in bytes): ");
		scanf("%d", &initial_size);
	}
	else
	{
		initial_size = atoi(argv[1]);
	}

	start_memory(initial_size);
	printf("+++++++++Original List++++++++++\n");
	print_list(freeBlocks.head);

	get_memory(62);
	//end_memory();
	printf("\n+++++++++Free Blocks++++++++++\n");
	print_list(freeBlocks.head);
	printf("\n+++++++++Used Blocks+++++++++++\n");
	print_list(usedBlocks.head);


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

	struct Block block;
	block.block_base = *base_address;	//Is this the address we want to keep track of??
	block.block_size = size;

	//Add to freeBlocks LinkedList
	freeBlocks.head = add(freeBlocks.head, block);

	return 1;
}
/*
 * end_memory function will iterate through both blockLists and free any allocated resources
 * It will also print out if there are any memory leaks
 */
void end_memory(void)
{
	while(freeBlocks.head != NULL)
	{
		freeBlocks.head = delete(freeBlocks.head, freeBlocks.head->block);
	}
	while(usedBlocks.head != NULL)
	{
		usedBlocks.head = delete(usedBlocks.head, usedBlocks.head->block);
	}
	print_list(freeBlocks.head);
}

/*
 * Given the size, get_memory() will first look through freeBlocks to find a big enough block.
 * When found, it will check if the block can be split in half and still fit size requirement
 * Function recurses repeating this process until blocks are as small as possible
 * Then we create a new Block struct to add to usedBlocks, and remove that block from freeBlocks
 * function returns the pointer to that block with the base address and size
 */
void *get_memory(int size)
{
	struct Block *usedBlock;
	struct Node *searchNode = freeBlocks.head;

	/*Look through freeBlocks list and find the first block of appropriate size */
	while(searchNode != NULL && searchNode->block.block_size < size)
	{
		searchNode = searchNode->next;

	}
	if(searchNode == NULL)	//reached end of list without finding a big enough block
	{
		printf("Not able to get requested memory\n");
		return NULL;
	}
	else	//Found block big enough. Now check if we can split this block into smaller blocks
	{
		if(searchNode->block.block_size/2 >= size)
		{
			//Split block
			int newBase = searchNode->block.block_base + (searchNode->block.block_size/2);
			int newSize = searchNode->block.block_size/2;
			//Create new block of half the size of original block
			struct Block newBlock;
			newBlock.block_base = newBase;
			newBlock.block_size = newSize;

			//modify original block to show it is half the size of original
			searchNode->block.block_size = newSize;

			//insert new block in front of original block
			freeBlocks.head = insert(freeBlocks.head, newBlock, searchNode->block);

			//Call function again until blocks cannot be split
			get_memory(size);
		}
		else
		{
			//We've split the blocks as much as we need, remove from freeBlocks and add to usedBlocks
			usedBlock->block_base = searchNode->block.block_base;
			usedBlock->block_size = searchNode->block.block_size;
			usedBlocks.head = add(usedBlocks.head, *usedBlock);
			freeBlocks.head = delete(freeBlocks.head, searchNode->block);

		}
	}
	return usedBlock;
}
/*
 * temporary function to test list and other things in the program
 */
void testList()
{
	struct Block b1,b2,b3,b4;
	b1.block_base = 0;
	b1.block_size = 10;

	b2.block_base = 10;
	b2.block_size = 5;

	b3.block_base = 15;
	b3.block_size = 20;

	b4.block_base = 50;
	b4.block_size = 100;

	freeBlocks.head = add(freeBlocks.head, b1);
	freeBlocks.head = add(freeBlocks.head, b2);
	freeBlocks.head = add(freeBlocks.head, b3);

	print_list(freeBlocks.head);
	printf("+++++++++++++++Inserting++++++++++++++++++\n");

	freeBlocks.head = insert(freeBlocks.head, b4, b1);
	print_list(freeBlocks.head);

}
