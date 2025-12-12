#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

void print_dir(const char *path)
{
	DIR *dir = opendir(path);
	if (dir == NULL)
	{
		printf("Error while opening directory '%s' \n", path);
		return;
	}

	printf("Directory: '%s' \n", path);

	struct dirent *d;
	while ((d = readdir(dir)) != NULL)
	{
		printf("%s\n", d->d_name);
	}

	if (errno != 0)
	{
		printf("Error reading directory \n");
		closedir(dir);
		return;
	}

	if (closedir(dir) != 0)
	{
		printf("Error closing directory \n");
		return;
	}
}

int main(int argc, char* argv[])
{

	print_dir(".");

	printf("\n");

	if (argc < 2)
	{
		printf("Usage: %s <r_directory_path>", dir[0]);
		return 0;
	}

	print_dir(argv[1]);
	return 0;
}
