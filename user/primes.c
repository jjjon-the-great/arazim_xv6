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
  char dummy[1];
  for (int i = 0; i < 4; i++){
    dummy[0] = (char) ('\0' + (arg%256));
    write(pipe[1], dummy, 1);
    sleep(1);
    arg = arg/256;
  }
  return;
}

int receive_int (int pipe[2]){
  int result = 0;
  char target[5];
  for (int i = 0; i < 4; i++){
    read(pipe[0], target, 1);
    printf("recieved %c ", target[0]);
    result += (int)(target[0]) * (1 << (8 * i));
  }
  return result;
}


int
main(int argc, char *argv[])
{
  //testing protocol
  int p[2];
  pipe(p);
  if (fork() == 0){
    write(1, "c", 1);
    int ans;
    ans = receive_int(p);
    write(1, "d", 1);
    printf("is: %d",ans);
    close(p[1]);
    exit(0);
  } else {
    write(1,"a",1);
    int message = 69;
    send_int(p, message);
    write(1, "b", 1);
    close(p[0]);
    exit(0);
  }
  exit(0);
}
