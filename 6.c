#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void scan_subdir(const char *base_path, const char *subdir, FILE *out)
{
	char path[4096];
	int written = snprintf(path, sizeof(path), "%s/%s", base_path, subdir);

	if (written >= sizeof(path))
	{
		printf("Path too long '%s/%s' \n", base_path, subdir);
		return;
	}

	DIR *dir = opendir(path);
	if (dir == NULL)
	{
		printf("Error while opening directory '%s' \n", path);
		return;
	}

	int file_count = 0;
	long long total_size = 0;
	long long max_size = -1;
	char largest_file[4096] = "-";

	struct dirent *d;
	while((d = readdir(dir)) != NULL)
	{
		if(strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
			continue;

		char file_path[4096];
		written = snprintf(file_path, sizeof(file_path), "%s/%s", path, d->d_name);

		if (written >= sizeof(file_path))
		{
			printf("File path too long '%s/%s' \n", path, d->d_name);
			continue;
		}

		struct stat st;
		if (stat(file_path, &st) != 0)
		{
			printf("Error stat(%s) \n", file_path);
			continue;
		}

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

	char buf[8192];
	written = snprintf(buf, sizeof(buf), "%s: files=%d,t \t total_size=%lld, \t largest=%s (%lld bytes) \n", subdir, file_count, total_size, largest_file, max_size);

	if (written >= sizeof(buf))
	{
		snprintf(buf, sizeof(buf), "%s: files: %d, \t total_size=%lld \n", subdir, file_count, total_size);
	}

	for (int i = 0; buf[i] != '\0'; i++)
	{
		fputc(buf[i], out);
	}

	printf("%s", buf);
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Usage: %s <directory_path> <output_filename> \n", argv[0]);
		return 1;
	}

	const char *dirpath = argv[1];
	const char *outfile = argv[2];

	DIR *dir = opendir(dirpath);
	if (dir == NULL)
	{
		printf("Error while opening directory \n");
		return 1;
	}

	FILE *out = fopen(outfile, "w");
	if (out == NULL)
	{
		printf("Error while opening file \n");
		closedir(dir);
		return 1;
	}

	printf("Scanning directory '%s' \n\n", dirpath);

	struct dirent *d;
	while ((d = readdir(dir)) != NULL)
	{
		if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
			continue;

		char subpath[4096];
		int written = snprintf(subpath, sizeof(subpath), "%s/%s", dirpath, d->d_name);

		if (written >= sizeof(subpath))
		{
			printf("Subpath too long '%s/%s' \n", dirpath, d->d_name);
			continue;
		}

		struct stat st;
		if (stat(subpath, &st) != 0)
		{
			printf("Error stat (%s)", subpath);
			continue;
		}

		if (S_ISDIR(st.st_mode))
		{
			scan_subdir(dirpath, d->d_name, out);
		}
	}

	closedir(dir);
	fclose(out);

	return 0;
}
