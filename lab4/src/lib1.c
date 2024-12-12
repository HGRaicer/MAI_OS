#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int PrimeCount(int A, int B) {
    if (B < 2) return 0;
    bool *is_prime = (bool *)malloc((B + 1) * sizeof(bool));
    for (int i = 0; i <= B; i++) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= B; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= B; j += i) {
                is_prime[j] = false;
            }
        }
    }
    int count = 0;
    for (int i=A; i<=B; ++i){
        if (is_prime[i]) count++;
    }
    free(is_prime);
    return count;
}

// Быстрая сортировка (сортировка Хоара)
void quicksort(int *array, int low, int high) {
    if (low < high) {
        int pivot = array[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (array[j] <= pivot) {
                i++;
                int temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
        int temp = array[i + 1];
        array[i + 1] = array[high];
        array[high] = temp;

        int pi = i + 1;
        quicksort(array, low, pi - 1);
        quicksort(array, pi + 1, high);
    }
}

void Sort(int *array, int size) {
    quicksort(array, 0, size - 1);
}
