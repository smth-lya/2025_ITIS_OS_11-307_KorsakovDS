#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Filename and N required. Usage: %s <filename> <N> \n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "r");

	if (f == NULL)
	{
		printf("Error while opening file \n");
		return 1;
	}

	char *endptr;
	long value = strtol(argv[2], &endptr, 10);

	if (*endptr != '\0')
	{
		printf("Second argument must be a number \n");
		fclose(f);
		return 1;
	}

	int n = (int)value;


	if (n > 0)
	{
		int ch;
		int counter = 0;
		while ((ch = fgetc(f)) != EOF)
		{
			if (putchar(ch) == EOF)
			{
				printf("Output error \n");
				fclose(f);
				return 1;
			}

			if (ch == '\n')
				counter++;

			if (counter == n)
			{
				printf("[ Press ENTER ]");
				fgetc(stdin);

				counter = 0;
			}
		}
	}
	else
	{
		int ch;
		while ((ch = fgetc(f)) != EOF)
		{
			if (putchar(ch) == EOF)
			{
				printf("Output error \n");
				fclose(f);
				return 1;
			}
		}
	}

	if (fclose(f) == EOF)
	{
		printf("Error while closing file \n");
	}

	return 0;
}
