#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void scan_dir(const char *fullpath, const char *name, FILE *out)
{
    DIR *dir = opendir(fullpath);
    if (!dir)
        return;

    int file_count = 0;
    long long total_size = 0;
    long long max_size = 0;
    char largest_file[4096] = "-";

    struct dirent *d;
    while ((d = readdir(dir)) != NULL)
    {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;

        char file_path[4096];
        int written = snprintf(file_path, sizeof(file_path), "%s/%s", fullpath, d->d_name);
        if (written < 0 || written >= sizeof(file_path))
            continue;

        struct stat st;
        if (stat(file_path, &st) != 0)
            continue;

        if (S_ISREG(st.st_mode))
        {
            file_count++;
            total_size += st.st_size;

            if (st.st_size > max_size)
            {
                max_size = st.st_size;
                strncpy(largest_file, d->d_name, sizeof(largest_file) - 1);
                largest_file[sizeof(largest_file) - 1] = '\0';
            }
        }
    }

    closedir(dir);

    if (file_count == 0)
        max_size = 0;

    char buf[8192];
    snprintf(buf, sizeof(buf),
             "%s: files=%d\t total_size=%lld\t largest=%s (%lld bytes)\n",
             name, file_count, total_size, largest_file, max_size);

    fputs(buf, out);
    printf("%s", buf);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        return 1;

    const char *dirpath = argv[1];
    const char *outfile = argv[2];

    DIR *dir = opendir(dirpath);
    if (!dir)
        return 1;

    FILE *out = fopen(outfile, "w");
    if (!out)
    {
        closedir(dir);
        return 1;
    }

    printf("Scanning directory '%s'\n\n", dirpath);

    scan_dir(dirpath, ".", out);

    struct dirent *d;
    while ((d = readdir(dir)) != NULL)
    {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;

        char subpath[4096];
        int written = snprintf(subpath, sizeof(subpath), "%s/%s", dirpath, d->d_name);
        if (written < 0 || written >= sizeof(subpath))
            continue;

        struct stat st;
        if (stat(subpath, &st) != 0)
            continue;

        if (S_ISDIR(st.st_mode))
            scan_dir(subpath, d->d_name, out);
    }

    closedir(dir);
    fclose(out);
    return 0;
}
