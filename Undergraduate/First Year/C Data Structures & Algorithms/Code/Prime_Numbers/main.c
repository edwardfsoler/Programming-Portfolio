#include<stdio.h>
#include<stdbool.h>

bool primer(int number);

int main(void)
{
    int test;

    printf("Please enter a number:\n");
    while(scanf("%d", &test) != 1)
    {
        printf("\nInvalid entry - Please enter an integer\n");
        fflush(stdin);
    }

    if(test < 2)
    {
        printf("%d is not Prime\n", test);
        return 0;
    }

    if(primer(test))
    {
        printf("%d is prime!\n", test);
    }
    else
    {
        printf("%d is not prime!\n", test);
    }

    return 0;
}

bool primer(int number)
{
    int temp, i;

    temp = (number + 1)/2; //integer division ensures that temp is half (or half + 0.5) of number

    for(i = 2; i <= temp; i++)
    {
        if(number % i == 0)
        {
            return false;
        }
    }

    return true;
}
