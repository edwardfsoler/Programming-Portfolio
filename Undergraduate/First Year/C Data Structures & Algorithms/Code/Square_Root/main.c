#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define SEED 1
#define ACCURACY 0.0001

float rooter(float num);
int seeder(float num, int n);

int main(void)
{
    float num, answer;

    printf("Please enter a positive number whose square root you would like to find:\n");
    while((scanf("%f", &num) != 1) || (num <=0))
    {
        fflush(stdin);
        printf("Invalid input. Please enter a positive integer/float.\n\n");
    }

    answer = rooter(num);

    printf("The square root of %.4f is: %.6f\n\n",num,answer);

    return 0;
}

float rooter(float num)
{
    float seed, answer, diff;
    answer = 0.0;
    int tseed;

    tseed = seeder(num,SEED);

    if(tseed % 2 == 0) //mathematical manner of finding rough estimate for square root
    {
        seed = 6*(pow(10,((tseed-2)/2)));
    }
    else
    {
        seed = 2*(pow(10,((tseed-1)/2)));
    }

    while((diff>ACCURACY)||(diff<-ACCURACY))
    {
        answer = (0.5*(seed + (num/seed)));
        diff = seed-answer;
        seed = answer;
    }

    return answer;
}

int seeder(float num, int n)
{
    while((num/=10)>10)
    {
        n++;
    }

    return ++n;
}
