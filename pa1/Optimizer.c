/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"


void optimizeOperands(Instruction *head, Instruction *ptr, int field)
{
	//1 when the current field is an offset, 0 when not
	int offset = 0;
	if(ptr->opcode == OUTPUTAI)
		offset = 1;
	while(ptr != head)
	{

		if((ptr->critical) == 1)
		{
			ptr = ptr->prev;
			continue;
		}
		
		//Get critical opcode from the given field from the previous iteration
		switch(ptr->opcode)
		{
			case LOADI:
				if(field == ptr->field2 && offset == 0)
				{
					ptr->critical = 1;
				}
				break;
			case LOADAI:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					offset = 1;
					field = ptr->field2;
				}
				break;
			case STOREAI:
				if(field == ptr->field3 && offset == 1)
				{
					ptr->critical = 1;
					offset = 0;
					field = ptr->field1;
				}
				break;
			case ADD:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
					
				}
				break;
			case SUB:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
					
				}
				break;
			case MUL:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
					
				}
				break;
			case DIV:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
				}
				break;
			case OUTPUTAI:
				ptr->critical = 1;
				optimizeOperands(head,ptr,ptr->field2);
			  	break;
			default:
				ERROR("Illegal instructions\n");
		}
		ptr = ptr->prev;
	}
}

void optimize(Instruction *head)
{
	Instruction *ptr = head;
	ptr = LastInstruction(ptr); 
	if(ptr->opcode != OUTPUTAI)
		return;

	//Mark as critical and take what needs to be derived next
	ptr->critical = 1;
	int field = ptr->field2;
	//1 when the current field is an offset, 0 when not
	int offset = 1;
	//Loop through the instruction linked list, skipping the criticals
	while(ptr != head)
	{
		if((ptr->critical) == 1)
		{
			ptr = ptr->prev;
			continue;
		}
		
		//Get crtical opcode from the given field from the previous iteration
		switch(ptr->opcode)
		{
			case LOADI:
				if(field == ptr->field2 && offset == 0)
					ptr->critical = 1;
				break;
			case LOADAI:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					offset = 1;
					field = ptr->field2;
				}
				break;
			case STOREAI:
				if(field == ptr->field3 && offset == 1)
				{
					ptr->critical = 1;
					offset = 0;
					field = ptr->field1;
				}
				break;
			case ADD:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
				}
				break;
			case SUB:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
				}
				break;
			case MUL:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
				}
				break;
			case DIV:
				if(field == ptr->field3 && offset == 0)
				{
					ptr->critical = 1;
					optimizeOperands(head,ptr,ptr->field1);
					optimizeOperands(head,ptr,ptr->field2);
					field = -1;
				}
				break;
			case OUTPUTAI:
				ptr->critical = 1;
				optimizeOperands(head,ptr,ptr->field2);
			  	break;
			default:
				ERROR("Illegal instructions\n");
		}
		ptr = ptr->prev;
	}

}

void removeNonCrit(Instruction *head)
{
	Instruction *ptr = head;
	Instruction *temp = NULL;
	while(ptr)
	{
		if(ptr->critical != 1)
		{
			temp = ptr->next;
			if(ptr->prev != NULL)
				ptr->prev->next = ptr->next;
			if(ptr->next != NULL)
				ptr->next->prev = ptr->prev;
			free(ptr);
			ptr = temp;
		}
		else
			ptr = ptr->next;
	}
}

int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	head->critical = 1;
	optimize(head);
	
	//Remove non criticls and print out the list
	removeNonCrit(head);
	if (head)
		PrintInstructionList(stdout, head);

	//Free the list
	Instruction *ptr = head;
	Instruction *temp = ptr;
	while(ptr)
	{
		temp = ptr->next;
		free(ptr);
		ptr = temp;
	}
	
	return EXIT_SUCCESS;
}

