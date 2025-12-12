#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

void copy_file(const char *src, const char *dst, const char *filename) {
    FILE *src_f;
    FILE *dst_f;
    int ch;
    long total = 0;
    struct stat st;

    src_f = fopen(src, "rb");
    if (src_f == NULL) {
        perror("fopen src");
        exit(1);
    }

    if (stat(src, &st) < 0) {
        perror("stat");
        exit(1);
    }

    dst_f = fopen(dst, "wb");
    if (dst_f == NULL) {
        perror("fopen dst");
        exit(1);
    }

    chmod(dst, st.st_mode);

    while ((ch = fgetc(src_f)) != EOF) {
        fputc(ch, dst_f);
        total++;
    }
    
    fclose(src_f);
    fclose(dst_f);

    printf("PID=%d | File=%s | Copied=%ld bytes\n",
           getpid(), filename, total);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <Dir1> <Dir2> <MaxProcesses>\n", argv[0]);
        return 1;
    }

    char *dir1 = argv[1];
    char *dir2 = argv[2];
    int N = atoi(argv[3]);
    if (N <= 0) {
        printf("MaxProcesses must be positive\n");
        return 1;
    }

    DIR *d1 = opendir(dir1);
    if (!d1) {
        perror("opendir Dir1");
        return 1;
    }

    struct dirent *d;
    struct stat st_file;
    int active_procs = 0;

    while ((d = readdir(d1)) != NULL) {
        if (strcmp(d->d_name, ".") == 0 || 
            strcmp(d->d_name, "..") == 0)
            continue;

        char path1[512], path2[512];

        snprintf(path1, sizeof(path1), "%s/%s", dir1, d->d_name);
        snprintf(path2, sizeof(path2), "%s/%s", dir2, d->d_name);

        if (stat(path1, &st_file) == 0 && S_ISREG(st_file.st_mode)) {
            if (access(path2, F_OK) != 0) {
                while (active_procs >= N) {
                    wait(NULL);
                    active_procs--;
                    printf("[Parent] Process finished. Active: %d\n", active_procs);
                }

                pid_t pid = fork();
                if (pid == 0) {
                    copy_file(path1, path2, d->d_name);
                    exit(0);
                } else if (pid > 0) {
                    active_procs++;
                    printf("[Parent] Active processes: %d\n", active_procs);
                } else {
                    perror("fork");
                    exit(1);
                }
            }
        }
    }

    closedir(d1);

    while (active_procs > 0) {
        wait(NULL);
        active_procs--;
    }

    return 0;
}
