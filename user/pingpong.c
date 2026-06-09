#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  char buf[5];

  if (fork() == 0){
    //child behaviour
    read(p[1], buf, 1);
    char * message = "_: received ping\n";
    message[0] = '0' + getpid();
    write(1, message, 17);
    write(p[0], "1", 1);
    exit(0);

  } else {
    //parent behaviour
    write(p[0], "0", 1);
    sleep(1); //needed to prevent race
    read(p[1], buf, 1);
    char * message = "_: received pong\n";
    message[0] = '0' + getpid();
    write(1, message, 17);
    wait(0);

  } exit(0);
}
