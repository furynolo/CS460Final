#include "ucode.c"

#define BUFSIZE 1
#define FILE_NAME_LENGTH 32
#define SCREEN_HEIGHT 23
#define SCREEN_WIDTH 70

main(int argc, char *argv[])
{
	char filename[FILE_NAME_LENGTH], buf[BUFSIZE], tty[64];
	int file_fd, bytes_read, i, print_height, value;
	char input;

	value = 0;

	// printf("\n^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^\n");
	// printf("^v                 more                  v^\n");
	// printf("^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^\n\n");
	
	// Check if the number of arguments is correct.
	if (argc < 1)
	{
		// There are not enough arguments.
		printf("Too few arguments.  Expected format:\n\n");
		printf("more filename\n\n");
		return;
	}
	else if (argc > 2)
	{
		// There are too many arguments.
		printf("Too many arguments.  Expected format:\n\n");
		printf("more filename\n\n");
		return;
	}

	print_height = 0;
	bytes_read = BUFSIZE;

	if (argc > 1)
	{
		strcpy(filename, argv[1]);
	
		file_fd = open(filename, O_RDONLY);
	
		if (file_fd == -1)
		{
			// This file does not exist in the specified directory.
			printf("Unable to open %s.  Expected format:\n\n", filename);
			printf("more filename\n\n");
			return;
		}
	}
	
	i = 0;

	while (bytes_read == BUFSIZE)
	{
		if (argc > 1)
			bytes_read = read(file_fd, buf, BUFSIZE);
		else
		{
			// value = getc();// buf[0] = getc();
			// buf[0] = value;
			bytes_read = read(0, buf, BUFSIZE);
		}
		// if (value == -1)
		// 	return;
		putc(buf[0]);
printf("[i = %d]", i);
		// printf("%c", buf[0]);

		if (buf[0] == '\n' || i >= SCREEN_WIDTH)
		{
			i = 0;
			print_height++;
			if ((buf[0] != '\n') && (argc > 1))
				printf("\n");
			if (print_height >= SCREEN_HEIGHT)
			{
				// Get a character from the user before printing next line.
				if (argv == 1)
				{
					// stdin needs to come from user terminal.
					file_fd = dup(0);
					close(0);
					gettty(tty);
					open(tty, O_RDONLY);
				}
				value = getc();
				input = value;
				if (argv == 1)
				{
					// fd 0 needs to go back to whatever it was.
					close(0);
					open(file_fd, O_RDONLY);
					close(file_fd);
				}
				if (value == -1)		// Pressing Ctrl-D terminates.
					break;
				else if (input == ' ')	// Pressing space bar moves the screen 1 full page.
					print_height = 0;
			}
		}
		i++;
	}
	if (argc > 1)
		close(file_fd);
}