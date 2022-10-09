#include "kernel/types.h"
#include "user/user.h"
int main() {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        int j;
        while (read(p[0], &j, sizeof(int)) != 0) {
            printf("prime %d\n", j);
        }
        close(p[0]);
    } else {
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}