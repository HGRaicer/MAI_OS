// dynamic.c: Динамическая загрузка библиотек
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef int (*PrimeCountFunc)(int, int);
typedef void (*SortFunc)(int *, int);

int main() {
    void *lib1_handle;
    void *lib2_handle;
    PrimeCountFunc PrimeCount;
    SortFunc Sort;
    char *error;

    lib1_handle = dlopen("./build/lib1.so", RTLD_LAZY);
    lib2_handle = dlopen("./build/lib2.so", RTLD_LAZY);
    if (!lib1_handle || !lib2_handle) {
        fprintf(stderr, "Error loading library: %s\n", dlerror());
        exit(1);
    }

    printf("select the library you want to use \n 1:lib1(quicksort and sieve of eratosthenes) \n 2:lib2(bubble sort and naive prime counting) \n");
    void *lib_handle;
    int var;
    scanf("%d", &var);
    if (var == 1) lib_handle = lib1_handle;
    else lib_handle = lib2_handle;

    PrimeCount = (PrimeCountFunc)dlsym(lib_handle, "PrimeCount");
    Sort = (SortFunc)dlsym(lib_handle, "Sort");

    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Error loading symbols: %s\n", error);
        exit(1);
    }

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

    dlclose(lib_handle);
    return 0;
}
