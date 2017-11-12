#include<stdio.h>
#include<time.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "linked_list.h"

//------------------------------------------- DECLERATION OF STATIC VARIABLES ----------------------------------------------------
extern Node node;
extern priority_queue queue, q1, q2, q3;

//---------------------------------------------- LOCAL FUNCTION PROTOTYPES -------------------------------------------------------
bool dequeue_no_return(priority_queue * queue);
bool create_q_no_name(int max_size, char * qname, priority_queue * queue);

//--------------------------------------------------- LOCAL FUNCTIONS ------------------------------------------------------------
bool create_q_no_name(int max_size, char * qname, priority_queue * queue) //creates a queue without asking for user input for name
{
    strcpy(queue->qname, qname);

    queue->size = 0;
    queue->max_size = max_size;
    queue->head = queue->tail = NULL;

    return true;
}

bool dequeue_no_return(priority_queue * queue) //removes greatest priority element from the queue without outputting the data
{
    Node * tempn;

    if(is_empty(queue))
    {
        printf("Queue is empty\n\n");
        return false;
    }

    tempn = queue->head;        //point tempn to front of queue

    if(size(queue) == 1)
    {
        queue->head = queue->tail = NULL;   //point head to NULL as queue is empty
    }
    else
    {
        queue->head = queue->head->next;    //point head to next element in queue
    }

    queue->size--;
    free(tempn);            //free highest priority element in queue

    return true;
}

//------------------------------------------------ EXTERNAL FUNCTIONS -----------------------------------------------------------
priority_queue * create_q(int max_size)         //creates a new priority queue
{
    priority_queue * pq;
    pq = malloc(sizeof(priority_queue));        //allocate memory for new priority queue

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
    pq->head = pq->tail = NULL;

    return pq;
}

int size(const priority_queue * queue)      //returns number of elements currently in queue
{
    return queue->size;
}

bool is_empty(const priority_queue * queue) //returns true if queue is empty
{
    return queue->size == 0;
}

bool clear(priority_queue * queue)          //clears elements in queue
{
    while(!is_empty(queue))
    {
        dequeue_no_return(queue);
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

Node * dequeue(priority_queue * queue)      //removes highest priority element in queue and returns data
{
    Node * tnode, * temp;
    Node rnode;

    if(is_empty(queue))
    {
        printf("Queue is empty\n\n");
        return NULL;
    }
     tnode = queue->head;
     rnode = *tnode;             //copy data of highest priority element for return

    if(size(queue) == 1)
    {
        queue->head = queue->tail = NULL;   //point head to NULL and queue is empty
    }
    else
    {
        queue->head = queue->head->next;        //point head to next element
    }

    queue->size--;

    free(tnode);            //free highest priority element in queue

    temp = &rnode;

    return temp;            //return copy of data of highest priority element
}

priority_queue * merge(const priority_queue * q1, const priority_queue * q2) //merge contents of two queues into a new queue
{
    int newmax, i;
    newmax = i = 0;

    priority_queue newq;
    priority_queue * tqueue;        //memory allocation for new queue to be created

    Node * tempn;

    newmax = q1->max_size + q2->max_size;   //determine maximum size of new queue

    tqueue = create_q(newmax); //create queue to hold maximum of newmax elements
    newq = *tqueue;

    tempn = q1->head; //point tempn to head of q1

    for(i = 0; i < q1->size; i++) //N.B: If both queues are empty, the new queue will be empty but will have a max_size of newmax
    {
        enqueue(&newq, tempn, tempn->priority); //enqueue newq with elements of q1
        tempn = tempn->next;
    }

    tempn = q2->head; //point tempn to head of q2

    for(i = 0; i < q2->size; i++)
    {
        enqueue(&newq, tempn, tempn->priority); //enqueue newq with elements of q1
        tempn = tempn->next;
    }

    if(size(&newq) == (size(q1) + size(q2))) //test that all elements have been copied
    {
        tqueue = &newq;
        return tqueue;
    }

    printf("Merge operation failed\n\n");
    return NULL;
}

bool enqueue(priority_queue * queue, Node * node, double priority)  //enqueue elements in queue
{
    Node *prev, *pnew, *temp;
    int i = 0;

    prev = NULL;

    pnew = malloc(sizeof(Node));        //allocate memory for new node to be queued

    *pnew = *node;

    if(size(queue) == queue->max_size)  //catch if queue is full
    {
        printf("Queue is full\n\n");
        return false;
    }

    if(is_empty(queue))                 //catch if queue is empty
    {
        pnew->next = NULL;                  //as new node is final element
        queue->head = queue->tail = pnew;   //point to new node
        queue->size++;                      //increment queue size

        return true;
    }

    if(size(queue) == 1) //special case
    {
        if((queue->head->priority) >= priority) //if priorities are equal FCFS basis due to >=
        {
            pnew->next = NULL;              //as new node is final element
            queue->head->next = pnew;       //as new node is second node
            queue->tail = pnew;             //as new node is final element
            queue->size++;                  //increment queue size

            return true;
        }
        else
        {
            pnew->next = queue->head;       //as new node is first element
            queue->head = pnew;             //as new node is the head of the queue
            queue->size++;                  //increment queue size

            return true;
        }
    }

    //more than 1 element in queue

    if(pnew->priority > queue->head->priority)      //new node has highest priority
    {
        pnew->next = queue->head;
        queue->head = pnew;
        queue->size++;

        return true;
    }

    if(priority <= queue->tail->priority)            //new node has lowest priority
    {
        queue->tail->next = pnew;
        queue->tail = pnew;
        pnew->next = NULL;
        queue->size++;

        return true;
    }

    pnew->next = queue->head->next;         //make node 2nd element in queue
    queue->head->next = pnew;

    if(queue->size == 2) //by elimination if there are 3 elements and the new one is not the head or the tail - it is the middle
    {
        queue->size++;
        return true;
    }

    prev = queue->head;                     //for operation to come

    while(i < ((queue->size) - 1))          //traverse queue
    {
        if(priority <= pnew->next->priority)    //sorting algorithm
        {
            temp = pnew->next;
            pnew->next = temp->next;
            temp->next = pnew;
            prev->next = temp;
        }
        else
        {
            queue->size++;
            return true;
        }
        prev = prev->next;
        i++;                      //increment i
    }

    printf("Something went wrong - element not queued\n\n");

    return false;
}

Node * peek(const priority_queue * queue)   //return data of highest priority element without removing it from queue
{
    if(is_empty(queue))
    {
        printf("Queue is empty\n\n");
        return NULL;
    }

    return queue->head;         //highest priority element
}

bool store(priority_queue * queue, char * fname)    //write queue contents to file
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

    rewind(fp);         //rewind to start of file

    fwrite(queue, sizeof(priority_queue), 1, fp); //write queue details such as max_size and name to file

    tsize = queue->size;        //temporarily store queue size

    for(i = 0; i <tsize; i++)                   //traverse queue
    {
        temp = dequeue(queue);                  //dequeue nodes from queue for saving
        tempn = *temp;                          //required as dequeue returns a node pointer
        fwrite(&tempn, sizeof(Node), 1, fp);    //write each node in the list to file
    }

    if(fclose(fp) != 0)
    {
        printf("Unable to close file '%s'\n\n", fname);
        return false;
    }

    return true;
}

bool load(priority_queue * queue, char * fname)     //read queue contents from file
{
    FILE * fp;
    Node ptr;
    int i, temps;
    i = temps = 0;

    if((fp = fopen(fname, "rb")) == NULL) //attempt to open file in read binary mode
    {
        printf("Unable to open file '%s'\n\n", fname);
        return NULL;
    }

    rewind(fp);         //rewind to start of file

    fread(queue, sizeof(priority_queue), 1, fp);//read queue data from file

    temps = queue->size;                        //temporarily store queue size

    for(i = 0; i <temps; i++)                   //traverse queue
    {
        fread(&ptr, sizeof(Node), 1, fp);       //read individual nodes from file
        queue->size = i;                        //for enqueueing - simulates new nodes being added to a queue
        enqueue(queue, &ptr, ptr.priority);     //enqueue nodes in queue
    }

    queue->size = temps;                        //restore correct queue size

    if(fclose(fp) != 0)
    {
        printf("Unable to close file '%s'\n\n", fname);
        return false;;
    }

    return true;
}

void initialise_q(priority_queue * queue)       //initialise queue data
{
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = queue->max_size = 0;
    strcpy(queue->qname, "");
}

char * s_gets(char * st, int n)             //use for string input
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
		else
		{
			while (getchar() != '\n')
			{
				continue;
			}
		}
	}
	return ret_val;
}
