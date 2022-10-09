#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char *command = argv[1];
    char buf;
    char params[MAXARG][100];
    char *exec_args[MAXARG];

    while (1) {
        memset(params, 0, sizeof(params));
        for (int i = 1; i < argc; ++i) {
            strcpy(params[i - 1], argv[i]);
        }

        int read_flag;
        int state = 0, params_n = argc - 1, length = 0;
        // state machine
        while ((read_flag = read(0, &buf, 1)) > 0 && buf != '\n') {
            if (buf != ' ') {
                params[params_n][length++] = buf;
                state = 1;
            } else if (buf == ' ' && state == 1) {
                params[params_n++][length] = 0;
                state = 0, length = 0;
            }
        }

        if (read_flag <= 0) {
            break;
        }

        for (int i = 0; i < MAXARG; ++i) {
            exec_args[i] = params[i];
        }

        exec_args[MAXARG - 1] = 0; // mark end
        if (fork() == 0) {
            exec(command, exec_args);
        } else {
            int status;
            wait(&status);
        }
    }
    exit(0);
}
