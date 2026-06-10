// Create a zombie process that
// must be reparented at exit.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  //test input acceptability
  char buf[100];
  while (gets(buf, 100) != 0){
    //printf("starting execution with \"%s\"\n", buf);
    if (strcmp("", buf) == 0){
      break;
    }
    if (fork() == 0){
      char *new_argv[30];
      int i;
      for (i = 1; i < argc; i++){
        new_argv[i-1] = argv[i];
      }
      buf[strlen(buf)-1] = '\0';
      new_argv[i-1] = buf;
      //printf("new arg is \"%s\", \"%d\"", buf, strlen(buf));
      new_argv[i] = 0;
      //printf("trying to execute %s\n", new_argv[0]);
      exec(new_argv[0], new_argv);
    }
  }
  sleep(1);
  exit(0);
}
