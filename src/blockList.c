/*
 * blockList.c
 *
 *  Created on: 2015-03-29
 *      Author: connor
 *
 * Simple Linked list for block structures. I add new nodes to the front of the list because
 * when new blocks are made, they will be smaller than the block they were broken up from
 * and I want the blocks to be in increasing order to potentially reduce search time for available blocks
 */

#include <stdio.h>
#include "blockList.h"
#include "block.h"

int size;
struct Node *head;

void init()
{
	head = NULL;
	size = 0;
}

void add(struct Block newBlock)
{
	struct Node *newNode;
	newNode = (struct Node *) malloc(sizeof(struct Node));
	if(newNode != NULL)
	{
		newNode->block = newBlock;
		newNode->next = NULL;

		if(size == 0) //empty list
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
		size--;
		free(temp);
	}
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
		size--;
		free(temp);
	}
}

void print_list()
{
	struct Node *searchNode = head;
	while(searchNode->next != NULL)
	{
		//Issue with block_base being pointer??
		printf("Base Address: %d", searchNode->block.block_base);
		printf("Block size: %d", searchNode->block.block_size);
		printf("---------\n");

		searchNode = searchNode->next;
	}
}

int main()
{
	return 0;
}
