#include<stdio.h>
#include<limits.h>

static int size;
static float m = INT_MIN;
static int i;

void max(float []);

int main(void)
{
    int i;

    printf("Please enter the size of your list: ");

    while((scanf("%d",&size)!=1)||(size<1))
    {
        fflush(stdin);
        printf("\nInvalid input - Please enter a positive integer.\n\n");
    }

    float arr[size];

    printf("Please enter %d integers:\n", size);
    for(i=0; i<size; i++)
    {
        printf("\n%d) ",i+1);

        while(scanf("%f",&arr[i])!= 1)
        {
            fflush(stdin);
            printf("Invalid Input!\n%d)",i+1);
        }
    }

    max(arr);

    printf("The greatest number in the list is %.2f\n\n", m);

    return 0;
}


void max(float arr[size])
{
    while(i<size) //i initialised to 0
    {
        if(arr[i] > m)
        {
            m = arr[i];
        }

        i++;

        max(arr);
    }
}
