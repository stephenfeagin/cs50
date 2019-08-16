#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int collatz(int n);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./collatz INT\n");
        return 1;
    }

    int n = atoi(argv[1]);
    if (n == 0)
    {
        printf("Usage: ./collatz INT\n");
        return 1;
    }
    printf("%i\n", collatz(n));
}

int collatz(int n)
{
    // base case
    if (n == 1)
        return 0;
    // recursion case: even numbers
    else if (n % 2 == 0)
        return 1 + collatz(n/2);
    // recursion case: odd numbers
    else
        return 1 + collatz(3*n + 1);
}