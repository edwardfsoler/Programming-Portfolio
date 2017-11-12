#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include <stdbool.h>

#define MAXSIZE 40
#define SIZE 81

typedef struct node{    double priority;
                        char title[MAXSIZE];
                        char genre[MAXSIZE];
                        int length;
                        char release_date[MAXSIZE];
                        char director[MAXSIZE];
                        char actors[MAXSIZE];
                        struct node * next;
                    } Node;

typedef struct Q{   char qname[MAXSIZE];
                    int size;
                    int max_size;
                    Node * head;
                    Node * tail;
                }priority_queue;


/* operation: create a new empty priority queue with maximum number of elements max_size*/
/* postcondition: new queue with a maximum size of 'max_size' created*/
/*                returns NULL if creation fails*/
priority_queue * create_q(int max_size);

/* operation: enqueue 'node' in priority queue 'queue' with priority 'priority'*/
/* precondition: Node 'node' and possibly empty or non-empty queue 'queue' */
/* postcondition: placement of 'node' in 'queue' at point of priority if return true*/
/*                else failed to enqueue item*/
bool enqueue(priority_queue * queue, Node * node, double priority);

/* operation: dequeue element with highest priority from 'queue'*/
/* precondition: non-empty priority_queue 'queue' */
/* postcondition: return of pointer to highest priority 'node' in 'queue' and removal of node from 'queue'*/
/*                returns NULL if dequeue was unsuccessful (queue is empty) */
Node * dequeue(priority_queue * queue);

/* operation: return highest priority 'node' in 'queue' without removal*/
/* precondition: non-empty priority queue 'queue'*/
/* postcondition: return of pointer to highest priority 'node' in 'queue'*/
/*                without removal of 'node' - returns NULL if queue is empty */
Node * peek(const priority_queue * queue);

/* operation: returns true if 'queue' is empty*/
/* precondition: possibly empty queue 'queue' */
/* postcondition: returns true if 'queue' contains 0 elements, false otherwise*/
bool is_empty(const priority_queue * queue);

/* operation: returns the number of elements in 'queue'*/
/* precondition: possibly empty priority_queue 'queue' */
/* postcondition: returns the number of nodes in 'queue'*/
int size(const priority_queue * queue);

/* operation: creates new priority_queue from elements in queues q1 and q2*/
/*            without altering their contents*/
/* precondition: two queues 'q1' and 'q2'*/
/* postcondition: return of pointer to priority_queue containing elements from 'q1' and 'q2'*/
/*                without effecting 'q1' and 'q2'*/
priority_queue * merge(const priority_queue * q1, const priority_queue * q2);

/* operation: remove all elements in priority_queue 'queue'*/
/* precondition: non-empty priority_queue 'queue' */
/* postcondition: returns true if queue is successfully cleared, false otherwise*/
bool clear(priority_queue * queue);

/* operation: store priority_queue 'queue' in file 'f'*/
/* precondition: non-empty priority_queue 'queue'*/
/* postcondition: returns true if queue is successfully written to file, false otherwise*/
bool store(priority_queue * queue, char * f);

/* operation: load priority_queue 'queue' from file 'f'*/
/* precondition: file 'f' containing priority_queue 'queue'*/
/* postcondition: returns true if queue is successfully read from file, false otherwise*/
bool load(priority_queue * queue, char * f);

/*operation: Read string input*/
char * s_gets(char * st, int n);

#endif // LINKED_LIST_H_INCLUDED
