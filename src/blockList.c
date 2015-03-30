/*
 * blockList.c
 *
 *  Created on: 2015-03-29
 *      Author: connor
 *
 * Simple Linked list for block structures. I add new nodes to the front of the list because
 * when new blocks are made, they will be smaller than the block they were broken up from
 * and I want the blocks to be in increasing order to potentially reduce search time for available blocks


#include <stdio.h>
#include "blockList.h"
#include "block.h"


void List_init(struct Node *head)
{
	if(head != NULL)
	{
		head = NULL;
	}

}

void add(struct Node *head, struct Block newBlock)
{

	struct Node *newNode = (struct Node *) malloc(sizeof(struct Node));
	if(newNode != NULL)
	{
		newNode->block = newBlock;
		newNode->next = NULL;

		if(head->next == NULL) //empty list
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

void delete(blockList *bList, struct Block b)
{
	struct Node *searchNode = bList->head;
	if(searchNode->block.block_base == b.block_base)
	{
		struct Node *temp = bList->head;
		bList->head = temp->next;
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

void print_list(blockList *bList)
{
	struct Node *searchNode = bList->head;
	while(searchNode->next != NULL)
	{

		printf("Base Address: %d", searchNode->block.block_base);
		printf("Block size: %d", searchNode->block.block_size);
		printf("---------\n");

		searchNode = searchNode->next;
	}
}

*/
