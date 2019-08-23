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
    = \mathtt{0x\ 4 \ 6\ A\ 2\ B\ 9\ 3\ D}
 $$

 We use hexadecimal in computer science not for math, but because memory
 addresses and other low-level data are often expressed in hexadecimal format.

## File Pointers

We can work with files in C through the use of file pointers (`FILE*`), which allow us to interact
with persistent data stores on disk. Persistent data refers to information stored outside of the
runtime of an application, on disk rather than in memory. File manipulation functions all live in
`stdio.h`. All of them accept `FILE*` as one of their paramters, except for the function `fopen()`
which is used to get the file pointer we want to work with.

Common file IO functions:

- `fopen()`
- `fclose()`
- `fgetc()`
- `fputc()`
- `fread()`
- `fwrite()`

### `fopen()`

`fopen()` opens a file and returns a pointer to it. When using `fopen()`, you should always check
the return value to make sure we didn't get back `NULL`.

Usage: `FILE* ptr = fopen(<filename>, <operation>);`

The operations we can use are `"r"` for "read," `"w"` for "write," and `"a"` for "append," which
retains the file contents and adds new content only to the end.

Example: `FILE* ptr1 = fopen("file1.txt", "r");`

### `fopen()`

Closes the file, which prevents us from using the `FILE*` without re-opening the file.

Usage: `fclose(ptr);`

### `fgetc()`

`fgetc()` reads from the file and returns the next character from that file. The operation of the
file pointer must have been `"r"` when using `fopen()` to obtain the file pointer. Otherwise, you
will get an error.

Usage: `char ch = fgetc(ptr);`

At this point, we can already read a file and print it to the screen:

```c
#include <stdio.h>

void cat(FILE* ptr)
{
    char ch;
    while ((ch = fgetc(ptr)) != EOF)
    {
        printf("%c", ch);
    }
}
```

We could put this into a file called `cat.c` which is pretty much what the `cat` function does.

Looking at the second line, we are assigning the result of `fgetc(ptr)` to `ch`, and then using that
same value to compare to `EOF`, the end-of-file marker.

### `fputc()`

`fputc()` is the write-counterpart to `fgetc()` -- it writes a single character to the file. The
operation used to open the file pointer must have been `"w"` or `"a"`.

Usage: `fputc(<char>, <file pointer>);`

Example: `fputc('A', ptr1);`

Now that we can both read and write, we can implement a copy command like `cp`:

```c
#include <stdio.h>

void cp(FILE* ptr1, FILE* ptr2)
{
    char ch;
    while ((ch = fgetc(ptr1)) != EOF)
    {
        fputc(ch, ptr2);
    }
}
```

### `fread()`

`fread()` reads `<qty>` units of size `<size>` from the file pointed to, and stores them in memory
in a buffer (usually an array) that is pointed to by `<buffer>` (this pointer is NOT the file
pointer, it is a regular pointer value like a `char*`). The operation of the file pointer must be
`"r"`. `fread()` is the general version of `fgetc()` that can read an arbitrary amount of data.

Usage: `fread(<buffer>, <size>, <qty>, <file pointer>)`

Example:

```c
int arr[10];
fread(arr, sizeof(int), 10, ptr);
```

It's important to remember in this example that when declaring an array in C, we're actually getting
back a pointer to the first element in the array, even if there is no `*` in the declaration. So
`int arr[10];` assigns to `arr` the value of a pointer that points to the first byte of 10 int-sized
chunks of memory that we have allocated for ourselves. Using `int arr[10];` allocates the buffer on
the stack, but we could also dynamically allocate the buffer using `malloc()`:

```c
double* arr2 = malloc(sizeof(double) * 80);
fread(arr2, sizeof(double), 80, ptr);
```

This would save the data on the heap, rather than the stack.

We could also treat `fread()` like a call to `fgetc()`:

```c
char c;
fread(&c, sizeof(char), 1, ptr);
```

Using it this way, we need to make sure that we're passing in the **address of the variable**.

### `fwrite()`

Counterpart to `fread()`: writes `<qty>` units of size `<size>` to the file pointed to by reading
them from a buffer (usually an array) pointed to by `<buffer>`. The operation of the file pointer
must be either `"w"` or `"a"`.

Usage: `fwrite(<buffer>, <size>, <qty>, <file pointer>);`

Example: 

```c
int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
fwrite(arr, sizeof(int), 10, ptr);
```

In this case, we're pulling information from `arr` and writing it to the file -- the reverse of
`fread()`.

We can similarly use dynamically allocated memory:

```c
double* arr2 = malloc(sizeof(double) * 80);
fwrite(arr2, sizeof(double), 80, ptr);
```

Or treat it like `fputc()`:

```c
char c;
fwrite(&c, sizeof(char), 1, ptr);
```

### Other useful file functions

- `fgets()` reads a full string from a file
- `fputs()` writes a full string to a file
- `fprintf()` allows you to write a formatted string to a file (like `printf()`)
- `fseek()` lets you rewind or fast-forward within a file
- `ftell()` tells you at what position (byte) you are at within a file
- `feof()` tells you whether you've read to the end of a file
- `ferror()` tells you whether an error has occurred while working with a file

## Call Stacks

When you call a function, the system allocates some space in memory just for that function to use.
These chunks of memory are often referred to as *stack frames* or *function frames*. More than one
function's stack frame may exist in memory at a time. For example, `main()` may call another
function, `move()`, which in turn calls `direction()`, so we have three open frames. But in general,
only one of those frames will be active at any given time -- they are all open, they all have space
set aside, but only one function is operating at a time.

Function frames are arranged in a *stack*. The frame of the most recently called function is always
on the top of the stack. When a new function is called, a new frame is *pushed* onto the top of the
stack and becomes the active frame.

When a function finishes its work and returns, its frame is *popped* off of the stack, and the frame
just below it becomes the active frame, now on the top of the stack. This function picks up its work
immediately where it left off when it called the function that had been on top.

This process of pushing and popping frames is why recursion works. Only one of the frames is actually
doing work at any given time, the rest are just sort of paused. So when a function calls itself, the
"parent" function stops its operation, and the new child function begins work. It will return a value
or maybe call another function (maybe itself again), and just continue the process of pushing new
frames, pausing, then continuing work once the active frame is popped.

## Pointers

Pointers give us a different way to pass data between functions. Previously, we have been passing
data **by value** -- when we pass data by value, we're only passing a copy of that data. We're not
passing the memory chunk containing the integer `1`, we're passing the value of `1`, represented as
`0b0001`, that will get put into a new chunk of memory and used totally separately from our original
integer value.

If we use pointer instead, we are able to pass the actual variable itself, not just a copy of the
data that the variable holds. This means that a change that is made in one function could
potentially change what happens in a different function.

### Memory

Let's take a step back and review how computer memory works. Every file is stored on some sort of a
disk drive that allows them to persist even after we turn the computer off. But disk drives are just
storage space, they can't be used for manipulating data. That only happens in RAM (random access
memory). So most programs operate in RAM, sometimes pulling data from a file on disk and/or writing
data back to a file.

Memory is essentially just an enormous array of 8-bit wide bytes. We say enormous, but it's usually
much smaller than the disk drive. However, RAM is ephemeral -- anything stored in RAM is destroyed
when we shut down or restart the computer.

Every piece of data takes up a certain amount of space in RAM. That amount of space is determined by
the data's type (which can differ across programming languages and CPU architecture).

Data Type | Size (bytes)
----------|-------------
`int` | 4
`char` | 1
`float` | 4
`double` | 8
`long long` | 8
`string` | ?

Once again, we can think of memory as a big array of byte-sized cells. Arrays are not only useful for
storage of information, but also for **random access**. Remember that we can access individual 
elements of an array by indicating which index location we want -- we can access any arbitrary 
element that we may be interested in, we don't have to take the whole array as one piece, nor do we
have to go in order, one at a time, from the beginning of the array. Similarly, with memory, we can
access any address to get the element that we want. We don't have to go in order from the beginning.

### Back to Pointers

The most important thing to remember: **pointers are just memory addresses**. Pointers are addresses
to locations in memory where variables live. 

Let's say we want to store the integer `5` in a variable called `k`. There are a couple steps we 
have to take:

#### Step 1

```c
int k;
```

This is creating a box that can hold an integer, and writing `k` on the side. The only things that
can go into the box are integers, and we're calling the box `k`.

#### Step 2

```c
k = 5;
```

We're not actually changing the box `k`, we're putting the integer `5` inside the box. But now we
have an integer-sized box called `k`, and inside of it is `5`.

#### Step 3

What if we say:

```c
int* pk;
```

This also creates a box, and it clearly has something to do with integers, but it's definitely not
the same as `int k;`. 

#### Step 4

```c
pk = &k;
```

What just happened? We didn't put the box `k` inside of `pk`. We didn't put the integer `5` inside
of `pk`. Instead, we put the **address of `k`** into `pk`. It turns out that in step 3, we created
a box that can hold the **address of an integer**. `k` remains unchanged, but inside of `pk` is now
a memory address, something like `0x80C74820`.

### What actually is a pointer?

A pointer is a data item whose *value* is a memory address, and whose *type* describes the data
located at that memory address. Pointers thus allow data structures or variables to be shared
among functions.

### Working with Pointers in C

The simplest pointer in C is the `NULL` pointer. It points to nothing, by definition. When you
create a pointer and don't set its value immediately, you should **always** set its value to `NULL`.
You can check whether a pointer is `NULL` with simple equality (`ptr == NULL`).

You can also create a pointer by extracting the address of a variable that you've already created.
We can do this with the `&` operator. If `x` is of type `int`, then `&x` is a pointer-to-`int` whose
value is the address of `x`. If `arr` is an array of `double`s, then `arr[i]` is a pointer-to-`double`
whose value is the address of the `i`th element in `arr`. An array's name is actually just a pointer
to its first element.

The main purpose of a pointer is to allow us to modify or inspect the location to which it points.
We can directly deal with the variable itself, rather than just a copy of its value. This is done by
**dereferencing** the pointer -- the pointer is a reference, and the value at the address it points
to is the dereferenced pointer. If we have a pointer-to-`char`, called `pc`, then `*pc` is the data
that lives at the emmeory address stored inside the variable `pc`. Used in this context, `*` is
known as the **dereference operator**. It "goes to the reference," accessing the data at that
memory location, which allows you to manipulate it. The pointer gives you the address, and the
dereferencing operator *takes you* to that address.

If we try to dereference a pointer whose value is `NULL`, you'll get a segmentation fault. This
segmentation fault is actually a useful behavior, because it defends against accidentally
manipulating memory that you shouldn't be accessing. This is why we set uninitialized pointers to
`NULL`, so that if we do try to dereference them, we get an error rather than changing some data that
we really shouldn't be touching. This is because uninitialized variables actually do get assigned
slots in memory. For an `int`, for example, it may just be `0` or some other random integer value.
But for `int*`, it would be some random memory address, and the data at that address may be used by
some other program and is thus NOT safe to touch.

### Pointer Syntax in C

```c
int* p;
```

The value of `p` is an address. We can dereference `p` with the `*` operator. If we do that, we will
find an `int` at that memory address.

For most data types, we can declare multiple variables of the same type on the same line:

```c
int x, y, z;
```

But for pointers, the `*` is important for both the type name **and** the variable name.

In contrast, if we do:

```c
int* px, py, py;
```

we will get a pointer-to-`int` called `px`, and then two `int`s called `py` and `pz`. Instead, if
you want to create multiple pointers of the same type on the same line, you would need something
like:

```c
int* pa, *pb, *pc;
```

But for clarity, you may just want to declare them on different lines.

Back to our earlier example:

```c
int k = 5;
int* pk = &k;
```

What happens if we then run:

```c
*pk = 35;
```

In this context, `*` is the dereference operator, getting the value stored at the address that `pk`
points to. So after running `*pk = 35;`, it would then be true that `k == 35`, because we have
changed the value of the variable stored where `pk` points to.

## Dynamic Memory Allocation

Previously, we've worked with pointers as a way to point to another variable hat already exists in
our program. This means that we don't really need to worry about how much memory the pointer needs,
because the memory that it points to is already dealt with by another variable with an explicitly
declared type/value. It's all figured out ahead of time, when the program is compiled. But what if
you don't know how much memory we'll need at compile time? What if the amount of memory that will be
needed is determined at runtime, rather than compile time?

We can get access to **dynamically-allocated memory** at runtime from a pool of memory called the
**heap**. Prior to now, all of the memory that we've been working with has been allocated from the
**stack**, which deals with the memory for function frames. Statically-allocated memory, like memory
that you've given a name to at compile time, comes from the stack.

But the stack and the heap are actually the same pool of memory, they just fill up from opposite
ends. Memory addresses on the stack have lower numbers, and memory addresses on the heap have higher
numbers. 

We can access heap memory by using `malloc()`, which comes from `stdlib.h`. `malloc()` takes as its
parameter the number of bytes that you want. After obtaining the memory, `malloc()` will return a
pointer to that memory. If `malloc()` cannot give you the memory that you asked for, it will return
`NULL`. So you should **always** check for `NULL` after using `malloc()` to deal with the possibility
of a `NULL` pointer. 

If we want to statically obtain an integer, we can just declare it:

```c
int x;
```

This memory will be allocated on the stack.

If instead we wanted to dynamically obtain an integer, we would use:

```c
int *px = malloc(4);  // or malloc(sizeof(int))
```

This will then take a contiguous chunk of memory from the heap of the size that you requested, and
return a pointer to it.

If you wanted to get an integer from a user, you could do:

```c
int x;
scanf("%i", &x);
```

Then, we want to allocate an array of `x` floats **on the stack**:

```c
float stack_array[x];
```

This is legal in C99 and C11, but not earlier versions.

Or we could dynamically allocate an array of `x` floats **on the heap**:

```c
float* heap_array = malloc(x * sizeof(float));
```

There is one issue with dynamically allcoated memory (memory on the heap): it is not automatically
released back to the system when the function is done. That memory is still in use, even after the
function exits, unless you manually release it. If you fail to return memory back to the system when
you're done, it will cause a **memory leak** which can compromise system performance. So when you
finish working with dynamically-allocated memory, you must `free()` it, passing in the pointer to
the memory that you wish to release.

Example:

```c
// allocate memory from the stack
char* word = malloc(50 * sizeof(char));

// do some stuff

// release the memory block
free(word);
```

### Three Rules for Dynamic Memory Allocation

1. Every block of memory that you `malloc()`, you must then `free()`
2. Only memory that you `malloc()` should be `free()`d
3. Do not `free()` any block of memory more than once
