#include<stdio.h>
#define SIZE 15

int main(void)
{
    int num, calc;
    num = calc = 0;

    char *ints[] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    char *tens[] = {"X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    char *hunds[] = {"C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};

    printf("Please enter an integer between 1 and 1024: ");
    while((scanf("%d", &num) != 1) || (num < 1) || (num > 1024))
    {
        fflush(stdin);
        printf("\nInvalid Input! - Please enter an integer between 1 and 1024\n\n");
    }

    if(num >= 1000)
    {
        printf("%c", 'M');
        num -= 1000;
    }

    if(num >= 100)
    {
        calc = num/100;
        printf("%s",hunds[calc-1]); //due to integer division, remainder is disregarded
        num -= (calc*100);
    }

    if(num >= 10)
    {
        calc = num/10;
        printf("%s",tens[calc-1]);
        num -= (calc*10);
    }

    if(num >= 1)
    {
        printf("%s",ints[num-1]);
    }

    return 0;
}
