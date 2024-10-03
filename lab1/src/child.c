#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT 512
#define MAX_NUMBERS 100

int check(char ch){
  if ((ch-'0' >= 0) && (ch-'0'<=9)){
    return 1;
  }
  return 0;
}

int main() {

  char filename[MAX_INPUT];
  char data[MAX_INPUT];
  char buf;
  int j = 0;
  int flag = 0;
  int i = 0;
  int result = 0;

  while (read(STDIN_FILENO, &buf, 1) > 0){
    if (buf != '|'){
      if (flag != 1){
      filename[j++] = buf;}
      else{
        data[i++] = buf;
      }
    } else {
      flag = 1;
    }
  }

  data[i] = '\0';

  char *buff = malloc(sizeof(char) * 10);

  int jj = 0;
  int tmp = 0;
  for (int k =0; k<i; ++k){
    if (data[k] != '\0'){
      while (check(data[k])==1){
        buff[jj++] = data[k];
        ++k;
      }
      sscanf(buff, "%d", &tmp);
      result += tmp;
      jj = 0;
      while(buff[jj] != '\0'){
        buff[jj++] = ' ';
      }
      jj = 0;
    }
    else{
      break;
    }
  }
  free(buff);
  filename[j]='\0'; 
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Ошибка при открытии файла");
    return 1;
  }

  fprintf(file, "%d\n", result);

  fclose(file);

  printf("Данные успешно записаны в файл: %s\n", filename);

  return 0;
}
