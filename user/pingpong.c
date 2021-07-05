#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pa[2];
  int pb[2];

  pipe(pa);
  pipe(pb);

  int fork_result = fork();
  if (fork_result < 0) {
    fprintf(2, "pingpong: Error forking");
  } else if (fork_result == 0) {
    char ch;
    read(pa[0], &ch, 1);
    printf("%d: received ping\n", getpid());
    write(pb[1], &ch, 1);

    close(pa[0]);
    close(pa[1]);
    close(pb[0]);
    close(pb[1]);
    exit(0);
  }

  // initiate the send from the parent
  char ch;
  write(pa[1], "A", 1);
  read(pb[0], &ch, 1);
  printf("%d: received pong\n", getpid());
  close(pa[0]);
  close(pa[1]);
  close(pb[0]);
  close(pb[1]);
  exit(0);
}
