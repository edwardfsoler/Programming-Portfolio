#ifndef CIRCULAR_BUFF_H_INCLUDED
#define CIRCULAR_BUFF_H_INCLUDED

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
                    } Node;

typedef struct Q{   char qname[MAXSIZE];
                    int size;
                    int max_size;
                    Node * head;
                    Node * tail;
                    Node * taken;
                    Node * free;
                }priority_queue;


/* operation: create a new empty priority queue with maximum number of elements max_size*/
/* postcondition: new queue with a maximum size of 'max_size' created if return is true */
/*                else queue creation failed */
priority_queue * create_q(int max_size);

/* operation: enqueue 'item' in priority queue 'queue' with priority 'priority'*/
/* precondition: Item 'item' and possibly empty or full queue 'queue' */
/* postcondition: placement of 'item' in 'queue' at point of priority if return true*/
/*                else failed to enqueue item*/
bool enqueue(priority_queue * queue, Node * node, double priority);

/* operation: dequeue element with highest priority from 'queue'*/
/* precondition: non-empty priority_queue 'queue' */
/* postcondition: return of pointer to highest priority 'item' in 'queue' and removal of 'item from 'queue'*/
/*                if 'queue' is emptied it is reset to 'empty' */
Node * dequeue(priority_queue * queue);

/* operation: return highest priority 'item' in 'queue' without removal*/
/* precondition: non empty priority queue 'queue'*/
/* postcondition: return of pointer to highest priority 'item' in 'queue'*/
/*                without removal of 'item' from queue */
Node * peek(const priority_queue * queue);

/* operation: returns true if 'queue' is empty*/
/* precondition: possibly empty queue 'queue' */
/* postcondition: returns true if 'queue' contains 0 elements*/
bool is_empty(const priority_queue * queue);

/* operation: returns the number of elements in 'queue'*/
/* precondition: possibly empty priority_queue 'queue' */
/* postcondition: returns the number of 'items' in 'queue'*/
int size(const priority_queue * queue);

/* operation: creates new priority_queue from elements in q1 and q2*/
/*            without altering their contents*/
/* precondition: two queues 'q1'and 'q2'*/
/* postcondition: return of pointer to priority_queue containing elements from 'q1' and 'q2'*/
/*                without effecting 'q1' and 'q2'*/
priority_queue * merge(const priority_queue * q1, const priority_queue * q2);

/* operation: remove all elements in priority_queue 'queue'*/
/* precondition: non-empty priority_queue 'queue' */
/* postcondition: priority_queue 'queue' is emptied*/
bool clear(priority_queue * queue);

/* operation: store priority_queue 'queue' in file 'f'*/
/* precondition: non-empty priority_queue 'queue'*/
/* postcondition: priority_queue 'queue' is stored in file 'f' */
bool store(priority_queue * queue, char * f);

/* operation: load priority_queue 'queue' from file 'f'*/
/* precondition: file 'f' containing priority_queue 'queue'*/
/* postcondition: priority_queue 'queue' is loaded from file 'f' */
bool load(priority_queue * queue, char * f);

/*operation: Read string input*/
char * s_gets(char * st, int n);

#endif // CIRCULAR_BUFF_H_INCLUDED
