#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int pipe1[2], pipe2[2];

    pipe(pipe1);
    pipe(pipe2);
    char buf[10];

    if (fork() == 0) {
        // son
        close(pipe1[1]); // close pipe1 write
        close(pipe2[0]); // close pipe2 read

        read(pipe1[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(pipe2[1], buf, 1);
        exit(0);
    } else {
        // parent
        close(pipe2[1]);
        close(pipe1[0]);

        write(pipe1[1], "1", 1);
        read(pipe2[0], buf, 1);
        printf("%d: received pong\n", getpid());
        int status;
        wait(&status); // wait for son to exit
        exit(0);
    }
}
