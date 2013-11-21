#include "ucode.c"

#define BUFSIZE 256
#define FILE_NAME_LENGTH 32

main(int argc, char *argv[])
{
	char filename[FILE_NAME_LENGTH], buf[BUFSIZE];
	int file_fd, bytes_read, i;

	printf("\n^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^\n");
	printf("^v                 cat                   v^\n");
	printf("^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^\n\n");

	bytes_read = BUFSIZE;

	strcpy(filename, argv[1]);

	file_fd = open(filename, O_RDONLY);

	if (file_fd == -1)
	{
		// This file does not exist in the specified directory.
		printf("Invalid arguments.  Expected format:\n\n");
		printf("cat filename\n\n");
		return;
	}

	while (bytes_read == BUFSIZE)
	{
		i = 0;
		bytes_read = read(file_fd, buf, BUFSIZE);
		while (i < bytes_read)
		{
			putc(buf[i]);
			i++;
		}
	}

	close(file_fd);
}