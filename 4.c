#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		printf("Usage: %s <source_filename> <target_filename> \n", argv[0]);
		return 1;
	}

	const char *src_name = argv[1];
	const char *dest_name = argv[2];

	struct stat st;
	if (stat(src_name, &st) != 0)
	{
		printf("Error getting file stats \n");
		return 1;
	}

	FILE *f_src = fopen(src_name, "r");
	if (f_src == NULL)
	{
		printf("Error while opening [source <%s> file] \n", argv[1]);
		return 1;
	}

	FILE *f_dest = fopen(dest_name, "w");
	if (f_dest == NULL)
	{
		printf("Error while opening [target <%s> file] \n", argv[2]);

		fclose(f_src);
		return 1;
	}

	int ch;
	while ((ch = fgetc(f_src)) != EOF)
	{
		if(fputc(ch, f_dest) == EOF)
		{
			printf("Error while writing to target file \n");

			fclose(f_src);
			fclose(f_dest);
			return 1;
		}
	}

	if (fclose(f_src) == EOF)
	{
		printf("Error closing source file \n");

		if (fclose(f_dest) == EOF)
			printf("Error closing target file \n");

		return 1;
	}

	if (fclose(f_dest) == EOF)
	{
		printf("Error closing target file \n");
		return 1;
	}

	if (chmod(dest_name, st.st_mode) != 0)
	{
		printf("Error while copying permissions");
		return 1;
	}

	return 0;
}
