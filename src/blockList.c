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


struct Node * List_init(struct Node *head)
{
	if(head != NULL)
	{
		head = NULL;
	}
	size = 0;
	return head;

}

struct Node * add(struct Node *head, struct Block newBlock)
{

	struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
	if(newNode != NULL)
	{
		newNode->block = newBlock;
		newNode->next = NULL;

		if(head == NULL || size == 0) //empty list
		{
			head = newNode;
			size++;
			return head;
		}
		else	//Add newNode to front of the list
		{
			newNode->next = head;
			head = newNode;
			size++;
			return head;
		}

	}
	else
	{
		printf("Could not create new Node to add\n");
		return NULL;
	}
}

struct Node * insert(struct Node *head, struct Block newBlock, struct Block refBlock)
{
	struct Node *searchNode = head;
	struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
	newNode->block = newBlock;
	newNode->next = NULL;

	if(head->block.block_base == refBlock.block_base)
	{
		 newNode->next = head;
		 head = newNode;
		 size++;
		 return head;
	}

	while(searchNode->next != NULL && searchNode->next->block.block_base != refBlock.block_base)
	{
		//printf("Comparing blocks\n");
		//printf("Search Node block: %d\t Ref Block: %d\n", searchNode->next->block.block_base, refBlock.block_base);

		searchNode = searchNode->next;
	}

	if(searchNode->next->block.block_base == refBlock.block_base)
	{
		//printf("Adding new node\n");
		newNode->next = searchNode->next;
		searchNode->next = newNode;
		size++;
		return head;
	}
	else
	{
		printf("Node not found, cannot insert\n");
		return head;
	}


	return head;
}

struct Node * delete(struct Node *head, struct Block b)
{
	int found = 0;
	struct Node *searchNode = head;
	if(searchNode->block.block_base == b.block_base) //Deleting head node
	{
		struct Node *temp = head;
		head = head->next;
		free(temp);
		size--;
		return head;
	}

	while (found == 0 && searchNode->next != NULL )
	{
		if (searchNode->next->block.block_base == b.block_base)
		{
			found = 1;
		}
		else
		{
			searchNode = searchNode->next;
		}
	}
	//We are either out of nodes or found a match
	if (found == 1)
	{
		if(size == 1)
		{
			struct Node *temp = head;
			head = NULL;
			free(temp);
			size--;
			return head;
		}
		else
		{
			struct Node *temp = searchNode->next; //Node we are deleting
			searchNode->next = temp->next;
			free(temp);
			size--;
			return head;
		}
	}
	else
	{
		printf("Node not found\n");
		return head;
	}

}

void print_list(struct Node *head)
{
	struct Node *searchNode = head;
	if(head == NULL)
	{
		printf("Empty List\n");
	}
	while(searchNode != NULL)
	{

		printf("Base Address: %d\n", searchNode->block.block_base);
		printf("Block size: %d\n", searchNode->block.block_size);
		printf("--------------------\n");

		searchNode = searchNode->next;
	}
}

void getSize()
{
	printf("Size: %d\n", size);
}

