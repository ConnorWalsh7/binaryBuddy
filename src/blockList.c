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

