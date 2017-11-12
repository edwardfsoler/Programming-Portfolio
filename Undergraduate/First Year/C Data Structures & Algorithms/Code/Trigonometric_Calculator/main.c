#include<stdio.h>
#include<math.h>

float expansion_cos(float x, int terms);
float expansion_sin(float x, int terms);
long long factorial(int num);

int main(void)
{
    float x, answer;
    int terms, choice;

    printf("Please select the desired trigonometric function:\n");
    printf("1) cos(x)                2) sin(x)\n\n");

    while((scanf("%d",&choice) != 1) || (choice < 1) || (choice > 2))
    {
        fflush(stdin);
        printf("Incorrect input\n");
    }

    printf("Please enter the radian cosine angle: \n");
    while(scanf("%f", &x)!=1)
    {
        fflush(stdin);
        printf("\nInvalid input - Please enter a radian angle.\n\n");
    }

    printf("\nPlease enter the number of terms to which you would like to calculate: \n");
    while((scanf("%d", &terms) != 1) || (terms < 2))
    {
        fflush(stdin);
        printf("\nInvalid input - Please enter a positive integer greater than 1.\n\n");
    }

    if(terms>17)
    {
        terms = 17; //as negligible difference with excess terms
    }

    switch(choice)
    {
    case 1:
        answer = expansion_cos(x, terms);
        break;
    case 2:
        answer = expansion_sin(x,terms);
        break;
    default:
        printf("Error\n");
        break;
    }

    printf("\nThe series expansion of %s(%.2f) to %d terms is: %.3f\n", choice == 1? "cos":"sin",x,terms,answer);

    return 0;
}

long long factorial(int num)
{
    int i;
    long long ans = 1;

    for(i=num; i>0; i--)
    {
        ans*=i;
    }

    return ans;
}

float expansion_cos(float x, int terms)
{
    int i;
    float value = 0.0;

    for(i=0; i<terms; i++)
    {
        value += (pow(-1,i))*(pow(x,2*i))/factorial(2*i);
    }

    return value;
}

float expansion_sin(float x, int terms)
{
    int i;
    float value = 0.0;

    for(i=1; i<=terms; i++)
    {
        value += (pow(-1,i-1))*(pow(x,(2*i)-1))/factorial((2*i)-1);
    }

    return value;
}
