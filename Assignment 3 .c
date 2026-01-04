#include <stdio.h>

int main()
{
    int i, j;
    //ASSIGNMENT-3
    // Roof
    for (i = 0; i <= 5; i++)
    {
        for (j = 1; j <= 5 - i; j++)
            printf("  ");
        for (j = 1; j <= 2 * i +3; j++)
            printf(" *");
        printf("\n");
    }

    // Upper wall above window
    for (i = 1; i <=2; i++)
    {
        for (j = 1; j <= 2; j++)
            printf("  ");
        for (j = 1; j <=9; j++)
            printf(" +");
        printf("\n");
    }

    // Window
    for (i = 1; i <= 2; i++)
    {
        for (j = 1; j <= 2; j++)
            printf("  ");
        printf(" +");
        printf("    ");
        for(j=1;j<=6;j++)
            printf(" +");
        printf("\n");
    }
    for (j = 1; j <= 2; j++)
            printf("  ");
    for(j=1;j<=9;j++)
            printf(" +");
        printf("\n");
    // door part
    for (i = 1; i <= 9; i++){
        for (j = 1; j <= 2; j++)
            printf("  ");
        for(j=1;j<=3;j++)
            printf(" +");
        for(j=1;j<=3;j++)
            printf("  ");
         for(j=1;j<=3;j++)
            printf(" +");
        printf("\n");
    }
for (j = 1; j <= 2; j++)
            printf("  ");
    for(j=1;j<=9;j++)
            printf(" +");
        printf("\n");
    return 0;
}