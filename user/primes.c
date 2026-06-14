#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void send_int (int pipe[2], int arg){
  write(pipe[1], &arg, 4);
  return;
}

int receive_int (int pipe[2]){
  int result = 67;
  if (read(pipe[0], &result, 4)==-1){
    printf("read ERROR\n");
    exit(0);
  }
  return result;
}

void new_layer_logic(int p[2], int max_depth, int * args){
  if (max_depth == 0) return;
  int i = 0;
  while (1){
    args[i] = receive_int(p);
    if (args[i] == 0) break;
    i++;
  }
  printf("prime %d\n", args[0]);
  if (args[1] == 0) exit(0);

  if (fork() == 0){
    new_layer_logic(p, max_depth - 1, args);
  } else {
    for (int j = 1; j <= i; j ++){
      if (args[j] % args[0] != 0 || args[j] == 0) send_int(p, args[j]);
    }
    int status;
    wait(&status);
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
  //37 for default
  int max_num = 37;
  int storage[max_num];
  int p[2];
  pipe(p);
  int child;
  if ((child = fork()) == 0){
    new_layer_logic(p, 100, storage);
  } else {
    for (int i = 2; i < max_num; i++){
      send_int(p, i);
    }
    send_int(p, 0);
    int status;
    wait(&status);
    exit(0);
  }
}
