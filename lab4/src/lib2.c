#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Подсчет количества простых чисел наивным методом
int PrimeCount(int A, int B) {
    int count = 0;
    for (int i = A; i <= B; i++) {
        if (i < 2) continue;
        bool is_prime = true;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) count++;
    }
    return count;
}

// Функция для сортировки пузырьком
void bubbleSort(int *arr, int n) {
    for (int i = 0; i < n - 1; ++i) {
        // Флаг для оптимизации
        int swapped = 0;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Обмен элементов
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        // Если не было обменов, массив уже отсортирован
        if (!swapped) {
            break;
        }
    }
}

void Sort(int *array, int size) {
    bubbleSort(array, size);
}
