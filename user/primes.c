#include "kernel/types.h"
#include "user.h"

void solve(int pipe_in[2]) {
    int pipe_out[2];
    int base; // first input is a prime
    // use base for filter

    read(pipe_in[0], &base, (int)sizeof(base));

    pipe(pipe_out);

    if (base == -1) {
        exit(0);
    }

    printf("prime %d\n", base);

    if (fork()) {
        // father
        close(pipe_out[0]);
        int coming;
        while (read(pipe_in[0], &coming, (int)sizeof(coming))) {
            if (coming == -1)
                break;

            if (coming % base != 0) {
                write(pipe_out[1], &coming, (int)sizeof(coming));
            }
        }
        coming = -1;
        write(pipe_out[1], &coming, (int)sizeof(coming));

        int status;
        wait(&status);
        exit(0);
    } else {
        // child
        close(pipe_in[0]);
        close(pipe_out[1]);
        solve(pipe_out);
    }
}

int main(int argc, char *argv[]) {
    int main_pipe[2];
    pipe(main_pipe);
    if (fork() != 0) {
        close(main_pipe[0]);

        for (int i = 2; i <= 35; ++i) {
            write(main_pipe[1], &i, (int)sizeof(i));
        }
        int end = -1; // use -1 as eof
        write(main_pipe[1], &end, (int)sizeof(end));
        close(main_pipe[1]);
        int status;
        wait(&status);
        exit(0);
    } else {
        close(main_pipe[1]);
        solve(main_pipe);
        int status;
        wait(&status);
        exit(0);
    }
}
