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
void *get_memory(int size);
void *grow_memory(int size, void *);
void *pregrow_memory(int size, void *);
void release_memory(void *);
void end_memory(void);

/* Global Variables */
int *base_address = NULL;
int total_allocations = 0;
int current_allocations = 0;
int initial_size = 0;

/*Linked lists to keep track of free and used blocks of memory */
blockList freeBlocks;
blockList usedBlocks;

int main(int argc, char **argv)
{
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

	int* b1,b2,b3;
	b1 = get_memory(62);
	b2 = get_memory(45);
	b3 = get_memory(15);

	b3 = grow_memory(32, b3);
	b2 = pregrow_memory(16, b2);

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
	block.block_base = *base_address;
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
	struct Block usedBlock;
	usedBlock.block_base = -1;
	usedBlock.block_size = -1;
	if(size > initial_size)
		{
			return (int *)usedBlock.block_base;
		}

	struct Node *searchNode = freeBlocks.head;

	/*Look through freeBlocks list and find the first block of appropriate size */
	while(searchNode != NULL && searchNode->block.block_size < size)
	{
		searchNode = searchNode->next;

	}
	if(searchNode == NULL)	//reached end of list without finding a big enough block
	{
		printf("Not able to get requested memory\n");
		return (int *)usedBlock.block_base;
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
			return (int *)usedBlock.block_base;

		}
	}
}

/*
 * Find block that pointer points to
 * remove from usedBlocks
 * insert back into freeBlocks
 */
void release_memory(void *p)
{
	/* Find block referenced by p */
	int base = (int)p;
	struct Node *searchNode = usedBlocks.head;
	while(searchNode != NULL && searchNode->block.block_base != base)
	{
		searchNode = searchNode->next;
	}
	if (searchNode == NULL)
	{
		printf("Could not find block\n");
		return;
	}
	struct Block b = searchNode->block;

	usedBlocks.head = delete(usedBlocks.head, b);

	//Find block that should proceed this block in freeBlocks
	searchNode = freeBlocks.head;
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
void *grow_memory(int growSize, void *p)
{
	int *bp = (int *)p;
	int baseVal = (int)bp;

	/* find referenced block in usedBlocks list to make sure it exists */
	struct Node *refNode = usedBlocks.head;
	while(refNode != NULL && refNode->block.block_base != baseVal)
	{
		refNode = refNode->next;
	}
	if(refNode == NULL)
	{
		printf("reference block does not exist\n");
		return NULL;
	}

	int base = refNode->block.block_base;
	int size = refNode->block.block_size;

	if(growSize > initial_size)
	{
		printf("Not enough memory\n");
		return NULL;
	}
	/* If user requests less memory than they have, see if we can split the block */
	if(growSize <= size/2)
	{
		printf("Reducing size of memory block\n");
		struct Block splitBlock;
		splitBlock.block_base = base+size/2;
		splitBlock.block_size = size/2;

		/*Find where to insert new free block into freeBlocks list */
		refNode = freeBlocks.head;
		while(refNode != NULL && refNode->block.block_base > splitBlock.block_base)
		{
			refNode = refNode->next;
		}
		if(refNode != NULL)
		{
			freeBlocks.head = insert(freeBlocks.head, splitBlock, refNode->block);
		}
		else
		{
			printf("Cannot split block and add to freeBlocks list\n");
			return NULL;
		}
		return (int *)base;
	}

	struct Node *searchNode = freeBlocks.head;

	/*Looking for block of memory that comes after referenced block to expand into */
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
		/*This block should proceed the reference block
		 * Check if new block is big enough
		 */
		if(searchNode->block.block_size + size >= growSize)
		{
			/*Increase size of current block */
			refNode->block.block_size += searchNode->block.block_size;

			/*Update global allocation trackers*/
			current_allocations += searchNode->block.block_size;
			total_allocations += searchNode->block.block_size;
			/* remove the new block from freeBlocks */
			freeBlocks.head = delete(freeBlocks.head, searchNode->block);
			return (int *)refNode->block.block_base;
		}

	}
	/* Look for new memory space */
	searchNode = freeBlocks.head;
	while(searchNode != NULL && searchNode->block.block_size < growSize)
	{
		searchNode = searchNode->next;
	}
	if(searchNode == NULL)
	{
		printf("Not enough memory to copy over data\n");
		return NULL;
	}
	/* Found a block big enough to copy over users data
	 *  Start at old pointer address and start copying data to new address
	 */
	int* dest,src;
	dest = (int *)searchNode->block.block_base;
	src = bp;

	int i;
	for(i = 0; i<= size; i++)
	{
		dest = src;
		dest++;
		src++;
	}

	return (int *)searchNode->block.block_base;
}

void *pregrow_memory(int growSize, void *p)
{
	int *b = (int *)p;
	if(growSize > initial_size)
	{
		printf("Not enough memory\n");
		return NULL;
	}

	/*Find referenced block */
	int base = (int)b;
	struct Node *refNode = usedBlocks.head;
	while(refNode != NULL && refNode->block.block_base != base)
	{
		refNode = refNode->next;
	}
	if(refNode == NULL)
	{
		printf("Could not find referenced block\n");
		return NULL;
	}
	struct Block refBlock = refNode->block;
	if(growSize < refBlock.block_size)
	{
		//Reduce size here
		return (int *)refBlock.block_base;
	}

	/*Find block that should come before refBlock */
	refNode = freeBlocks.head;
	while(refNode != NULL && refNode->block.block_base + refNode->block.block_size != refBlock.block_base)
	{
		refNode = refNode->next;
	}
	if(refNode != NULL)
	{
		/* Check if size is big enough */
		if(refNode->block.block_size + refBlock.block_size >= growSize)
		{
			struct Block newBlock = refNode->block;
			newBlock.block_size += refBlock.block_size;
			freeBlocks.head = delete(freeBlocks.head, newBlock);
			usedBlocks.head = delete(usedBlocks.head, refBlock);
			usedBlocks.head = add(usedBlocks.head, newBlock);
			return (int *)newBlock.block_base;
		}
	}

	/* Look for a new block big enough for size requested */
	refNode = freeBlocks.head;
	while(refNode != NULL && refNode->block.block_size < growSize)
	{
		refNode = refNode->next;
	}
	if(refNode == NULL)
	{
		printf("Could not find big enough block of memory\n");
		return NULL;
	}

	/*Copy data from old pointer to new pointer loction */
	struct Block newBlock = refNode->block;
	/*Start at the old blocks size into the new block, so the free memory is before it */
	int *dest = (int *)newBlock.block_base+refBlock.block_size;
	int *src = (int *)refBlock.block_base;
	int i;
	for(i=0; i<=refBlock.block_size; i++)
	{
		dest = src;
		dest++;
		src++;
	}
	return (int *)newBlock.block_base;




}

