#include <stdio.h>

// Recursive function to count number of ways to multiply n matrices
long long countWays(int n)
{
    if (n == 0 || n == 1)
        return 1;

    long long ways = 0;

    for (int i = 0; i < n; i++)
    {
        ways += countWays(i) * countWays(n - 1 - i);
    }

    return ways;
}

int main()
{
    int n;
    printf("---------ASSIGNMENT-2 ------------\n\n\t");
    printf("Enter number of matrices: ");
    scanf("%d", &n);

    printf("Number of ways to multiply %d matrices = %lld\n",
           n, countWays(n - 1));

    return 0;
}