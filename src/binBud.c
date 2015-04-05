/*
 * binBud.c
 *
 *  Created on: 2015-03-29
 *      Author: connor
 */

#include <stdio.h>
#include <stdlib.h>
#include "block.h"
#include "blockList.c"

int start_memory(int size);
struct Block get_memory(int size);
struct Block grow_memory(int size, struct Block);
void pregrow_memory(int size, struct Block);
void release_memory(struct Block);
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

	struct Block testBlock = get_memory(62);
	struct Block test2 = get_memory(45);
	struct Block test3 = get_memory(15);

	printf("\n+++++++++Free Blocks++++++++++\n");
	print_list(freeBlocks.head);
	printf("\n+++++++++Used Blocks+++++++++++\n");
	print_list(usedBlocks.head);

	release_memory(testBlock);
	release_memory(test3);
	//release_memory(test2);


	printf("\n+++++++++Free Blocks++++++++++\n");
	print_list(freeBlocks.head);
	printf("\n+++++++++Used Blocks+++++++++++\n");
	print_list(usedBlocks.head);

	end_memory();

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
		current_allocations-= usedBlocks.head->block.block_size;
		usedBlocks.head = delete(usedBlocks.head, usedBlocks.head->block);
	}
	printf("Total memory requested: %d\n", total_allocations);
	printf("Memory still allocated at termination: %d\n", current_allocations);

	printf("\n==============Lists after================\n");
	print_list(freeBlocks.head);
	print_list(usedBlocks.head);
}

/*
 * Given the size, get_memory() will first look through freeBlocks to find a big enough block.
 * When found, it will check if the block can be split in half and still fit size requirement
 * Function recurses repeating this process until blocks are as small as possible
 * Then we create a new Block struct to add to usedBlocks, and remove that block from freeBlocks
 * function returns the pointer to that block with the base address and size
 */
struct Block get_memory(int size)
{
	if(size > initial_size)
	{
		return NULL;
	}
	struct Block usedBlock;
	usedBlock.block_base = -1;
	usedBlock.block_size = -1;

	struct Node *searchNode = freeBlocks.head;

	/*Look through freeBlocks list and find the first block of appropriate size */
	while(searchNode != NULL && searchNode->block.block_size < size)
	{
		searchNode = searchNode->next;

	}
	if(searchNode == NULL)	//reached end of list without finding a big enough block
	{
		printf("Not able to get requested memory\n");
		return usedBlock;
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
			usedBlock.block_base = searchNode->block.block_base;
			usedBlock.block_size = searchNode->block.block_size;
			usedBlocks.head = add(usedBlocks.head, usedBlock);
			freeBlocks.head = delete(freeBlocks.head, searchNode->block);
			total_allocations+= usedBlock.block_size;
			current_allocations += usedBlock.block_size;
			return usedBlock;

		}
	}
}

/*
 * Find block that pointer points to
 * remove from usedBlocks
 * insert back into freeBlocks
 */
void release_memory(struct Block b)
{
	usedBlocks.head = delete(usedBlocks.head, b);

	//Find block that should proceed this block in freeBlocks
	struct Node *searchNode = freeBlocks.head;
	while(searchNode != NULL && searchNode->block.block_base >= b.block_base)
	{
		searchNode = searchNode->next;
	}
	if(searchNode != NULL)
	{
		/*Check if block can be combined into existing block
		 * Base address's must be in order and new size of block must be a power of 2
		 */
		int newSize = searchNode->block.block_size + b.block_size;
		if((searchNode->block.block_base + searchNode->block.block_size) == b.block_base && !(newSize & (newSize-1)))
		{
			printf("Combining block\n");
			searchNode->block.block_size = newSize;
		}
		else
		{
			freeBlocks.head = insert(freeBlocks.head, b, searchNode->block);
		}
		current_allocations-= b.block_size;
	}
	else
	{
		printf("Block not found\n");
	}
}

/*
 * Look through freeBlocks list and see if the next block is available
 * If available, check if size is enough
 * If not, look for a different block of appropriate size
 * copy contents from existing block to new block
 * return new block
 */
struct Block grow_memory(int growSize, struct Block b)
{
	int base = b.block_base;
	int size = b.block_size;

	if(growSize > initial_size)
	{
		return NULL;
	}
	if(growSize < size)
	{
		//Reduce space here
		return NULL;
	}

	struct Node *searchNode = freeBlocks.head;
	while(searchNode != NULL && searchNode->block.block_base != (base+size))
	{
		searchNode = searchNode->next;
	}
	if(searchNode == NULL)
	{
		printf("Cannot expand block, will try to find new memory space\n");
	}
	else
	{
		//This block should proceed the given block
		if(searchNode->block.block_size + size >= growSize)
		{
			//See if we can split block and still be the right size
			if(searchNode->block.block_size/2 + size >+ growSize)
			{
				//split block
			}
		}
	}
	return NULL;


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
