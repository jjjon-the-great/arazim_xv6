#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char * error_message = "argument needed!\n";
  if (argc < 2){
    write(1, error_message, 17);
  } else { sleep( (int) atoi(argv[1]) ); }
  exit(0);
}
