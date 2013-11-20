#include "ucode.c"

#define BUFSIZE 256
#define FILE_NAME_LENGTH 32

main(int argc, char *argv[])
{
	char filename[FILE_NAME_LENGTH], buf[BUFSIZE];
	int file_fd, bytes_read, i;

	bytes_read = BUFSIZE;

	strcpy(filename, argv[1]);

	file_fd = open(filename, O_RDONLY);

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

			
}