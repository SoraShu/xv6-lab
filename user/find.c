#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename) {
    {
        char buf[512];
        int fd;
        struct dirent de;
        struct stat st;

        if ((fd = open(path, 0)) < 0) {
            // open path
            // fprintf(2, "cannot open %s\n", path);
            return;
        }

        if (fstat(fd, &st) < 0) {
            // stat path
            // fprintf(2, "cannot stat %s\n", path);
            close(fd);
            return;
        }

        if (st.type != T_DIR) {
            // fprintf(2, "path mast be a dir\n");
            close(fd);
            return;
        }

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            strcpy(buf, path);
            // append '/'
            char *p = buf + strlen(buf);
            *p++ = '/';

            if (de.inum == 0) {
                continue;
            }

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                // printf("cannot stat %s\n", buf);
                continue;
            }

            switch (st.type) {
            case T_FILE:
                if (strcmp(filename, de.name) == 0) {
                    printf("%s\n", buf);
                }
                break;

            case T_DIR:
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                    find(buf, filename);
                }
                break;
            }
        }
        close(fd);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("ERROR, 2 params needed!\n");
        printf("Usage:\n    find <path> <filename>\n");
        exit(0);
    }
    char *path = argv[1], *filename = argv[2];
    find(path, filename);
    exit(0);
}
