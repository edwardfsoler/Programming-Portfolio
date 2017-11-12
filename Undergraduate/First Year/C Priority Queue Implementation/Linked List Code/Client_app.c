#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include "linked_list.h"

//------------------------------------------------ LOCAL FUNCTION PROTOTYPES ----------------------------------------------------
int menu_fun(void);
void create_new_queue_fun(void);
void enqueue_queue_fun(void);
void dequeue_queue_fun(void);
void peek_fun(void);
void size_fun(void);
void merge_fun(void);
void truncate_fun(void);

//--------------------------------------------- DECLERATION OF STATIC VARIABLES -------------------------------------------------
priority_queue queue, q1, q2, q3;

Node node;

//------------------------------------------------------ MAIN FUNCTION -------------------------------------------------
int main(void)
{
    int choice = 0;
    bool quit = false;

    do
    {
        choice = menu_fun();                            //menu call

        switch(choice)
        {
        case 1:

           create_new_queue_fun();                      //queue creation function call

            break;
        case 2:

            enqueue_queue_fun();                        //queue enqueuement function call

            break;
        case 3:

           dequeue_queue_fun();                         //queue dequeuement function call

            break;
        case 4:

            peek_fun();                                 //peek element function call

            break;
        case 5:

            size_fun();                                 //size function call

            break;
        case 6:

            merge_fun();                                //merge function call

            break;
        case 7:

            truncate_fun();                             //truncate function call

            break;
        case 8:
                quit  = true;                           //quit program

            break;
        default:

                printf("Invalid input\n");

            break;
        }

    }while(!quit);

    return 0;
}

//-------------------------------------------------- LOCAL FUNCTIONS ----------------------------------------------------------

int menu_fun(void)                                                  //menu function
{
    int choice = 0;

        printf("                        ***** MENU *****\n\n");
        printf("1) Create new queue                   2) Enqueue element\n");
        printf("3) Dequeue highest rated element      4) View highest rated element\n");
        printf("5) Queue size                         6) Merge two queues\n");
        printf("7) Truncate queue                     8) Quit\n\n");

        while((scanf("%d", &choice) != 1) || (choice < 1) || (choice > 8))
        {
            fflush(stdin);
            printf("\nInvalid entry\n\n");
        }

        return choice;
}

void create_new_queue_fun(void)                                 //function to create new queue
{
    int tempcheck, size2;
    tempcheck = size2 = 0;

     printf("                   ***** CREATE NEW QUEUE *****\n\n");

            printf("Please enter the maximum number of elements\nto be stored in the queue to be created: \n\n");
            tempcheck = scanf("%d", &size2);


            while((tempcheck != 1) || (size2 < 1) || (size2 > 100))
            {
                fflush(stdin);
                printf("\nInvalid Input\n\n");
                tempcheck = scanf("%d", &size2);
            }

            priority_queue * qu;

            qu = create_q(size2);

            queue = *qu;

            printf("\nQueue successfully generated\n\n");

            if(!store(&queue, queue.qname))
            {
                printf("\nUnable to save '%s' to file\n\n",queue.qname);
            }
}

void enqueue_queue_fun(void)                                            //function to enqueue elements
{
    int test = 0;

    printf("                    ***** ENQUEUE ELEMENT *****\n\n");

            do
            {
                    fflush(stdin);
                    printf("Please enter the name of the queue you would like to enqueue:\n\n");
                    s_gets(queue.qname, SIZE);


            }while(queue.qname[0] == '\0');

           if(!load(&queue, queue.qname)) //no need for return as actual queue data is edited
            {
                  printf("Unable to load queue\n\n");
                  return;
            }

            if(size(&queue) == queue.max_size)
            {
                printf("Queue is full\n\n");
                return;
            }

            printf("We will now enqueue '%s' with elements\n\n",queue.qname);
            printf("Please enter the Movie Title: ");
            s_gets(node.title, SIZE);

            if(node.title[0] == '\0')
            {
                strcpy(node.title, "N/A");
            }

            printf("\nPlease enter the Movie Genre: ");
            s_gets(node.genre, SIZE);

            if(node.genre[0] == '\0')
            {
                strcpy(node.genre, "N/A");
            }

            printf("\nPlease enter the Movie Length: ");
            test = scanf("%d",&node.length);
            fflush(stdin);

            if(test == 0)
            {
                node.length = '~';
            }

            printf("\nPlease enter the Movie Release Date: ");
            s_gets(node.release_date, SIZE);

            if(node.release_date[0] == '\0')
            {
                strcpy(node.release_date, "N/A");
            }

            printf("\nPlease enter the Movie's director: ");
            s_gets(node.director, SIZE);

            if(node.director[0] == '\0')
            {
                strcpy(node.director, "N/A");
            }

            printf("\nPlease enter the Movie's Main Actors: ");
            s_gets(node.actors, SIZE);

            if(node.actors[0] == '\0')
            {
                strcpy(node.actors, "N/A");
            }

            printf("\nPlease enter the Movie Rating (0 - 10): ");

            while(((scanf("%lf",&node.priority)) != 1) || (node.priority < 0.0) || (node.priority > 10.0))
            {
                printf("\nInvalid input!\n\n");
                fflush(stdout);
            }

            if(enqueue(&queue, &node, node.priority))
            {
                printf("Element successfully queued!\n\n");

                if(!store(&queue, queue.qname))
                {
                    printf("Unable to store '%s' at the moment\n\n", queue.qname);
                }

                return;
            }

            printf("Element was not successfully enqueued\n\n");
}

void dequeue_queue_fun(void)                                        //function to dequeue elements
{
    Node * ptr;
     printf("                   ***** DEQUEUE ELEMENT *****\n\n");

            do
            {
                fflush(stdin);
                printf("Please enter the name of the queue you would like to dequeue:\n\n");
                s_gets(queue.qname, SIZE);

            }while(queue.qname[0] == '\0');

           if(!load(&queue, queue.qname))
           {
               return;
           }

           if(size(&queue) == 0)
           {
               printf("Queue is empty\n\n");
               return;
           }

            printf("We will now dequeue '%s'\n\n",queue.qname);

            ptr = dequeue(&queue);

            if(ptr == NULL)
            {
                return;
            }

            node = *ptr;

            printf("The following Movie has been removed from '%s': \n\n", queue.qname);
            printf("Title: %s\n", node.title);
            printf("Genre: %s\n", node.genre);
            printf("Director: %s\n", node.director);
            printf("Actors: %s\n", node.actors);
            printf("Length: %d\n", node.length);
            printf("Release date: %s\n", node.release_date);
            printf("Rating: %0.1lf\n\n", node.priority);

            if(!store(&queue, queue.qname))
            {
                printf("Unable to store '%s' to file\n\n", queue.qname);
            }
}

void peek_fun(void)                                                     //function returns highest priority element
{
    printf("                    ***** HIGHEST RATED ELEMENT *****\n\n");

           do
            {
                fflush(stdin);
                printf("Please enter the name of the queue you would like to operate upon:\n\n");
                s_gets(queue.qname, SIZE);

            }while(queue.qname[0] == '\0');

            if(!load(&queue, queue.qname))
            {
               printf("Unable to load queue\n\n");
               return;
            }

            Node * no;
            no = peek(&queue);

            if(no == NULL)
            {
                return;
            }

            node = *no;

            printf("The highest rated Movie in '%s' is:\n\n", queue.qname);
            printf("Title: %s\n", node.title);
            printf("Genre: %s\n", node.genre);
            printf("Director: %s\n", node.director);
            printf("Actors: %s\n", node.actors);
            printf("Length: %d\n", node.length);
            printf("Release date: %s\n", node.release_date);
            printf("Rating: %0.1lf\n\n", node.priority);
}

void size_fun(void)                                                          //function returns size of queue
{
    printf("                    ***** QUEUE SIZE *****\n\n");

            do
            {
                fflush(stdin);
                printf("Please enter the name of the queue you would like to operate upon:\n\n");
                s_gets(queue.qname, SIZE);

            }while(queue.qname[0] == '\0');

            if(!load(&queue, queue.qname))
            {
               printf("Unable to load queue\n\n");
               return;
            }

            printf("'%s' currently contains %d elements out of a maximum of %d\n\n", queue.qname, size(&queue), queue.max_size);
}

void merge_fun(void)                                                    //merge function
{
    printf("                    ***** QUEUE MERGE *****\n\n");

            do
            {
                fflush(stdin);
                printf("Please enter the name of the first queue that you would like to merge:\n\n");
                s_gets(q1.qname, SIZE);

            }while(q1.qname[0] == '\0');

            if(!load(&q1, q1.qname))
            {
                printf("Unable to load queue\n\n");
                return;
            }

            do
            {
                printf("Please enter the name of the queue you would like to operate upon:\n\n");
                s_gets(q2.qname, SIZE);

            }while(q2.qname[0] == '\0');

            if(!load(&q2, q2.qname))
           {
               printf("Unable to load queue\n\n");
               return;
           }

            priority_queue * qu2;
            qu2 = merge(&q1, &q2);
            q3 = *qu2;

            if(store(&q3, q3.qname))
            {
                printf("Merge successful\n\n");
                return;
            }

                printf("Unable to store '%s' to file\n\n", q3.qname);
}

void truncate_fun(void)                                                  //truncate function
{
    printf("                    ***** TRUNCATE QUEUE *****\n\n");

            do
            {
                fflush(stdin);
                printf("Please enter the name of the queue you would like to truncate:\n\n");
                s_gets(queue.qname, SIZE);

            }while(queue.qname[0] == '\0');

            if(!load(&queue, queue.qname))
            {
               printf("Unable to load queue\n\n");
               return;
            }

            if(clear(&queue))
            {
                printf("'%s' has successfully been truncated\n\n", queue.qname);
                if(!store(&queue, queue.qname))
                {
                    printf("Unable to store queue '%s' to file \n\n", queue.qname);
                }
                return;
            }

            printf("Clear unsuccessful\n");
}
