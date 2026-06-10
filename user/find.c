#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
search_dir(char *path, char * target, int MAXSIZE)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    //printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      char * fname = fmtname(buf);
      int i = 0;
      while (fname[i] != ' '){
        i ++;
      }
      fname[i] = '\0';
      //printf("looking at %s\n", fname);
      if (strcmp(fname, target) == 0 && st.type == 2){
        printf("%s/%s\n", path, fname);
      }
      if (st.type == 1 && strcmp(fname, ".") != 0 && strcmp(fname, "..") != 0){
        char new_path[MAXSIZE];
        //printf("looking at dir %s with %s\n", fname, strcmp(fname, "."));
        strcpy(new_path, path);
        strcpy(new_path+strlen(path), "/");
        strcpy(new_path+strlen(path)+1, fname);
        fname[i] = ' ';
        search_dir(new_path, target, MAXSIZE);
      }
      //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int MAX_SIZE = 50; 
  if (argc != 3){
    printf("invalid arguments!\n"); 
  } 
  search_dir(argv[1], argv[2], MAX_SIZE);
  exit(0);
}
