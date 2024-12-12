// static.c: Линковка на этапе компиляции
#include <stdio.h>
#include <stdlib.h>

extern int PrimeCount(int A, int B);
extern void Sort(int *array, int size);

int main() {
    int choice;
    printf("1: Count primes\n2: Sort array\nChoose option: ");
    scanf("%d", &choice);

    if (choice == 1) {
        int A, B;
        printf("Enter range A and B: ");
        scanf("%d %d", &A, &B);
        printf("Prime count: %d\n", PrimeCount(A, B));
    } else if (choice == 2) {
        int size;
        printf("Enter array size: ");
        scanf("%d", &size);
        int *array = (int *)malloc(size * sizeof(int));
        printf("Enter array elements: ");
        for (int i = 0; i < size; i++) scanf("%d", &array[i]);
        Sort(array, size);
        printf("Sorted array: ");
        for (int i = 0; i < size; i++) printf("%d ", array[i]);
        printf("\n");
        free(array);
    }

    return 0;
}
