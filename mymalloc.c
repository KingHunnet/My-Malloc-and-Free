#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include "mymalloc.h"

//maximum size to malloc (might add const)
#define maxsize 5000

//Array of our "memory"
static char memory[maxsize];

//if this is the first time malloc is called and whether or not we need to initialize it
static short start = 0; 

struct node_
{
	unsigned short capacity : 15; //capacity of this memory
	unsigned short inUse : 1;  //whether or not this memory is in use
	short next; //address of next node (might change to pointer) do (node*)(&memory[Node->next]) for now
    short prev; //address of prev node (might change to pointer)
};

typedef struct node_ node;

node * head;

void merge(node * current, node * prev)
{
    prev->next = current->next;
    prev->capacity += (current->capacity + sizeof(node));

    
    node * currentsnext = (node*)(&memory[current->next]);
    currentsnext->prev = prev->next - prev->capacity - sizeof(node);

    for(int i = 0; i < prev->capacity; i++)
    {
        memory[(prev->next - prev->capacity + i)] = 0;
    }
}


//mymalloc
void* mymalloc(int sizeaskedfor){
	//if malloc of size 0 is called
	if (sizeaskedfor == 0)
	{
		// fprintf(STDERR, "malloc of size 0 called\n");
		fprintf(stderr, "malloc size of 0 called\n");
		return NULL;
	}
	//if malloc of size less than 0 is called
	if (sizeaskedfor < 0)
	{
		fprintf(stderr, "trying to malloc less than 0 bytes\n");
		return NULL;
	}

	//if this is the first time malloc is called
	if (start == 0)
	{
		//printf("first time in malloc\n");
		//set everything to null just in case there's junk leftover in memory
		for(int x = 0; x < maxsize; x++)
		{
			memory[x] = '\0';
		}

		//check if sizeaskedfor is too big, else let head get values for first node and change start to 1
		if (sizeaskedfor > (maxsize - sizeof(node)))
		{
			fprintf(stderr, "Too much space asked for\n");
			return NULL;
		}
		else
		{
			//initializing head node
			head = (node *)(&memory[0]);
			node firstdata = {0, 0, 0, 0};
			*head = firstdata;
			head->capacity = sizeaskedfor;
			head->inUse = 1;
			head->next = sizeof(node) + sizeaskedfor;
			head->prev = -1;
		}
		start = 1;

		//return pointer to beginning of allowed memory
		// printf("head pointer:%p\n", head);
		fprintf(stderr, "worked in malloc first time\n");
		//printf("node: %d, %d, %d, %d\n", head->capacity, head->inUse, head->next, head->prev);
        node * retnode = head + 1;
		void * ret = (void*)(retnode);
		// printf("void * return: %p\n", ret);
		
		return ret;
	}

	//find open location with enough capacity for sizeaskedfor
	node * current = head;
	int usedcapacity = 0;
	int lastbefore0;
	int pass = 0;
	while(1)
	{
		//checking if that Node has enough memory
		if (sizeaskedfor <= current->capacity && current->inUse == 0)
		{
			if (current->capacity - sizeaskedfor < sizeof(node))
			{
				//set node inUse to true and use all the capacity
				current->inUse = 1;

				//printf("node: %d, %d, %d, %d\n", current->capacity, current->inUse, current->next, current->prev);

				current++;
				void * retoldnode = (void*)(current);
				

				//printf("malloc worked 1\n");
				return retoldnode;
			}
			else
			{
				//make new node with asked for size and point it to new node that has leftover size and point that to original node's next
				current->inUse = 1;

				node * newnode = (node *)(&memory[(current->next - current->capacity + sizeaskedfor)]);
				node data = {current->capacity - sizeaskedfor - sizeof(node) , 0, (current->next), (current->next - current->capacity - sizeof(node))};
				*newnode = data;

                node* tempnextnode = (node*)(&memory[(current->next)]);

				current->next = current->next - current->capacity + sizeaskedfor;
				current->capacity = sizeaskedfor;

                tempnextnode->prev = current->next;
				
				//printf("node: %d, %d, %d, %d\n", current->capacity, current->inUse, current->next, current->prev);

				current++;
				void * retnewnode = (void*)(current);
				//printf("malloc worked 2\n");
				
				return retnewnode;
			}
			printf("how'd you get here? no, seriously. how'd you get here?\n");
			break;
		}

		if (current->next != 0)
		{
			if (current->next >= maxsize)
			{
				break;
			}

			pass++;
			//usedcapacity = (usedcapacity + current->capacity +  sizeof(node));
            usedcapacity = current->next;
			// printf("in loop usedcapacity %d\n", usedcapacity);
            
			if (((short int)(memory[current->next])) == 0)
			{
				lastbefore0 = current->next - current-> capacity - sizeof(node);
				// printf("lastbefore0 on pass %d: %d\n", pass, lastbefore0);
			}
			current = (node *) (&memory[(current->next)]);
		}
		else
		{
			break;
		}
	}

	// printf("lastbefore0: %d\n", lastbefore0);

	// printf("in 0 space\n");

	//check capacity left in array and see if new node can be added
	if (sizeaskedfor > (maxsize - sizeof(node) - usedcapacity))
	{
		fprintf(stderr, "Too much space asked for\n");
		return NULL;
	}
	else
	{
		//check to make sure there will be enough space to make another node
		if ((maxsize - usedcapacity - sizeaskedfor - sizeof(node)) < sizeof(node))
		{
			sizeaskedfor = maxsize - usedcapacity - sizeof(node);
		}

		// printf("usedcapacity %d\n", usedcapacity);
		//create new node and set values. return void * pointer
		node * in0spacenode = (node *)(&memory[usedcapacity]);
		node datain0space = {sizeaskedfor, 1, usedcapacity + sizeof(node) + sizeaskedfor, lastbefore0};
		*in0spacenode = datain0space;

		//printf("node: %d, %d, %d, %d\n", in0spacenode->capacity, in0spacenode->inUse, in0spacenode->next, in0spacenode->prev);

		in0spacenode++;
		void * returning = (void*)(in0spacenode);

		// printf("returning: %p\n", returning);
		// printf("returning's next: %d\n", in0spacenode->next);
		//printf("malloc worked 3\n");

		return returning;
	}
	// printf("usedcapacity: %d and sizeaskedfor: %d\n", usedcapacity, sizeaskedfor);
	// printf("head next print:%d\n", head->next);
	return NULL;
}

//myfree
void myfree(void* ptr){
	//case void * to node *
    
    if (ptr == NULL)
    {
        fprintf(stderr,"NULL pointer\n");
        return;
    }

    

	node * check = (node *)ptr;
	check = check - 1;
	
	//printf("check: %d\n", check->next - check->capacity - sizeof(node));

	//start from head and check each beginning pointer to node * - 1
	node * current = head;
	short int validnode = 0;
    //printf("outside\n");

	while(current->next < maxsize && current->next >= 0)
	{
      //  printf("inside\n");
		if (current->next != 0)
		{
		//	printf("current: %d\n", current->next - current->capacity - sizeof(node));
		//	printf("current->next: %d\n", current->next);
			if (check == ((node*)(&memory[current->next - current->capacity - sizeof(node)])))
			{
				validnode = 1;
				break;
			}
			current = (node *) (&memory[(current->next)]);
		}
		else{
			//printf("break statement: current->next: %d\n", current->next);
			break;
		}
	}
    
    //printf("validnode %d\n", validnode);

	if (validnode == 1){
		check->inUse = 0;
		//printf("valid node\n");
	}
	else{
		fprintf(stderr, "not a valid pointer\n");
		return;
	}
    
	
	//check if next or prev have inUse = 0. if so, merge
    
    if (check->next > 0  && check->next < maxsize)
    {
        node* freesnextnode = (node*)(&memory[(check->next)]);

        if (freesnextnode->inUse == 0 && freesnextnode->capacity != 0)
        {
            //printf("in merge NEXT\n");
            merge(freesnextnode, check);
        }
    }

    if (check->prev != -1 && check->prev < maxsize && check->prev >= 0)
    {
        node * freeprevnode = (node*)(&memory[(check->prev)]);

        if (freeprevnode->inUse == 0 && freeprevnode->capacity != 0)
        {
            //printf("in merge PREV\n");
            merge(check, freeprevnode);
        }
    }
	/*
    
	//easier to do (if merge) next node first
	if (check->next < maxsize && check->next >= 0)
	{
		node* freesnextnode = (node*)(&memory[(check->next)]); 

		if (!(freesnextnode->capacity == 0))
		{
			if (check->next < maxsize)
			{
				if (freesnextnode->inUse == 0)
				{
					printf("in merge next\n");
					check->next = freesnextnode->next;
					check->capacity += (freesnextnode->capacity + sizeof(node));

					if (freesnextnode->next < maxsize)
					{
						node * freesnextnodeNEXT = (node *)(&memory[freesnextnode->next]);
						freesnextnodeNEXT->prev = check->next - check->capacity - sizeof(node);
					// printf("prev node: %d\n", freesnextnodeNEXT->prev);
					}

					char* charcasted = (char*)check;
					charcasted += sizeof(node);

					for(int todel = 0; todel < check->capacity; todel++)
					{
						*charcasted = 0;
						charcasted++;
					}
				}
			}
		}
	}

	//make function. not sure if we need to add it to .h file and how to keep it unavailable to memgrind
	if (check->prev < maxsize && check->prev >= 0)
	{
		node* freesprevnode = (node*)(&memory[(check->prev)]);
		if (check != (node*)(&memory[0]))
		{
			if (freesprevnode->inUse == 0)
			{	
				printf("in merge prev\n");

				freesprevnode->next = check->next;
				freesprevnode->capacity += (check->capacity + sizeof(node));
                
                node * checksnext;
                node checksnextdata = {-1,-1,-1,-1};
                *checksnext = checksnextdata;

				if (check->next < maxsize)
				{
					checksnext = (node*)(&memory[check->next]);
					checksnext->prev = freesprevnode->next - freesprevnode->capacity - sizeof(node);
					// printf("freesprevnode's\n");
				}

				printf("%d and %d\n", freesprevnode->next, checksnext->prev);

				char* charcasted2 = (char*)freesprevnode;
				charcasted2 += sizeof(node);

				for (int todel = 0; todel < freesprevnode->capacity; todel++)
				{
					*charcasted2 = 0;
					charcasted2++;
				}
			}
		}
	}
	// printf("made it to the bottom\n");

	//just need to make sure malloc doesnt go past maxsize YEEEEEEEEEEEE
	// make sure free doesn't go out of bounds (like if we get pointer for first node, prev points to itself, don't accidentally loop) and don't check past
	// maxsize
	//zero out freed memory
	//fprintf stderr errno
    printf("left free\n");
    */
}
