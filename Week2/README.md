# Lecture 2

## Code Compilation

When compiling a program using `cs50.h`, the sandbox `make` knows all of the things to do. But 
`clang` or `gcc` doesn't know about the `cs50` library. We have to tell it to link it in, using
the command line argument `-lcs`. If we want to use the math library, we need to include `-lm`. The
`-l` in these examples means "link." These arguments have to go at the end of the various command 
line options.

### What "Compilation" Really Means

Four steps:

1. Preprocessing
2. Compiling
3. Assembling
4. Linking

#### Preprocessing

The `#include <stdio.h>` at the top of the file is a *preprocessor directive*, telling `clang` that
these things need to be dealt with first. These files declare variables and functions so that 
`clang` will know how to compile our program with those variables and functions (their data types
in particular).

Preprocessing is essentially replacing the `#include` lines with all of the declarations contained
within that header file, like a big search-and-replace.

#### Compiling

Technically speaking, *compiling* is the step that turns our C source code into Assembly code. These
are the lower-level instructions that your CPU can actually understand. Assembly code is the 
low-level very basic instructions, like `push` and `move` and `call`.

#### Assembling

Our CPUs can work with assembly code, but computers really only understand machine code -- binary.
*Assembling* is the process of turning assembly instructions into machine code.

#### Linking

At the end of the process, we need to plug in the machine code that other people have written and
that we are using via the `#include` instruction. `cs50.c` and `printf.c` are already stored on our
computers, along with their machine code representations, and those binary files get *linked* onto
our own file that we just wrote.

## Arrays

Our computers store information that we are using *right now* (as opposed to long-term storage) in
*RAM* (Random Access Memory). We can imagine RAM as a long collection of individual bytes in which
information can be stored. A `char` in C is normally 1B, so it would take up one of those blocks. An
`int` is normally 4B, so it would take up four of those blocks. We refer to blocks that are all
directly connected to each other as *contiguous memory* or an *array*.

The ability to store objects in contiguous memory opens up a lot of possibilities for improving 
program design and performance. `int`s are stored in four contiguous bytes -- if they were stored in
4 non-contiguous bytes, it would be much slower. Arrays also make it easier to design our programs
in ways that are easier to understand.

### Example: scores

#### Without arrays

```c
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int score1 = get_int("Score 1: ");
    int score2 = get_int("Score 2: ");
    int score3 = get_int("Score 3: ");

    for (int i = 0; i < score1; i++)
    {
        printf("#");
    }
    printf("\n");

    for (int i = 0; i < score2; i++)
    {
        printf("#");
    }
    printf("\n");

    for (int i = 0; i < score2; i++)
    {
        printf("#");
    }
    printf("\n");
}
```

#### Using a function

```c
#include <cs50.h>
#include <stdio.h>

void chart(int score);

int main(void)
{
    int score1 = get_int("Score 1: ");
    int score2 = get_int("Score 2: ");
    int score3 = get_int("Score 3: ");

    printf("Score 1: ");
    chart(score1);

    printf("Score 2: ");
    chart(score2);

    printf("Score 3: ");
    chart(score3);
}

void chart(int score)
{
    for (int i = 0; i < score; i++)
    {
        printf("#");
    }
    printf("\n");
}
```

#### Using an array

```c
#include <cs50.h>
#include <stdio.h>

void chart(int score);

int main(void)
{
    // Get scores from user
    int scores[3];
    for (int i = 0; i < 3; i++)
    {
        scores[i] = get_int("Score %i: ", i+1);
    }

    // Chart scores
    for (int i = 0; i < 3; i++)
    {
        printf("Score %i: ", i+1);
        chart(scores[i]);
    }
}

void chart(int score)
{
    for (int i = 0; i < score; i++)
    {
        printf("#");
    }
    printf("\n");
}
```

But the above solution isn't perfect yet -- it still uses the *magic number* `3` in several
places. A better way would be to declare a global variable so that you can change it in only
one place if you need to. We can even tell C that it's a value that should never change, which
will help the compiler enforce that.

#### Using a constant

```c
#include <cs50.h>
#include <stdio.h>

const int COUNT = 3;

void chart(int score);

int main(void)
{
    // Get scores from user
    int scores[COUNT];
    for (int i = 0; i < COUNT; i++)
    {
        scores[i] = get_int("Score %i: ", i+1);
    }

    // Chart scores
    for (int i = 0; i < COUNT; i++)
    {
        printf("Score %i: ", i+1);
        chart(scores[i]);
    }
}

void chart(int score)
{
    for (int i = 0; i < score; i++)
    {
        printf("#");
    }
    printf("\n");
}
```

The convention is to use `CAPS` for global variables, just to indicate that it has global scope and
is not local to any particular subroutine.

#### Even better

```c
#include <cs50.h>
#include <math.h>
#include <stdio.h>

const int COUNT = 3;

void chart(int count, int scores[]);

int main(void)
{
    // Get scores from user
    int scores[COUNT];
    for (int i = 0; i < COUNT; i++)
    {
        scores[i] = get_int("Score %i: ", i+1);
    }

    // Chart scores
    chart(COUNT, scores);
}

// Uses an array of scores, of which we don't specify the length
void chart(int count, int scores[])
{
    // Output one hash per point
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < scores[i]; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
```

### Example: strings

Strings are actually just arrays of `char`s. You can index into it to get characters in exactly
the same way that we index into an array to get its elements. We can write a simple program to print
out the characters in a string, one by one.

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("Input: ");
    printf("Output:\n");
    for (int i = 0; i < strlen(s); i++)
    {
        printf("%c\n", s[i]);
    }
}
```

The above program has a slight inefficiency: it's calculating `strlen(s)` on every iteration. We can
improve the design by creating a variable to store the result:

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("Input: ");
    printf("Output:\n");
    int n = strlen(s);
    for (int i = 0; i < n; i++)
    {
        printf("%c\n", s[i]);
    }
}
```

It probably doesn't save too much time, but it's definitely better design to cache the result of a
calculation that will be repeated over and over without the answer changing.

The program can even be cleaned up a little bit more, by moving the calculation of `n` into the loop
initialization.

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("Input: ");
    printf("Output:\n");
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        printf("%c\n", s[i]);
    }
}
```

That's mostly a stylistic change, but it does scope `n` more tightly into just the loop, rather than
the entire `main()` function.

Arrays are stored internally as contiguous blocks of memory, and the variable in C points just to
the beginning block. It has no knowledge of the end of the variable's data by itself. So what it
does is to add a NULL character `\0` at the end of the string. This character is simply a completely
zero byte, all eight bits are 0. So the string `stephen` would not be 7 bytes, it would be 8 (one
for each letter, plus one for the null character).

We could even implement `strlen` ourselves, knowing that `\0` terminates the string:

```c
int strlen(char s[])
{
    int n = 0;
    while (s[n] != '\0')
    {
        n++;
    }
    return n;
}
```

## Type Conversion

Characters are represented internally as integers, per the ASCII standard. We can convert
characters to ints and back:

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("String: ");
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        int c = (int) s[i];
        printf("%c %i\n", s[i], c);
    }
}
```

We're *casting* the `char` at `s[i]` to an `int` by using the notation `int c = (int) s[i]`. We 
could even do this implicitly by writing `int c = s[i]`. Or even more implicitly by getting rid of
`c` altogether, and writing `printf("%c %i\n", s[i], s[i])`. The compiler knows how to print that
byte represented as a character with `%c` and as an integer with `%i`.

We can leverage our knowledge of strings to implement a function to capitalize a string.

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("Before: ");
    printf("After: ");
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            printf("%c", s[i] - ('a' - 'A'));
        }
        else
        {
            printf("%c", s[i]);
        }
    }
}
```

There are a couple weird parts. We're comparing if `s[i]` is *greater than or equal to* `'a'` and
*less than or equal to* `'z'`. This is the programmatic way of determining if the character is
lowercase. If it is, we want to capitalize that. We do that by subtracting off the difference
between `'a' and 'A'` -- the capital letters are that difference less than the lower case letters
in ASCII representation. Uppercase letters start with `'A'` at 65, and lowercase letters start with
`'a'` at 97.

However, there are built-in functions like `islower()`, `isupper()`, and `toupper()` in the library
`ctype.h`.

## Command Line Arguments

We're used to `int main(void)`. But we can have `main()` accept command line arguments by changing
its function signature to `int main(int argc, string argv[])`. `argc` is the number of arguments,
and `argv` is the *vector* of arguments.

### Example: hello

```c
#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        printf("hello, %s\n", argv[1]);
    }
    else
    {
        printf("hello, world\n");
    }
}
```

### Example: print all arguments

```c
#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    for (int i = 0; i < argc; i++)
    {
        printf("%s\n", argv[i]);
    }
}
```

### Example: print all characters

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Iterate over strings in argv
    for (int i = 0; i < argc; i++)
    {
        // Iterate over chars in argv[i]
        for (int j = 0, n = strlen(argv[i]); j < n; j++)
        {
            printf("%c\n", argv[i][j]);
        }
    }
}
```

This shows that an array of strings is an array of arrays of characters. We can index into arrays of
arrays repeatedly.

## Cryptography

This all has practical applications in cryptography. We can implement algorithms to convert 
*plaintext* into *ciphertext* and back by manipulating the characters of the message. These
algorithms use a *key* or a *secret* as a second input to the algorithm.

### Caesar Cypher

Rotational cypher based on a key that increments each character by that key.

## Exit Codes

In addition to printing error statements, we can return *exit codes* to signal whether a process
succeeded or failed. On the screen, we only see the messages by default. However, if after running a
process we run the command `echo $?`, we will be given the exit code of that process. It is
convention that the program returns a non-zero error code if anything goes wrong, otherwise it will
return 0.

## Other Applications

Computers can only look at a single byte or value at a time. It can't step back and look at an
entire array all at once. For searches, if we have absolutely no information about the order of the
elements, we can't do any better than random guessing. But if we know about the ordering of the
array, we can implement much more efficient algorithms to find the element we're looking for. But
that's information that *we* need to know, the computer doesn't know that. That means that we need
to work on ways to efficiently sort arrays so that we can use the search algorithms.

### Bubble Sort

Going through the array, do comparisons of every element pair. If element `i` is larger than element
`i+1`, switch them. At the end of one iteration through the array, the largest element will be in
the highest index position. So the next iteration will proceed from the beginning of the array to
the next highest index, doing pairwise comparisons of every element.

```
repeat until no swaps
    for i from 0 to n-2
        if ith and i+1th elements are out of order
            swap them
```

### Selection Sort

Select the smallest element in the array. Place that element in index 0, and move the element that
was at index 0 to the initial location of that smallest element. Repeat beginning at index 1, then
2, etc.

```
for i from 0 to n-1
    find the smallest element between index i and index n-1
    swap the smallest element with the ith element
```

### Algorithmic Efficiency

*On average*, bubble sort and selection sort will take the same number of steps given the
same array.

Bubble sort will take (n-1) + (n-2) + ... + 1 steps. This series simplifies to n(n-1)/2. We
would describe the number of steps required for this algorithm as O(n^2), on the order of
n-squared. The biggest exponent of n is the dominating factor, which is what we use as the 
order.

### Merge Sort

We can do better than O(n^2) if we allow ourselves to use multiple arrays. We divide the array
iteratively until we have a sub-array of only one element (index 0). That sub-array is by
definition sorted. We repeat that with the element at index 1, which is again sorted by
definition. By using some extra memory, we can combine those two elements into a separate
array, but in order. We repeat that again, getting another sub-array of length 2 where we know
that the elements are sorted. We know that the smaller of the index 0 elements is the first
element in their new merged array. This is hard to explain verbally without an example. But it
works out to O(n log(n)).

# Video Shorts

## Algorithm Overview

O() notation refers to the worst-case run time, &Omega;() refers to the best-case run time.

### Selection Sort

*Find the **smallest** unsorted element in an array and swap it with the **first** unsorted
element in that array*. O(n^2), &Omega;(n^2).

### Bubble Sort

*Swap **adjacent pairs** of elements if they are out of order, effectively "bubbling" larger
elements to the right and smaller ones to the left.* O(n^2), &Omega;(n).

### Insertion Sort

*Proceed once through the array from left to right, **shifting** elements as necessary to
insert each element into its correct place*. O(n^2), &Omega;(n).

### Merge Sort

***Split** the full array into subarrays, them **merge** those subarrays back together in the
correct order*. O(n log n), &Omega;(n log n).

### Linear Search

***Iterate** across the array from left to right, trying to find the target element.** O(n), 
&Omega;(1).

### Binary Search

*Given a **sorted** array, **divide and conquer** by systematically eliminating half of the
remaining elements in the search for the target element.* O(log n), &Omega;(1).

## Arrays

- Arrays hold values of the same type at contiguous memory locations
- Partitioned into small, identically-sized blocks of space called *elements*
- Each element can store a certain amount of data
- All elements have the same data type
- Elements can be accessed directly by *index*

### Indexing

- Elements of an array are indexed starting from 0
- If an array consists of n elements, the first element is located at index 0 and the last element
is located at index n-1
- C is very lenient, and will not prevent you from accessing memory that is outside the bounds of
the array

### Declaration

- `type name[size];`
- The `type` is what kind of variable each element of the array will be
- `name` is the name of the variable
- `size` is the number of elements you want the array to contain

### Initialization

- We can declare an array and provide its values all at the same time
- `bool truthtable[3] = { false, true, true };`
- `int scores[4] = { 100, 72, 89, 92 };`
- You actually don't need to specify the number of elements if you're using this instantiation
syntax
- `bool truthtable[] = { false, true, true };`

### Multi-Dimensional Arrays

- Arrays can have more than one dimension
- You can have as many size specifiers as you need
- `bool battleship[10][10]` would give you something analagous to a 10x10 grid
- Remember, though, that in memory it's actually just a 100-element single-dimensional array
- It's an abstraction to help represent grids and other complex structures

### Working with Arrays

- We can treat individual elements of arrays like variables
- But we cannot treat entire arrays themselves as variables
    + For instance, we cannot assign one array to another using the `=` assignment operator
    + That would not be legal C code
- Instead, we have to loop over the array to copy it one element at a time
- While most variables in C are passed by value, arrays are *passed by reference*
- That is, the callee receives the actual array (or a pointer to the array), not a copy of it
- Thus, if the callee manipulates the elements of the array, the original array keeps those changes

## Selection Sort

The idea is to find the smallest unsorted element and add it to the end of the sorted list. This
requires dividing the original array into sorted and unsorted segments, and keeping track of the
boundary between those segments.

### Pseudocode

```
while there is least 1 element in the unsorted portion:
    search the unsorted portion to find the smallest value
    swap the smallest value with the first element in the unsorted part
```

### Example

- Original array: `5 2 1 3 6 4`
- The entire array is unsorted at this point
- The sorted portion has 0 elements, the unsorted portion has 6 elements
- The first index of the unsorted portion is 0
- We search through the unsorted part for the smallest value
    + At index 0, `5` is the smallest value
    + At index 1, `2 < 5` so 2 is the smallest value
    + At index 2, `1 < 2`, so 1 is the smallest value
    + At index 3, `1 < 3`, so 1 is still the smallest value
    + At index 4, `1 < 6`, so 1 is still the smallest value
    + At index 5, `1 < 4`, so 1 is still the smallest value
- We then swap that smallest value that we found with the element at the first index of the unsorted
portion (index 0, `5`)
- After one iteration, we now have `1 2 5 3 6 4`
- The sorted portion has 1 element, index 0, and the unsorted portion has 5 elements
- The first index of the unsorted portion is 1
- We repeat our search through the unsorted portion, and find that `2` is the smallest element
- We swap it with the element at index 1, which is `2`, so the array remains unchanged
- After two iterations, we have `1 2 5 3 6 4`
- The array hasn't changed, but the starting index of the unsorted portion is now 2
- Iteration 3: 3 is the smallest element, swaps with the element at index 2, we now have
`1 2 3 5 6 4`
- Iteration 4: 4 is the smallest element, swaps with the element at index 3, we now have
`1 2 3 4 6 5`
- Iteration 5: 5 is the smallest element, swaps with the element at index 4, we now have
`1 2 3 4 5 6`
- The length of the unsorted portion is now 1, containing only index 5, and so is sorted by
definition
- We are done

### Efficiency

- **Worst-Case Scenario**: We have to iterate over each of the *n* elements of the array to find the
smallest unsorted element, and we must repeat this process *n* times, since only one element gets
sorted on each iteration. O(n^2).
- **Best-Case Scenario**: Exactly the same, because there is no way to guarantee that the array is
sorted until we've gone through the entire process for all elements. &Omega;(n^2).
