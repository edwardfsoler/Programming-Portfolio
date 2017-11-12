#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "circular_buff.h"

//------------------------------------------- DECLERATION OF STATIC VARIABLES ----------------------------------------------------
extern Node *buff;
extern Node node;
extern priority_queue queue, q1, q2, q3;

//---------------------------------------------- LOCAL FUNCTION PROTOTYPES -------------------------------------------------------
bool dequeue_no_return(priority_queue * queue);
bool create_q_no_name(int max_size, char * qname, priority_queue * queue);

//--------------------------------------------------- LOCAL FUNCTIONS ------------------------------------------------------------
bool create_q_no_name(int max_size, char * qname, priority_queue * queue) //create queue with out asking user to input name
{
    strcpy(queue->qname, qname);

    queue->size = 0;
    queue->max_size = max_size;
    queue->head = queue->tail = NULL;

    return true;
}

bool dequeue_no_return(priority_queue * queue)      //remove highest priority element from queue without returning the data
{
    if(is_empty(queue))
    {
        printf("Queue is empty\n\n");
        return false;
    }

    if(size(queue) == 1)
    {
        queue->taken = NULL;    //point taken pointer to NULL as no indexes are taken
    }
    else
    {
        if(queue->taken == queue->tail)
        {
            queue->taken = queue->head;         //point taken pointer to head as end of queue has been reached
        }
        else
        {
            queue->taken = queue->taken + 1;    //point taken pointer to next element
        }
    }

    queue->size--;          //decrement queue size

    return true;
}

//------------------------------------------------ EXTERNAL FUNCTIONS -----------------------------------------------------------
priority_queue * create_q(int max_size)     //create new priority queue
{
    priority_queue * pq;
    pq = malloc(sizeof(priority_queue));    //memory allocation for queue to be created

    fflush(stdin);
    printf("Please enter the name of the queue that you would like to create:\n\n");
    s_gets(pq->qname, MAXSIZE);

    if(pq->qname[0] == '\0')
    {
        printf("Invalid input\n\n");
        s_gets(pq->qname, MAXSIZE);
        fflush(stdout);
    }

    pq->size = 0;
    pq->max_size = max_size;

    buff = malloc(sizeof(Node) * max_size); //allocation of memory to global array variable

    pq->head = &buff[0];                    //point head to first index in the array
    pq->tail = &buff[max_size - 1];         //point tail to last index in the array
    pq->taken = NULL;                       //initialise first taken index pointer to 0 as no indexes are taken
    pq->free = &buff[0];                    //point first empty index pointer to first index

    return pq;
}

int size(const priority_queue * queue)      //return number of elements in queue
{
    return queue->size;
}

bool is_empty(const priority_queue * queue) //return true if queue is empty
{
    return queue->size == 0;
}

bool clear(priority_queue * queue)          //clear queue contents
{
    while(!is_empty(queue))
    {
        dequeue_no_return(queue);           //call dequeue_no_return function to dequeue elements until empty
    }

    if(is_empty(queue))
    {
        return true;
    }
    else
    {
        return false;
    }
}

Node * dequeue(priority_queue * queue)      //remove element with highest priority from queue and return node data
{
    Node tnode;
    Node * rnode;

    if(is_empty(queue))
    {
        printf("Queue is empty\n\n");
        return NULL;
    }

    tnode = *(queue->taken);            //store contents of item at head of queue in tnode

    if(size(queue) == 1)
    {
        queue->taken = NULL;            //queue is now empty
    }
    else
    {
        if(queue->taken == queue->tail)
        {
            queue->taken = queue->head;         //if queue->taken points to the end of the array, direct it to start (circular motion)
        }
        else
        {
            queue->taken = queue->taken + 1;    //make the first taken pointer point to the next index
        }
    }

    queue->size--;      //decrement size

    rnode = &tnode;     //required as return is of type pointer and tnode is a local variable

    return rnode;
}

priority_queue * merge(const priority_queue * q1, const priority_queue * q2) //create new queue from contents of q1 and q2
{
    int newmax, i;
    newmax = i = 0;

    priority_queue newq;
    priority_queue * tqueue;            //create queue for new queue

    Node * tempn;

    newmax = q1->max_size + q2->max_size;   //compute the maximum for the new queue

    tqueue = create_q(newmax); //create queue to hold maximum of newmax elements
    newq = *tqueue;

    tempn = q1->taken; //point tempn to taken of q1

    for(i = 0; i < q1->size; i++) //N.B: If both queues are empty, the new queue will be empty but will have a max_size of newmax
    {
        enqueue(&newq, tempn, tempn->priority); //enqueue newq with elements of q1

        if(tempn == q1->tail)
        {
            tempn = q1->head;
        }
        else
        {
            tempn++;
        }

    }

    tempn = q2->taken; //point tempn to taken of q2

    for(i = 0; i < q2->size; i++)
    {
        enqueue(&newq, tempn, tempn->priority); //enqueue newq with elements of q2

        if(tempn == q2->tail)
        {
            tempn = q2->head;
        }
        else
        {
            tempn++;
        }
    }

    if(size(&newq) == (size(q1) + size(q2))) //test that all elements have been copied
    {
        tqueue = &newq;
        return tqueue;
    }

    printf("Merge operation failed\n\n");
    return NULL;
}

bool enqueue(priority_queue * queue, Node * node, double priority)  //enqueue node in queue
{
    int i = 0;
    bool change = false;
    Node cur, temp;
    Node * ptr1, * ptr2;

    if(size(queue) == queue->max_size)      //queue is full
    {
        printf("Queue is full\n\n");
        return false;
    }

    if(is_empty(queue)) //special case
    {
        queue->taken = queue->free;

        *(queue->free) = *node; //take copy of node data and store in location pointed to by free

        if(queue->free == queue->tail)
        {
            queue->free = queue->head;
        }
        else
        {
            queue->free = queue->free + 1; //point free pointer to next index;
        }

        queue->size++;

        return true;
    }
    else
    {
        *(queue->free) = *node; //take copy of node data and store in location pointed to by free

        if(queue->free == queue->tail)
        {
            queue->free = queue->head;
        }
        else
        {
            queue->free = queue->free + 1; // point queue->free to next free element
        }

        queue->size++;
    }

    //sort array priorities

    ptr1 = malloc(sizeof(Node));            //memory allocation for Node pointers
    ptr2 = malloc(sizeof(Node));            //to be used in sorting operation

    change  = false;

    ptr1 = queue->taken;                    //point ptr1 to element at front of queue

    if(queue->free == queue->head)
    {
        ptr2 = queue->tail;
    }
    else
    {
        ptr2 = queue->free - 1;
    }

    /* every time a node is enqueued in the queue it will be placed at the back of the queue
    therefore we shall sort the queue out each time a new node is added, therefore everytime
    a new node is added, any other nodes in the queue are already sorted, therefore once the
    new node is in place the queue is sorted and we may return*/

    for(i = 0; i < ((queue->size) - 1); i++)        //sorting algorithm
    {
        cur = *ptr1;    //take copy of data pointed to by ptr1 and store in cur
        temp = *ptr2;   //same with ptr2 and temp

        if((cur.priority < temp.priority) || ((cur.priority == temp.priority) && (temp.priority != node->priority))) //test priority of new node against priority of other nodes before it
        {
            *ptr2 = cur;
            *ptr1 = temp;
            change = true;
        }
        else
        {
            if(change)
            {
                break; //if there was a change made, and now there isn't, the queue must be sorted therefore break
            }
        }

        if(ptr1 == queue->tail)
        {
            ptr1 = queue->head;
        }
        else
        {
            ptr1++;
        }
    }

    free(ptr1);         //free previously allocated memory

    ptr2 = queue->taken;    //for testing purposes

    for(i = 0; i < ((queue->size) - 1); i++)        //test for sorting success
    {
        if(ptr2->priority < (ptr2+1)->priority)
        {
            printf("Something went wrong - element not queued\n\n");
            return false;
        }
        ptr2++; //traverse list
    }

    free(ptr2); //free previously allocated memory

    return true;
}

Node * peek(const priority_queue * queue)   //return data of highest priority element without removing it from queue
{
    if(is_empty(queue))
    {
        printf("Queue is empty\n\n");
        return NULL;
    }

    return queue->taken;
}

bool store(priority_queue * queue, char * fname)    //write queue data to file
{
    FILE * fp;
    Node tempn;
    Node * temp;
    int i, tsize;
    i = tsize = 0;

    if((fp = fopen(fname, "w+b")) == NULL) //attempt to open 'fname' in write plus binary mode
    {
        printf("Unable to open file '%s'\n\n", fname);
        return false;
    }

    rewind(fp); //rewind file to the beginning

    fwrite(queue, sizeof(priority_queue), 1, fp);   //write queue data such as name and size to file

    tsize = queue->size;            //temporarily store the queue size

    for(i = 0; i <tsize; i++)       //sequentially read in node data
    {
        temp = dequeue(queue);                      //dequeue highest priority element from list
        tempn = *temp;                              //required as dequeue returns a node pointer
        fwrite(&tempn, sizeof(Node), 1, fp);        //write highest priority element to file
    }

    if(fclose(fp) != 0)
    {
        printf("Unable to close file '%s'\n\n", fname);
        return false;
    }

    return true;
}

bool load(priority_queue * queue, char * fname)     //read queue data from file
{
    FILE * fp;
    Node * ptr;
    int i, temps, highest, num;
    i = temps = highest = num = 0;

    if((fp = fopen(fname, "rb")) == NULL) //attempt to open file in read binary mode
    {
        printf("Unable to open file '%s'\n\n", fname);
        return NULL;
    }

    rewind(fp); //rewind file to the beginning

    fread(queue, sizeof(priority_queue), 1, fp);        //read queue data from file

    temps = queue->size;        //temporarily store queue size

    buff = malloc(sizeof(Node) * queue->max_size);      //allocate memory for previously declared global array

    fread(buff, sizeof(Node), queue->max_size, fp);     //read max_size nodes from memory in node sized chunks and write to buff array

    queue->head = &buff[0];                             //point head of queue to first index in array
    queue->tail = &buff[queue->max_size - 1];           //point tail to last index in array

    ptr = queue->head;

    highest = 0;

    for(i = 0; i < queue->max_size; i++)                //algorithm determines which node the taken pointer must point too
    {
        if(ptr != NULL)
        {
            if(ptr->priority > highest)
            {
                highest = ptr->priority;                //node with highest priority is found
                num = i;
            }
        }
        ptr = ptr+1;
    }

    queue->taken = &buff[num];      //node with highest priority is in the front of the queue and is therefore pointed to by taken

    queue->free = queue->taken;

    for(i = 0; i < temps; i++)
    {
            if(queue->free == queue->tail)
            {
                queue->free = queue->head;
            }
            else
            {
                queue->free = queue->free + 1;
            }
    }

    queue->size = temps;        //queue size is restored

    if(fclose(fp) != 0)
    {
        printf("Unable to close file '%s'\n\n", fname);
        return false;;
    }

    return true;
}

char * s_gets(char * st, int n)     //implemented for string input
{
	char * ret_val;
	int i = 0;

	ret_val = fgets(st, n, stdin);

	if (ret_val)
	{
		while (st[i] != '\n' && st[i] != '\0')
		{
			i++;
		}

		if (st[i] == '\n')
		{
			st[i] = '\0';
		}
		else // must have words[i] == '\0'
		{
			while (getchar() != '\n')
			{
				continue;
			}
		}
	}
	return ret_val;
}
