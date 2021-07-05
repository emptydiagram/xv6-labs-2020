#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void process_primes(int pipe_fds[2]);

int
main(int argc, char *argv[])
{
  int p[2];
  
  if (pipe(p) < 0) {
    fprintf(2, "primes: Could not open pipe");
    exit(1);
  }

  int fork_result = fork();
  if (fork_result < 0) {
    fprintf(2, "primes: Error forking");
  } else if (fork_result == 0) {
    process_primes(p);
  }

  close(p[0]);
  for (int i = 2; i <= 35; i++) {
    write(p[1], &i, 1);
  }
  close(p[1]);

  wait(0);

  exit(0);
}

void process_primes(int pipe_fds[2]) {
  close(pipe_fds[1]);

  int first;
  if (read(pipe_fds[0], &first, 1) < 1) {
    // this shouldnt happen, but if it does, just exit
    close(pipe_fds[0]);
    exit(0);
  }
  printf("prime %d\n", first);

  int p[2];

  int n;

  // if there are no other numbers to read, then we don't need to create any more pipes/processes
  if (read(pipe_fds[0], &n, 1) < 1) {
    close(pipe_fds[0]);
    exit(0);
  }

  if (pipe(p) < 0) {
    fprintf(2, "primes: Could not open pipe");
    exit(1);
  }

  int fork_result = fork();
  if (fork_result < 0) {
    fprintf(2, "primes: Error forking");
  } else if (fork_result == 0) {
    process_primes(p);
  }

  close(p[0]);

  do {
    if (n % first != 0) {
      write(p[1], &n, 1);
    }
  } while (read(pipe_fds[0], &n, 1) > 0);

  close(p[1]);
  close(pipe_fds[0]);
  wait(0);
  exit(0);
}
