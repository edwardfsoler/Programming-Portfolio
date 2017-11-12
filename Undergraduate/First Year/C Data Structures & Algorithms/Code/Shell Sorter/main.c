#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define SIZE 16384

void shell(int *a, int n) ;

int main(void)
{
    int * arr;
    int i;
    arr = (int *)malloc(SIZE * sizeof(int)); //dynamic memory allocation
    srand(time(NULL)); //randomiser

    for(i=0; i<SIZE; i++)
    {
        *(arr+i) = rand();
    }

    shell(arr, SIZE);

    for(i=0; i<SIZE; i++)
    {
        printf("%d) %d\n", i+1, *(arr+i));
    }

    return 0;
}

void shell(int * arr, int size)
{
    int gap, skim, pos, val;

    for (gap = size; gap /= 2;)
    {
        for (skim = gap; skim < size; skim++)
        {
            val = arr[skim];

            for (pos = skim; (pos >= gap) && (val < arr[pos - gap]); pos -= gap)
            {
                arr[pos] = arr[pos - gap];
            }

            arr[pos] = val;
        }
    }
}
