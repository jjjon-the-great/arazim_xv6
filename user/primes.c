#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

//void int_to_byte_arr(int arg, char * bytes);

/*protocol with -1 as terminator message.*/
/*
void send_pipe(int message, int pipe[2]){
  char bytes[4];
  int_to_byte_arr(message, bytes);
  write(pipe[0], bytes, 4);
  int dummy;
  read(pipe[1], &dummy, 1); //wait until release accepted.
  return;
}

void read_pipe(int pipe[2], char * message){
  int reply = -1;
  read(pipe[1], message, 4);
  write(pipe[0], &reply, 1);
  return;
}
*/
void send_int (int pipe[2], int arg){
  //printf("sending %d\n", arg);
  write(pipe[1], &arg, 4);
  return;
}

int receive_int (int pipe[2]){
  int result = 67;
  if (read(pipe[0], &result, 4)==-1){
    printf("read ERROR\n");
    exit(0);
  }
  //printf("recieved: %d\n", result);
  return result;
}

void print_int(int arg){
  printf("%d is prime\n", arg);
  return;
}

/*
void layer_logic(int p[2], int max_depth){
  //first capture
  if (max_depth == 0) {
    return;
  }
  //printf("hi!");
  //printf("first:");
  int first = receive_int(p);
  print_int(first);
  //printf("next:");
  int target;
  target = receive_int(p);
  //printf("new target: %d\n", target);
  if (target == 0){
    exit(0);
  } else {
    int new_pipe[2];
    if (pipe(new_pipe) == -1) {
      printf("PIPE ERROR\n");
    }
    if (fork() == 0){
      layer_logic(new_pipe, max_depth - 1);
    } else {
      while(target != 0){
        if (target % first != 0) send_int(new_pipe, target);
        target = receive_int(p);
      }
      send_int(new_pipe, 0);
      close(new_pipe[1]);
      exit(0);
    }
  }
}
*/

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
