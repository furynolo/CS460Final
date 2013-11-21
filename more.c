#include "ucode.c"

#define BUFSIZE 1
#define FILE_NAME_LENGTH 32
#define SCREEN_HEIGHT 23
#define SCREEN_WIDTH 70

main(int argc, char *argv[])
{
	char filename[FILE_NAME_LENGTH], buf[BUFSIZE];
	int file_fd, bytes_read, i, print_height;

	printf("\n^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^\n");
	printf("^v                 more                  v^\n");
	printf("^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^\n\n");
	
	// Check if the number of arguments is correct.
	if (argc < 2)
	{
		// There are not enough arguments.
		printf("Invalid arguments.  Expected format:\n\n");
		printf("more filename\n\n");
		return;
	}
	else if (argc > 2)
	{
		// There are too many arguments.
		printf("Invalid arguments.  Expected format:\n\n");
		printf("more filename\n\n");
		return;
	}

	bytes_read = BUFSIZE;
	print_height = 0;

	strcpy(filename, argv[1]);

	file_fd = open(filename, O_RDONLY);

	if (file_fd == -1)
	{
		// This file does not exist in the specified directory.
		printf("Invalid arguments.  Expected format:\n\n");
		printf("more filename\n\n");
		return;
	}
	i = 0;

	while (bytes_read == BUFSIZE)
	{
		bytes_read = read(file_fd, buf, BUFSIZE);
		putc(buf[0]);

		if (buf[0] == '\n' || i == 70)
		{
			i = 0;
			print_height++;
			if (buf[0] != '\n')
				printf("\n");
			if (print_height >= SCREEN_HEIGHT)
				getc();
		}
		i++;
	}

	close(file_fd);
}