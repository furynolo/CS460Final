#include "ucode.c"

#define MAX_INPUT_LEN 128
#define MAX_NUM_PIPES 32
#define MAX_FILENAME_LEN 32

int child;
int pipe_index[MAX_NUM_PIPES], infile_index, outfile_index, append_index;
int pipe_index_counter;

char input[MAX_INPUT_LEN];
char infilename[MAX_FILENAME_LEN], outfilename[MAX_FILENAME_LEN], appendfilename[MAX_FILENAME_LEN];

main(int argc, char *argv[])
{
	// int i;
	int infile_fd, outfile_fd, appendfile_fd;

	while (1)
	{
		// New input so no known locations of pipes, infile, outfile, and append indexes.
		reset_input();

		printf("\n$ ");
		gets(input);

		if (find_special_characters())
		{
			// we have at least 1 of the following: pipe(s), infile, outfile, append.
			if (infile_index > -1)
			{
				get_infilename();
printf("(debug): infilename = %s\n", infilename);
				// We dont open infile yet, only the last child process will use it.
			}
			if (outfile_index > -1)
			{
				get_outfilename();
printf("(debug): outfilename = %s\n", outfilename);
				open_outfile();
			}
			if (append_index > -1)
			{
				get_appendfilename();
printf("(debug): appendfilename = %s\n", appendfilename);
				open_appendfile();
			}
			if (pipe_index[0] > -1)
			{

			}

			// 
		}
		else
		{
			child = fork();
			if (child)
				wait(child);
			else
				exec(input);
		}
	}
}

int find_special_characters()
{
	int value, i;

	value = 0;

	while ((input[i] != '\n') || (input[i] != 0))
	{
		switch (input[i])
		{
			case '|' :	pipe_index[pipe_index_counter] = i;
						pipe_index_counter++;
						value = 1;
						break;
			case '<' :	infile_index = i;
						value = 1;
						break;
			case '>' :	i++;
						if (input[i] == '>')
							append_index = i;
						else
							outfile_index = i - 1;
						value = 1;
						break;
		}

		i++;
	}

	return value;
}

int reset_input ()
{
	int i;

	i = 0;

	pipe_index_counter = 0;

	for (i = 0; i < MAX_NUM_PIPES; i++)
		pipe_index[i] = -1;

	for (i = 0; i < MAX_FILENAME_LEN; i++)
	{
		infilename[i] = 0;
		outfilename[i] = 0;
		appendfilename[i] = 0;
	}

	infile_index = outfile_index = append_index = -1;

	return 1;
}

// Set infilename[] with the name of the file to recieve input from.
int get_infilename()
{
	int value;
	int input_i, infile_j;
	
	value 	= 0;
	input_i = infile_j = 0;

	while (input_i < infile_index)
	{
		switch (input[input_i])
		{
			case ' '  :	
			case '\t' :	
			case '\r' :	
			case '\n' :	break;
			default   :	infilename[infile_j] = input[input_i];
						infile_j++;
						value = 1;
						break;
		}

		input_i++;
	}

	return value;
}

// Set outfilename[] with the name of the file to send output to.
int get_outfilename()
{
	int value;
	int input_i, outfile_j;
	
	value 	  = 0;
	input_i   = outfile_index + 1;
	outfile_j = 0;

	while (input[input_i] != 0)
	{
		switch (input[input_i])
		{
			case ' '  :	
			case '\t' :	break;
			default   :	outfilename[outfile_j] = input[input_i];
						outfile_j++;
						value = 1;
						break;
		}

		input_i++;
	}

	return value;
}

// Set appendfilename[] with the name of the file to append to.
int get_appendfilename()
{
	int value;
	int input_i, appendfile_j;
	
	value 	  = 0;
	input_i   = appendfile_index + 1;
	appendfile_j = 0;

	while (input[input_i] != 0)
	{
		switch (input[input_i])
		{
			case ' '  :	
			case '\t' :	break;
			default   :	appendfilename[appendfile_j] = input[input_i];
						appendfile_j++;
						value = 1;
						break;
		}

		input_i++;
	}

	return value;
}

int open_infile()
{
	int infile_fd;
	infile_fd = open(infilename, O_RDONLY);

	if (infile_fd == -1)
	{
		printf("Unable to open %s.\n", infilename);
	}

	return infile_fd;
}

int open_outfile()
{
	int outfile_fd;
	outfile_fd = open(outfilename, O_WRONLY);

	if (outfile_fd == -1)
	{
		printf("Unable to open %s.\n", outfilename);
	}

	return outfile_fd;
}

int open_appendfile()
{
	int appendfile_fd;
	appendfile_fd = open(appendfilename, O_RDWR);

	if (appendfile_fd == -1)
	{
		printf("Unable to open %s.\n", appendfilename);
	}

	return appendfile_fd;
}