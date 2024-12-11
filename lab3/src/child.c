#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int check(char ch) {
    return (ch >= '0' && ch <= '9') ? 0 : 1;
}

float calc_func(char* data, int size) {
    int result = 0;
    char buffer[10] = {0};
    int jj = 0;

    for (int k = 0; k < size; ++k) {
        if (check(data[k]) == 0) {
            buffer[jj++] = data[k];
        } else if (jj > 0) {
            buffer[jj] = '\0';
            result += atoi(buffer);
            jj = 0;
        }
    }

    return (float)result;
}

int main() {
    const char* back_name = "Lab3.back";
    unsigned perms = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

    int map_fd = shm_open(back_name, O_RDWR, perms);
    if (map_fd < 0) {
        perror("SHM_OPEN");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    if (fstat(map_fd, &statbuf) == -1) {
        perror("FSTAT");
        close(map_fd);
        exit(EXIT_FAILURE);
    }

    size_t map_size = statbuf.st_size;

    char* memptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0);
    if (memptr == MAP_FAILED) {
        perror("MMAP");
        close(map_fd);
        exit(EXIT_FAILURE);
    }

    close(map_fd);

    char filename[20] = {0};
    size_t readed_data_id = 0;

    // Разделяем имя файла и данные
    for (size_t i = 0; i < map_size; i++) {
        if (memptr[i] != '|') {
            filename[i] = memptr[i];
        } else {
            readed_data_id = i + 1;
            break;
        }
    }

    char* input_data = memptr + readed_data_id;
    size_t input_size = map_size - readed_data_id;


    float result = calc_func(input_data, input_size);

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("FOPEN");
        munmap(memptr, map_size);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%.2f\n", result);
    fclose(file);

    munmap(memptr, map_size);
    return 0;
}
