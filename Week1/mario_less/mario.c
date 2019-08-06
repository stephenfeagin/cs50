#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    
    for (int i = 0; i < height; i++)
    {
        // Print spaces up to the difference between the height and the row number
        for (int k = 0; k < height - i - 1; k++)
        {
            printf(" ");
        }
        // Print hashes
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }
        // End of row, print newline
        printf("\n");
    }
}

