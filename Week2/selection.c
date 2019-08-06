#include <stdio.h>
#include <stdlib.h>

void selection_sort(int n, int arr[]);

int main(int argc, char *argv[])
{
    int n = argc - 1;
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = atoi(argv[i + 1]);
    }

    selection_sort(n, arr);
}

void selection_sort(int n, int arr[])
{
    // Iterate from the beginning of the unsorted portion through n
    for (int i = 0; i < n; i++)
    {

        // Find the smallest value and its index in the unsorted portion
        int min_index = i;
        int min_val = arr[min_index];
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < min_val)
            {
                min_index = j;
                min_val = arr[j];
            }
        }

        // Save the first unsorted value
        int element_i = arr[i];

        // Re-assign arr[i] to be min_value, and arr[min_index] to be element_i
        arr[i] = min_val;
        arr[min_index] = element_i;
    }

    // Print the array
    for (int i = 0; i < n; i++)
    {
        printf("%i ", arr[i]);
    }
    printf("\n");
}
