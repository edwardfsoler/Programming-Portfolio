#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define WIDTH 16
#define HEIGHT 16
#define BASE 10

void matrix(double mat1[WIDTH][HEIGHT], double mat2[WIDTH][HEIGHT], double temp[WIDTH][HEIGHT]);

int main(void)
{
    int i,j,k;
    double mat1[WIDTH][HEIGHT], mat2[WIDTH][HEIGHT], temp[WIDTH][HEIGHT];
    srand(time(NULL));

    for(i = 0; i <WIDTH; i++)
    {
        for(j = 0; j <HEIGHT; j++)
        {
            mat1[i][j] = (rand()%BASE) + 1; //populated matrices with random numbers between 1 and 10
            mat2[i][j] = (rand()%BASE) + 1;
            temp[i][j] = 0;
        }
    }

    for(i=0; i < HEIGHT; i++)
    {
        for(j=0; j < WIDTH; j++)
        {
            for(k=0; k < WIDTH; k++)
            {
                temp[j][i] += (mat1[k][i])*(mat2[j][k]);
            }
        }
    }

    for(i = 0; i <WIDTH; i++)
    {
        for(j = 0; j <HEIGHT; j++)
        {
           printf(" %.2f ", temp[i][j]);
        }
        printf("\n\n");
    }

    return 0;
}
