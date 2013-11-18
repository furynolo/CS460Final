 //////////////////////////////////////////////////////////////////////////////////
// After mounting the root file system, P0 creats P1, whose Umode image is 		 //
// the /bin/init program. P1 will go Umode directly to execute /bin/init, 		 //
// in which it forks children P2 on /dev/tty0. If the VM supports serial ports,	 //
// P1 may also fork P3 /dev/ttyS0 and P4 on /dev/ttyS1. Then P1 waits for any 	 //
// child to die. Henceforth, P1 plays the role of the INIT process (P1) in Unix. //
//////////////////////////////////////////////////////////////////////////////////

 //////////////////////////////////////////////////////////////////////////////////
//                      Logic of init.c:										 //
// NOTE: this init.c creates only ONE login process on console=/dev/tty0		 //
// YOUR init.c must also create login processes on serial ports /dev/ttyS0		 //
// and /dev/ttyS1..																 //
//////////////////////////////////////////////////////////////////////////////////

#include "ucode.c"

int pid, child_tty0, child_ttyS0, child_ttyS1, status;
int stdin, stdout;

main(int argc, char *argv[])
{
	stdin = stdout = -1;
	init();

	// P1 waits for tty0, ttyS0, ttyS1 to die and forks a new login for them.
	// It also buries orphaned children
	while(1)
	{
		printf("Init(): P1 waiting .....\n");

		pid = wait(&status);

		if (pid == child_tty0)
		{
			//fork another login child
			init_tty0();
		}
		else if (pid == child_ttyS0)
		{
			//fork another login child
			init_ttyS0();
		}
		else if (pid == child_ttyS1)
		{
			//fork another login child
			init_ttyS1();
		}
		else
		{
			printf("Init():  buried orphan child %d\n", pid);
		}
	}
}

int init()
{
	//printf("Init() : fork a login task on console.\n"); 
	// child = fork();
	// if (child)
	// 	parent();
	// else
	// 	exec("login /dev/tty0");
	init_tty0();

	init_ttyS0();

	init_ttyS1();

	return 1;
}

int init_tty0()
{
	if (stdin != -1)
		close(stdin);
	if (stdout != -1)
		close(stdout);

//	1.	Open /dev/tty0 as 0 (READ) and 1 (WRITE) in order to display messages
	stdin  = open("/dev/tty0", O_RDONLY);
	stdout = open("/dev/tty0", O_WRONLY);
//	2.	Now we can use printf, which calls putc(), which writes to stdout
	printf("Init() : fork a login task on console.\n"); 
	child_tty0 = fork();

	if (child_tty0 == 0)
	{
		// login task
		exec("login /dev/tty0");
	}

	return 1;
}

int init_ttyS0()
{
	if (stdin != -1)
		close(stdin);
	if (stdout != -1)
		close(stdout);

//	1.	Open /dev/ttyS0 as 0 (READ) and 1 (WRITE) in order to display messages
	stdin  = open("/dev/ttyS0", O_RDONLY);
	stdout = open("/dev/ttyS0", O_WRONLY);
//	2.	Now we can use printf, which calls putc(), which writes to stdout
	printf("Init() : fork a login task on console.\n"); 
	child_ttyS0 = fork();

	if (child_ttyS0 == 0)
	{
		// login task
		exec("login /dev/ttyS0");
	}

	return 1;
}

int init_ttyS1()
{
	if (stdin != -1)
		close(stdin);
	if (stdout != -1)
		close(stdout);

//	1.	Open /dev/ttyS1 as 0 (READ) and 1 (WRITE) in order to display messages
	stdin  = open("/dev/ttyS1", O_RDONLY);
	stdout = open("/dev/ttyS1", O_WRONLY);
//	2.	Now we can use printf, which calls putc(), which writes to stdout
	printf("Init() : fork a login task on console.\n"); 
	child_ttyS1 = fork();

	if (child_ttyS1 == 0)
	{
		// login task
		exec("login /dev/ttyS1");
	}
	
	return 1;
}

int login()
{
	return exec("login /dev/tty0");
}

int parent()
{
	while(1)
	{
		printf("Init() : waiting .....\n");

		pid = wait(&status);

		if (pid == child)
		{
			//fork another login child

		}
		else
		{
			printf("Init():  buried orphan child %d\n", pid);
		}
	}
}