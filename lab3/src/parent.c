#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    const char* back_name = "Lab3.back";
    unsigned perms = S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* input_data = NULL;
    size_t size = 0;
    printf("Enter numbers separated by spaces: ");
    getline(&input_data, &size, stdin);

    size_t input_len = strlen(input_data);
    if (input_data[input_len - 1] == '\n') {
        input_data[input_len - 1] = '\0';
        input_len--;
    }

    size_t map_size = strlen(argv[1]) + 1 + input_len + 1;

    int fd = shm_open(back_name, O_RDWR | O_CREAT, perms);
    if (fd == -1) {
        perror("SHM_OPEN");
        free(input_data);
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, map_size) == -1) {
        perror("FTRUNCATE");
        close(fd);
        free(input_data);
        exit(EXIT_FAILURE);
    }

    char* memptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memptr == MAP_FAILED) {
        perror("MMAP");
        close(fd);
        free(input_data);
        exit(EXIT_FAILURE);
    }

    snprintf(memptr, map_size, "%s|%s", argv[1], input_data);

    pid_t cpid = fork();
    if (cpid == -1) {
        perror("FORK");
        munmap(memptr, map_size);
        close(fd);
        free(input_data);
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {
        munmap(memptr, map_size);
        close(fd);
        execl("./build/child", "child", NULL);
        perror("EXECL");
        exit(EXIT_FAILURE);
    } else {
        int status;
        wait(&status);
        free(input_data);
        munmap(memptr, map_size);
        close(fd);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Child process completed successfully.\n");
            exit(EXIT_SUCCESS);
        } else {
            fprintf(stderr, "Child process failed.\n");
            exit(EXIT_FAILURE);
        }
    }
}
