#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define DEFAULT_LIMIT 1000000
#define NCHILD 8

static int
ipow(int base, int exp)
{
  int r = 1;
  while(exp-- > 0)
    r *= base;
  return r;
}

static int
is_armstrong(int n)
{
  int digits = 0, t = n, sum = 0;
  while(t > 0){ digits++; t /= 10; }
  t = n;
  while(t > 0){ sum += ipow(t % 10, digits); t /= 10; }
  return sum == n;
}

static int
count_armstrong(int limit)
{
  int count = 0;
  for(int i = 2; i <= limit; i++)
    if(is_armstrong(i))

      count++;
  return count;
}

int
main(int argc, char *argv[])
{
  int limit = DEFAULT_LIMIT;
  if(argc > 1)
    limit = atoi(argv[1]);

  int prios[NCHILD] = {0, 0, 5, 5, 10, 10, 15, 15};
  int pids[NCHILD], counts[NCHILD], times[NCHILD];
  char buf[16];

  printf("Enter a Debug Value (Must be either 0 or 1):\n");
  gets(buf, sizeof(buf));
  if(debug(atoi(buf)) < 0){
    printf("Invalid Debug Value! Continuing with debug=1\n");
    debug(1);
  }

  int start = uptime();
  for(int i = 0; i < NCHILD; i++){
    int pid = priority_fork(prios[i]);
    if(pid < 0){
      printf("priority_fork failed\n");
      exit(1);
    }
    if(pid == 0)
      exit(count_armstrong(limit));   
    printf("\nForking a process with PID: %d and Priority: %d\n", pid, prios[i]);

  }

  for(int i = 0; i < NCHILD; i++){
    int status;
    pids[i] = wait(&status);
    counts[i] = status;
    times[i] = uptime() - start;
  }

  debug(0);   // switch to run moe

  for(int i = 0; i < NCHILD; i++)
    printf("\nProcess %d:\nNumber of Armstrong Numbers between 2 and %d: %d\nTime for process to complete:%dticks\n",
           pids[i], limit, counts[i], times[i]);

  exit(0);
}
