#include <stdio.h>

int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		printf("Filename required \n");
		return 1;
	}

	FILE *f = fopen(argv[1], "w");

	if (f == NULL)
	{
		printf("Error while opening file \n");
		return 1;
	}

	int ch;

	while ((ch = fgetc(stdin)) != EOF)
	{
		if (ch == 6)
		{
			printf("Input terminated (Ctrl+F) \n");
			break;
		}

		if (fputc(ch, f) == EOF)
		{
			printf("Error writing to file \n");
			fclose(f);
			return 1;
		}
	}

	if (fclose(f) == EOF)
	{
		printf("Error closing file \n");
		return 1;
	}

	return 0;
}
