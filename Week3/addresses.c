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
