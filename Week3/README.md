# Lecture 3

Getting input from users can be pretty tricky without the CS50 functions like
`get_char`, `get_int`, etc. `get_string` is particularly tricky because C does
not actually have a built-in `string` type. This can cause some issues. For
instance, comparing strings doesn't work as expected -- they always come out as
unequal if you just use `s1 == s2`. `string` was created as a new data type in
`libcs50`. `string` is just a character array `char[]`, and we know that arrays
in C are passed by reference, not by value -- copies aren't made to pass around
to functions, their addresses in memory are passed around. So when we try to
compare strings, we're actually comparing memory addresses, and those are NOT
the same even for strings that contain the same characters. Functions can only
return a single value, and that value is a memory address for arrays. For
primitive types, like ints, chars, bools, etc., the actual value is returned.
This is because the value has a prescribed size, the compiler can know exactly
how much memory those data types use and where in memory those values are
stored.

## Example String Comparison

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool compare_strings(string a, string b);

int main(void)
{
    string s = get_string("s: ");
    string t = get_string("t: ");
    if (compare_strings(s, t))
    {
        printf("same\n");
    }
    else
    {
        printf("different\n");
    }
}

bool compare_strings(string a, string b)
{
    int a_len = strlen(a);
    int b_len = strlen(b);

    if (a_len != b_len)
    {
        return false;
    }

    for (int i = 0; i < a_len; i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }

    return true;
}
```

## There is No String

`string` is not a built-in data type. In its implementation in `libcs50`, it's a
synonym for `char *` -- the address of a `char`. This is called a pointer. A
variable of type `char *` tells C that I don't want to store a `char`, I want to
store a *pointer to* some `char`. It's actually just the address of the first
byte of a `char` array.

### Example string comparison sans `string`

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool compare_strings(char *a, char *b);

int main(void)
{
    char *s = get_string("s: ");
    char *t = get_string("t: ");
    if (compare_strings(s, t))
    {
        printf("same\n");
    }
    else
    {
        printf("different\n");
    }
}

bool compare_strings(char *a, char *b)
{
    int a_len = strlen(a);
    int b_len = strlen(b);

    if (a_len != b_len)
    {
        return false;
    }

    for (int i = 0; i < a_len; i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }

    return true;
}
```

### Example string comparison with strcmp

This example uses `strcmp()` from `string.h`. It behaves a little differently
from `compare_strings()` in our above examples -- it returns an `int` rather
than a `bool`, and moreover it returns `0` if the strings are equal. It returns
a *positive* value if the first argument comes before the second in lexical
order, and a *negative* value if the first argument comes after the second.

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *s = get_string("s: ");
    char *t = get_string("t: ");
    if (strcmp(s, t) == 0)
    {
        printf("same\n");
    }
    else
    {
        printf("different\n");
    }
}
```

### Example with strcmp and error checking

It turns out that `get_string()` returns a `char*` OR `NULL` if there is an
error. `NULL` refers to the memory address 0, which should be an invalid address
for any value, nothing should be stored there. We should be checking if we end
up with an error from `get_string`, and if we do, return a non-zero exit code.
Doing this will help us to avoid a *segmentation fault* which can occur when we
try to access memory that we really shouldn't be accessing.

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *s = get_string("s: ");
    if (s == NULL)
    {
        return 1;
    }
    char *t = get_string("t: ");
    if (t == NULL)
    {
        return 1;
    }

    // Compare strings for equality
    if (strcmp(s, t) == 0)
    {
        printf("same\n");
    }
    else
    {
        printf("different\n");
    }
    return 0;
}
```

### Slightly more succinct

Because `NULL` is the zero address, we could write `s == NULL` or `s == 0` or
`s == 0x0`. But because `0` evaluates to `false` when cast as a boolean, we can
shorten the whole thing to `!s`.

```c
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    char *s = get_string("s: ");
    if (!s)
    {
        return 1;
    }
    char *t = get_string("t: ");
    if (!t)
    {
        return 1;
    }

    // Compare strings for equality
    if (strcmp(s, t) == 0)
    {
        printf("same\n");
    }
    else
    {
        printf("different\n");
    }
    return 0;
}
```

You could probably even do `if (!strcmp(s, t))...`

### Example String Copying

```c
#include <cs50.h>
#include <ctype.h>
#include <stio.h>
#include <string.h>

int main(void)
{
    // get a string
    string s = get_string("s: ");

    // copy string's address
    string t = s;

    // capitalize first letter in string
    if (strlen(t) > 0)
    {
        t[0] = toupper(t[0]);
    }

    // print string twice
    printf("s: %s\n", s);
    printf("t: %s\n", t);
}
```

We'll find that this doesn't work! When we try to capitalize `t`, we're also
capitalizing `s` because *we didn't copy the string `s` into `t`, we just copied
the pointer!* To actually copy a string, we need to allocate a new array and
individually copy each element from the original array into the new array.

### String Copying that Works

```c
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Get a string
    char *s = get_string("s: ");
    if (!s)
    {
        return 1;
    }

    // Allocate memory for another string
    char *t = malloc((strlen(s) + 1) * sizeof(char));
    // We need strlen(s) + 1 because we need room for the null character
    // sizeof(char) is 1 by definition, so this could be simplified:
    // char *t = malloc(strlen(s) + 1);
    if (!t)
    {
        return 1;
    }

    // Copy string into newly allocated memory
    // We need i <= n because we need to copy over the null character
    for (int i = 0, n = strlen(s); i <= n; i++)
    {
        t[i] = s[i];
    }

    // Capitalize first letter in copy
    if (strlen(t) > 0)
    {
        t[0] = toupper(t[0]);
    }

    // Print strings
    printf("s: %s\n", s);
    printf("t: %s\n", t);

    // But we NEED to de-allocate the memory that we allocated for t
    // If we don't, we'll keep using up more and more memory without ever giving
    // it back to the operating system (called a memory leak)
    free(t);
    return 0;
}
```

### Built-In String Copying

```c
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Get a string
    char *s = get_string("s: ");
    if (!s)
    {
        return 1;
    }

    // Allocate memory for another string
    char *t = malloc((strlen(s) + 1) * sizeof(char));
    // We need strlen(s) + 1 because we need room for the null character
    // sizeof(char) is 1 by definition, so this could be simplified:
    // char *t = malloc(strlen(s) + 1);
    if (!t)
    {
        return 1;
    }

    // Use the built-in strcpy function
    strcpy(t, s);

    // Capitalize first letter in copy
    if (strlen(t) > 0)
    {
        t[0] = toupper(t[0]);
    }

    // Print strings
    printf("s: %s\n", s);
    printf("t: %s\n", t);
}
```

## Getting User Input

### Get an int from user

This is pretty similar to how `get_int()` is implemented

```c
#include <stdio.h>

int main(void)
{
    // Declare an int variable x
    int x;
    
    // Ask for the input
    printf("x: ");
    
    // Uses a formatted string like printf, but we're not outputting it
    // We're inputting it from the user
    // &x is getting the address of x
    scanf("%i", &x);
    
    printf("x: %i\n", x);
}
```

### Getting a string from user

We could not naively substitute in `char *` and `%s` and expect this to work for
getting string inputs. Initializing a `char *` variable is literally only
allocating the space in memory for the pointer itself, not for the variable that
the pointer points to. Pointers in modern computers are 64 bits, hence 64-bit
architecture (x86_64, Nintendo 64, etc.)

```c
#include <stdio.h>

int main(void)
{
    // Let's arbitrarily pick 6 for the length of the array (5 characters + \0)
    char s[6];
    printf("s: ");

    /* We don't need to do &s here because an array variable is really just the
     * pointer to the array values. Arrays are passed by reference, and this is
     * what that means practically -- s is a pointer to 6 bytes of memory. We
     * can pass around the s variable without having the compiler copy the array
     * and duplicate memory usage because it really just passes around the
     */single 64-bit pointer
    scanf("%s", s);
    printf("s: %s\n", s);
}
```

This implementation is *inherently unsafe* because we only allocate 6 characters
worth of memory, but we do nothing to prevent the user from inputting more,
which would force the program to try to save that into *the contiguous block of
memory that starts at the address that `s` points to*. We don't have
"permission" to use memory past those 6 bytes. The compiler will often allocate
slightly more than the requested memory block, but if you go beyond that, you
will get a segmentation fault wherein the program crashes because you are trying
to touch memory that you should not be touching.

So the key feature that `get_string` provides us is *bounds checking*, making
sure that we don't end up with a segmentation fault.

## Memory Addresses

The below code will print out *not* the strings that we get from users, but
rather the actual memory addresses of those strings.

```c
#include <cs50.h>
#include <stdio.h>

int main(void)
{
    char *s = get_string("s: ");
    char *t = get_string("t: ");

    // %p for pointer format
    printf("s: %p\n", s);
    printf("t: %p\n", t);
}
```

Addresses are reported in hexadecimal format, where each digit represents 4 bits
and can take a value from 0 to 9 or 'a' to 'f' (a total of sixteen different
digits). Hexadecimal values are represented `0x55a627436a0` -- starting with
`0x`.

## Example: Swap 2 Values

If we want to swap the values of two variables, we need to have some temporary
memory available to store one value while it is being overwritten by the other.

This code snippet seems like it should make sense:

```c
void swap(int a, int b)
{
    int tmp = a;
    a = b;
    b = tmp;
}
```

However, this doesn't work. As we have learned, variables in C are passed *by
value* -- `swap()` gets *copies* of `a` and `b`, not the actual variables.
Instead, we need to pass the *addresses* of the variables so that the function
can change the actual values, instead of just changing copies.

```c
void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
```

In this context, when we specify `*a` in the arguments of a function, we are
telling C that we are expecting not an `int`, but the address of an `int`. We're
providing pointers that we're calling `a` and `b`. `tmp` is still just a regular
`int`. But we don't actually care about the address of `a`, we want to store the
value that is stored at the address `*a`. In this context, `int tmp = *a;`, `*a`
is the *dereference* operator that is telling C to *go to the address `a` and
get the value stored there*. In the next line, we're getting the value that's
stored at the address `b` and storing it in the chunk of memory that `a` points
to. Then finally, we are storing the value `tmp` in the chunk of memory that `b`
points to.

When we're not declaring variables or variable types, `&x` means *give me the
address of `x`*, whereas `*x` means *give me the value stored at address `x`*.

To make that function `swap()` work, we need to pass in pointers:

```c
#include <stdio.h>

void swap(int *a, int *b);

int main(void)
{
    int x = 1;
    int y = 2;

    printf("x is %i, y is %i\n", x, y);
    swap(&x, &y);
    printf("x is %i, y is %i\n", x, y);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
```

## Other Memory Nomenclature

The computer treats different bytes in memory in different ways. At the "top" of
the memory of your program is the *text* of that program -- the actual binary
that your code has been compiled into. The *heap* is the chunk of memory that
you have access to when you call `malloc()` -- it's the memory that's available
for your program to use. The bottom part of the memory is the *stack*. The stack
is the part of memory that functions use when they are called. This is relevant
and important for understanding how variables are assigned and manipulated. In
C, `main()` is always the first function on the stack, and stacks are built from
the bottom up, so `main()` is always on the bottom of the stack. In our example
above, `main()` assigns two variables, `x` and `y`. These get their own chunks
of memory inside of `main()`'s frame -- the slice of memory in the stack that
`main()` has been given. We then have `swap()`, which gets its own slice of
memory in the stack, above `main()`. It gets space for `a`, `b`, and `tmp`. In
those slots for `a` and `b`, it gets *copies* of `x` and `y` -- they're
different locations in memory, different actual bytes, so of course they're only
copies. We're not moving the actual location of `x` and `y` in the stack. So in
the earlier example of `swap()` that didn't actually work, `a` and `b` are
getting copies of `x` and `y`, and the function is actually swapping them, but
it's swapping the copies, not the actual values. When the function returns, the
slice of memory on the stack gets freed up, those variables are effectively
erased, and we're still left with our original `x` and `y`. In the second
example, which actually worked, `swap()` took arguments `*a` and `*b`. Those are
still copies of the values that were passed in, but the values that were passed
in were *not* `x` and `y`, they were `&x` and `&y`. They were copies of
addresses, and a copy of address still leads to the same place. So `swap()`
wasn't trying to swap values in its slice of memory, it was following those
addresses and overwriting the values stored at those locations.

## `struct`

If I wanted to implement a program that contained data on multiple students, it
would get messy to store all the different data about all the different students
in a lot of different variables. It would be a lot easier if we could create
some compound data structures that could contain multiple pieces of information
about each student. `struct` allows us to declare our own custom data types in
custom header files and use them in our own programs.

```h
// Represents a student

typedef struct
{
    char *name;
    char *dorm;
}
student;
```

```c
#include <cs50.h>
#include <stdio.h>

#include "struct.h"

int main(void)
{
    // Space for students
    int enrollment = get_int("Enrollment: ");
    student students[enrollment];

    // Prompt for students' names and dorms
    for (int i = 0; i < enrollment; i++)
    {
        students[i].name = get_string("Name: ");
        students[i].dorm = get_string("Dorm: ");
    }
}
```

We can access elements in an array with brackets, and we can access fields in a
`struct` using dot-name notation.

Now that we know about pointers, we can do even more with this data:

```c
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#include "struct.h"

int main(void)
{
    // Space for students
    int enrollment = get_int("Enrollment: ");
    student students[enrollment];

    // Prompt for students' names and dorms
    for (int i = 0; i < enrollment; i++)
    {
        students[i].name = get_string("Name: ");
        students[i].dorm = get_string("Dorm: ");
    }

    // Save students to disk
    FILE *file = fopen("students.csv", "w");
    if (file)
    {
        for (int i = 0; i < enrollment; i++)
        {
            fprintf(file, "%s,%s\n", students[i].name, students[i].dorm);
        }
        fclose(file);
    }
}
```

# Video Shorts

## Recursion

- A recursive function is one that *calls itself* as part of its execution
- Recursion is often used to implement an elegant solution to a complicated
  problem

### Example: Factorial

- The factorial function $n!$ is defined over all positive integers as the
  product of all positive integers less than or equal to $n$.
- Factorial can be calculated either through iteration or recursion
    + Iteration would involve iterating through all positive integers less than
    or equal to $n$ and multiplying them together
    + Recursion is perhaps a cleaner, more elegant approach

```
fact(1) = 1
fact(2) = 2 * 1
fact(3) = 3 * 2 * 1
fact(4) = 4 * 3 * 2 * 1
fact(5) = 5 * 4 * 3 * 2 * 1
...
```

We can rewrite the above using recursion:

```
fact(1) = 1
fact(2) = 2 * fact(1)
fact(3) = 3 * fact(2)
fact(4) = 4 * fact(3)
fact(5) = 5 * fact(4)
...
```

Or even more generally: `fact(n) = n * fact(n - 1)`

Recursive functions need two components:

1. The *base case*, which when triggered will terminate the recursive process
2. The *recursive case*, which when triggered will call the parent function with
   different inputs

Recursion relies on having an easy answer to some variant of the problem, which
we represent as the base case. When we get to that sub-problem, we already know
the answer and can return it right away, thus starting a chain of constructing
the answer to the actual problem from the simpler versions that we've already
solved.

In the factorial function, the base case is `fact(1) = 1`.

```c
int fact(int n)
{
    // base case
    if (n == 1)
        return 1;

    // recursive case
    return n * fact(n - 1);
}
```

In general, but not always, recursive functions replace loops in non-recursive
functions.

Iterative factorial:

```c
int fact_iter(int n)
{
    int product = 1;
    while (n > 0)
    {
        product *= n;
        n--;
    }
    return product;
}
```

It's also possible to have multiple base cases or multiple recursive cases,
depending on the problem. 

A problem with multiple base cases is the Fibonacci number sequence:
- The first element is 0
- The second element is 1
- The $n$<sup>th</sup> element is the sum of the $(n-1)$<sup>th</sup> and
  $(n-2)$<sup>th</sup> elements

In this example, the base cases are `fib(0) = 0` and `fib(1) = 1`, and the
recusrive case is `fib(n) = fib(n-1) + fib(n-2)`.

A problem with multiple recursive cases is the Collatz conjecture, which 
stipulates that you will always reach 1 if you follow these steps:
- If $n=1$, stop
- Otherwise, if $n$ is even, repeat the process on $n/2$
- Otherwise, if $n$ is odd, repeat the process on $3n+1$

A recursive function `collatz(n)` could calculate how many steps it takes to
reach 1 if you start from $n$ and recurse as indicated above

```c
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
```

## Hexadecimal

Hexadecimal is another way of writing numbers, using a base of 16, rather than
base 10 (decimal) or base 2 (binary) or base 8 (octal). In hexadecimal, we have
16 digits to work with (0-9 and A-F). It is useful because it is completely
translatable into binary (there are no rounding issues like we may have in
converting decimal to binary), but it's much more compact and easy for humans to
read. 16 is $2^4$, which makes for a very direct mapping of 4 bits (binary
digits). 


Decimal | Binary | Hexadecimal
---------|----------|---------
 0  | 0000 | 0
 1  | 0001 | 1
 2  | 0010 | 2
 3  | 0011 | 3
 4  | 0100 | 4
 5  | 0101 | 5
 6  | 0110 | 6
 7  | 0111 | 7
 8  | 1000 | 8
 9  | 1001 | 9
 10 | 1010 | A
 11 | 1011 | B
 12 | 1100 | C
 13 | 1101 | D
 14 | 1110 | E
 15 | 1111 | F

 When we are expressing numbers in hexadecimal format, we normally prefix the
 numbers with `0x`. This just allows for humans to more easily distinguish
 them from decimal numbers.

 Decimal | Binary | Hexadecimal
---------|----------|---------
 0  | `0000` | `0x0`
 1  | `0001` | `0x1`
 2  | `0010` | `0x2`
 3  | `0011` | `0x3`
 4  | `0100` | `0x4`
 5  | `0101` | `0x5`
 6  | `0110` | `0x6`
 7  | `0111` | `0x7`
 8  | `1000` | `0x8`
 9  | `1001` | `0x9`
 10 | `1010` | `0xA`
 11 | `1011` | `0xB`
 12 | `1100` | `0xC`
 13 | `1101` | `0xD`
 14 | `1110` | `0xE`
 15 | `1111` | `0xF`

 Just like binary has place values (1, 2, 4, 8, ...) and decimal has place
 values (1, 10, 100, 1000, ...), hexadecimal does too. Instead of powers of 2 or
 powers of 10, they're powers of 16. The first place is $16^0=1$, the second is
 $16^1=16$, the third is $16^2 = 256$, the fourth is $16^3=4096$, etc. So where
 in decimal: 
 
 $$397 = 3\times10^2 + 9\times10^1 + 7\times10^0 \\
 = 3\times100 + 9\times10 + 7\times 1 \\
 = 300 + 90 + 7 \\
 = 397$$
 
 in hexadecimal we would have:
 
 $$\mathtt{0x397} = 3\times16^2 + 9\times16^1 + 7\times16^0 \\
 = 3\times256 + 9\times16 + 7\times1 \\
 = 768 + 144 + 7 \\
 = 919$$

 Converting between hexadecimal and binary is really easy because each
 hexadecimal digit converts perfectly to one 4-bit binary number. To convert
 binary to hexadecimal, divide the binary number into groups of 4, left-padding
 the leftmost group with 0s if necessary. Then simply convert each group of 4
 bits into the corresponding hexadecimal digit, using the above chart.

 $$
    \mathtt{0100 0110 1010 0010 1011 1001 0011 1101} \\
    = \mathtt{0100\ 0110\ 1010\ 0010\ 1011\ 1001\ 0011\ 1101} \\
    = \mathtt{0x\ 4\ 6\ A\ 2\ B\ 9\ 3\ D}
 $$

 We use hexadecimal in computer science not for math, but because memory
 addresses and other low-level data are often expressed in hexadecimal format.

## File Pointers



## Call Stacks



## Pointers



## Dynamic Memory Allocation


