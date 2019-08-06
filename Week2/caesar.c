#include <cs50.h>
#include <ctype.h>  // isupper(), islower(), etc.
#include <stdio.h>  // printf()
#include <stdlib.h> // atoi()
#include <string.h> // strlen()

char rotate(char letter, int key);

int main(int argc, string argv[])
{
    // If exactly one argument is not provided, or if the argument provided
    // is not numeric, print usage instructions and exit
    if (argc != 2 || !isdigit(argv[1][0]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert the command line argument to integer
    int key = atoi(argv[1]);
    
    // Get plaintext string from user
    string plaintext = get_string("plaintext: ");

    printf("ciphertext: ");
    // Iterate over the length of plaintext, printing the rotated value of each letter
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        printf("%c", rotate(plaintext[i], key));   
    }
    printf("\n");
}

/**
 * Given a letter and an integer key, return a new letter derived from "rotating" the original letter
 * by n=key places. Preserve case and non-alphabetical characters. 
 */
char rotate(char letter, int key)
{
    if isalpha(letter)
    {
        // factor is the additive element that we need to convert between alphabetical index and ASCII value
        int factor;
        if (isupper(letter))
        {
            factor = 'A';
        }
        else
        {
            factor = 'a';
        }
        // We can get a letter's alphabetical index by subtracting `factor` from the integer value of letter
        // The formula (letter + key) % 26 gives us the new alphabetical index after rotation
        // Adding factor back again converts the new alphabetical index to the new ASCII value
        return (letter - factor + key) % 26 + factor;
    }
    else
    {
        return letter;
    }
}
