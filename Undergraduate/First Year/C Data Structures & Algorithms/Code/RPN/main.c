#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>


typedef struct{ //create stack ADT
        int size;
        int max;
        float * list; //ensures exactly enough space for all integers
        } Stakk;

float POP(Stakk *stak);
bool PUSH(Stakk *stak, float entry);
void contents(Stakk *stak);

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        printf("Invalid Operation\n");
        return 1;
    }

    Stakk stak;  //declare variable of type Stakk

    stak.max = argc/2;
    stak.size = 0;
    stak.list = malloc(stak.max * sizeof(float));

    int i = 0;

    float temp1, temp2;
    temp1 = temp2 = 0.0;

    for(i=1; i<argc; i++)
    {
        if(atoi(argv[i]))
        {
            if(!PUSH(&stak,atoi(argv[i])))
            {
                printf("Unable to PUSH element on stack.\n\n");
            }
        }
        else
        {
            switch(*argv[i])
            {
            case '+':
                    temp2 = POP(&stak);
                    temp1 = POP(&stak);

                    if((temp2 == -999)||(temp1 == -999))
                    {
                        return 1;
                    }

                    PUSH(&stak, temp1+temp2);

                break;
            case '-':
                    temp2 = POP(&stak);
                    temp1 = POP(&stak);

                    if((temp2 == -999)||(temp1 == -999))
                    {
                        return 1;
                    }

                    PUSH(&stak, temp1-temp2);

                break;
            case '/':
                    temp2 = POP(&stak);
                    temp1 = POP(&stak);

                    if((temp2 == -999)||(temp1 == -999))
                    {
                        return 1;
                    }

                    PUSH(&stak, temp1/temp2);

                break;
            case 'x':
                    temp2 = POP(&stak);
                    temp1 = POP(&stak);

                    if((temp2 == -999)||(temp1 == -999))
                    {
                        return 1;
                    }

                    PUSH(&stak, temp1*temp2);

                break;
            default:
                printf("\"%s\" is an invalid operator\n\n", argv[i]);
                break;
            }
        }

        printf("\n");
        contents(&stak);
        printf("\n");
    }

    return 0;
}

float POP(Stakk *stak)
{
    if(stak->size < 1)
    {
        printf("Stack is empty.\n");
        return -999;
    }

    (stak->size)--;

    return (stak->list[stak->size]); //as size was already decremented
}

bool PUSH(Stakk *stak, float entry)
{
    if(stak->size == stak->max)
    {
        printf("Stack is full.\n");
        return false;
    }

    (stak->list[stak->size])= entry; //as stack size has not yet been incremented

    (stak->size)++;

    return true;
}

void contents(Stakk *stak)
{
    if(stak->size == 0)
    {
        printf("Stack is empty.\n");
        return;
    }

    int i;

    for(i=1; i<=(stak->size); i++)
    {
        printf(" |  %.2f  |\n",stak->list[(stak->size)-i]);
    }

}


