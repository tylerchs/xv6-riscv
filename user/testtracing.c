#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
   // invoke uptie and print returned value
   printf("uptime: %d\n", uptime());

   // call syscalltrace to start tracing system calls
   syscalltrace();
   // make new directory and change into it
   mkdir("tracedir");
   chdir("tracedir");
   dup(1);

   // fork
   int pid = fork();
   // have child exec syscall
   if (pid == 0){
      getpid();
      exit(0);
   }

   exit(0);
}
