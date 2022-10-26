#include <cerrno>
#include <fcntl.h>  // open
#include <cstdio>   // printf
#include <string.h>   // strerror

int main(){
  
  int fd;
  fd = open("", O_RDONLY);
  if(fd == -1){
    printf("%d: %s", errno, strerror(errno));
  }
  return 0;
}
