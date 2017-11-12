#include<stdio.h>
#define SIZE 1024

void sieve(int * arr);

int main(void)
{
    int temp;

    printf("Please enter a number between 1 and %d:\n", SIZE);

    while((scanf("%d",&temp) != 1) || (temp <1) || (temp > SIZE))
    {
        fflush(stdin);
        printf("Invalid input - Please enter an integer between 1 and %d\n\n",SIZE);
    }

    if(temp == 1)
    {
        printf("%d is not a prime number!\n",temp);
        return 0;
    }

    if(temp == 2)
    {
        printf("%d is indeed a prime number!\n", temp);
        return 0;
    }

    int arr[1024] = {1}; //so as to initialise all elements except the first to 0

    sieve(arr);

    if(arr[temp-1] == 0)
    {
        printf("%d is indeed a prime number!\n", temp);
        return 0;
    }
    else
    {
        printf("%d is not a prime number!\n", temp);
    }

    return 0;
}

void sieve(int * arr)
{
    int i,j;

    for(i = 1; i < SIZE; i++)
    {
        if(arr[i] == 0)
        {
            for(j=2; j<=(SIZE/(i+1)); j++) //stopping condition ensures array boundaries are respected
            {
                arr[j*(i+1)-1] = 1; //replaces indexes which are multiples of i+1 or j with 1 (not prime)
            }
        }
    }
}
