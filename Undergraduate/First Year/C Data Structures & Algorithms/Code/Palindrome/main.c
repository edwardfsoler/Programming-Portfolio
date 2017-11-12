#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define SIZE 25

int main(void)
{
    char str[SIZE];
    char temp[SIZE];
    int size, i;

    printf("Please input a string:\n");
    while(scanf("%s",str) != 1)
    {
        fflush(stdin);
        printf("Invalid Input - Please input a string");
    }

    size = strlen(str);

    if(size == 1)
    {
        printf("%s is not a palindrome!\n", str);
        return 0;
    }

    strlwr(str);

    for(i=0; i<size; i++)
    {
        temp[i] = str[size -(i+1)];
    }

    temp[size] = '\0'; //so as to disregard the \n

    if (strcmp(temp, str) == 0)
    {
        printf("%s is infact a palindrome!", str);
        return 0;
    }

    printf("%s is not a palindrome!", str);
    return 0;
}
