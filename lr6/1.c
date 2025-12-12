#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

void print_info(const char *process_name) {
    struct timeval tv;
    struct tm *tm_info;

    gettimeofday(&tv, NULL);
    tm_info = localtime(&tv.tv_sec);

    printf("%s, PID=%d, PPID=%d, Time=%02d:%02d:%02d:%03ld\n",
           process_name,
           getpid(),
           getppid(),
           tm_info->tm_hour,
           tm_info->tm_min,
           tm_info->tm_sec,
           tv.tv_usec / 1000);
}

int main() {
    pid_t pid1, pid2;

    pid1 = fork();  

    if (pid1 == 0) {
        print_info("child 1");
        return 0;
    }

    pid2 = fork();

    if (pid2 == 0) {
        print_info("child 2");
        return 0;
    }

    print_info("parent");

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    system("ps -x");

    return 0;
}
