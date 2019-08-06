#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char rotate(char letter, int key);
int shift(char letter);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    int key_len = strlen(argv[1]);
    for (int i = 0; i < key_len; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Usage: ./vigenere keyword\n");
            return 1;
        }
    }

    string key = argv[1];
    
    string plaintext = get_string("plaintext: ");
    int n = strlen(plaintext);

    printf("ciphertext: ");
    // initialize i, the counter for the plaintext string
    // AND k, the counter for the key string
    for (int i = 0, k = 0; i < n; i++)
    {
        char letter = plaintext[i];

        // Get the character in the key that we're working with now
        // It's the counter k mod the length of the key (so that we don't go out of bounds)
        char k_i = key[k % key_len];

        // Print the letter rotated by the key
        printf("%c", rotate(letter, shift(k_i)));

        // Only if the letter is alphabetical do we want to increment the key counter
        // If it's space or punctuation or anything else, it should stay the same
        if (isalpha(letter))
        {
            k++;
        }
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

/**
 * Given a letter key, calculate the number of places to shift the plaintext.
 * Essentially convert the ASCII code to a case-insensitive alphabetical index
 * If the character is non-alphabetical, return 0 (no shift)
 */
int shift(char letter)
{
    if isalpha(letter)
    {
        return toupper(letter) - 'A';
    }
    else
    {
        return 0;
    }   
}
