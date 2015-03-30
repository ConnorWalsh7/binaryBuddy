/*
 * binBud.c
 *
 *  Created on: 2015-03-29
 *      Author: connor
 */

#include <stdio.h>
#include <stdlib.h>
#include "block.h"


int start_memory(int size);
void *get_memory(int size);
void *grow_memory(int size, void *p);
void *pregrow_memory(int size, void *p);
void *release_memory(void *p);



void List_init();
void add(struct Block);
void delete(struct Block);
void print_list();



/* Global Variables */
int *base_address = NULL;
int total_allocations = 0;
int current_allocations = 0;
int initial_size = 0;

struct Node
{
	struct Block block;
	struct Node *next;
};



struct Node *head;

int main(int argc, char **argv)
{
	struct Block b1, b2, b3;
	b1.block_base = 10;
	b1.block_size = 2;

	b2.block_base = 13;
	b2.block_size = 4;

	b3.block_base = 18;
	b3.block_size = 1;

	//blockList *bList = (blockList *) malloc (sizeof(blockList));
	List_init();
	add(b1);
	//add(bList, b2);
	//add(bList, b3);

	//print_list(bList);



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





/* blockList code work around */

void List_init()
{
	head = NULL;
}

void add(struct Block newBlock)
{

	struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
	if(newNode != NULL)
	{
		newNode->block = newBlock;
		newNode->next = NULL;

		if(head == NULL) //empty list
		{
			head = newNode;
		}
		else	//Add newNode to front of the list
		{
			newNode->next = head;
			head = newNode;
		}

	}
	else
	{
		printf("Could not create new Node to add\n");
	}
}

void delete(struct Block b)
{
	struct Node *searchNode = head;
	if(searchNode->block.block_base == b.block_base)
	{
		struct Node *temp = head;
		head = temp->next;
		free(temp);
	}
	else
	{
		while(searchNode->next->block.block_base != b.block_base || searchNode->next != NULL)
		{
			searchNode = searchNode->next;
		}
		if(searchNode->next == NULL && searchNode->next->block.block_base != b.block_base)
		{
			printf("Node not found\n");
		}
		else
		{
			struct Node *temp = searchNode->next;
			searchNode->next = temp->next;
			free(temp);
		}
	}
}

void print_list()
{
	struct Node *searchNode = head;
	while(searchNode->next != NULL)
	{

		printf("Base Address: %d", searchNode->block.block_base);
		printf("Block size: %d", searchNode->block.block_size);
		printf("---------\n");

		searchNode = searchNode->next;
	}
}

