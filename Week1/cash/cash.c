#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    // Declare change variable, get a non-negative value from user 
    float change;
    do
    {
        change = get_float("Change owed: ");
    } 
    while (change < 0);

    // Get number of cents
    int cents = round(change * 100);
    
    // Initialize coins var to 0
    int coins = 0;

    // For each coin (25, 10, 5, 1) we will add 1 to coins and deduct the value of that coin from
    // cents, as long as cents is at least the value of that coin
    while (cents >= 25)
    {
        coins++;
        cents -= 25;
    }
    while (cents >= 10)
    {
        coins++;
        cents -= 10;
    }
    while (cents >= 5)
    {
        coins++;
        cents -= 5;
    }
    while (cents >= 1)
    {
        coins++;
        cents -= 1;
    }
    printf("%i\n", coins);
}
