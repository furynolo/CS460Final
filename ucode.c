#include "type.h"

#define EOF -1

int  pid;
char line[64];
char pathname[32];
char i2[32];
char i3[32];
char *name[16];
char components[64];
int  nk;

 /////////////////////////////////////////////////////////////////////////////////
//																				//
//	  ====================== MTX System Call Functions ======================	//
//	 Number	Name			Usage						Function				//
//	 -----------------------------------------------------------------------	//
//	    0	getpid			getpid()					get process pid			//
//	    1	getppid			getppid()					get parent pid			//
//	    2	getpri			getpri()					get priority			//
//	    3	setpri			setpri(pri)					set priority			//
//	    4	getuid			getuid()					get uid					//
//	    5	chuid			chuid(uid,gid)				set uid,gid				//
//	    6	yield			yield()						switch process			//
//	    9	exit			exit(value)					terminate process		//
//	   10	fork			fork()						fork child process		//
//	  *11	exec			exec(cmd_line)				change image to a file	//
//	   12	wait			wait(&status)				wait for child to die	//
//	   13	vfork			vfork()						fork child process		//
//	   14	thread			thread(fd,stack,flag,prt)	create thread			//
//	   15	mutex_creat		mutex_creat()										//
//	   16	mutex_lock		mutex_lock(&mutex)									//
//	   17	mutex_unlock	mutex_unlock(&mutex)								//
//	   18	mutex_destroy	mutex_destroy(&mutex)								//
//	  ----------------------------------------------  -----------------------	//
//	   20	mkdir			mkdir(pathname)				make directory			//
//	   21	rmdir			rmdir(pathname)				rm   directory			//
//	   22	creat			creat(pathname)				creat file				//
//	   23	link			link(oldname, newname)		hard link to file		//
//	   24	unlink			unlink(pathname)			unlink					//
//	   25	symlink			symlink(oldname,newname)	create symbolic link	//
//	   26	readlink		readlink(name, buf[ ])		read symlink 			//
//	   27	chdir			chdir(pathname)				change dir				//
//	   28	getcwd			getcwd(buf[ ])				get cwd pathname		//
//	   29	stat			stat(filename, &stat_buf)	stat file				//
//	   30	fstat			fstat(fd, &stat_buf)		stat file descriptor	//
//	   31	open			open(filename, flag)		open file for R|W|APP	//
//	   32	close			close(fd)					close file descriptor 	//
//	  *33	lseek			lseek(fd, position)			lseek 					//
//	   34	read			read(fd, buf[ ], nbytes)	read file				//
//	   35	write			write(fd,buf[ ], nbytes)	write to file			//
//	   36	pipe			pipe(pd[ ])					carete pipe				//
//	   37	chmod			chmod(filename, mode)		change permission		//
//	   38	chown			chown(filname, uid)			change file owner		//
//	   39	touch			touch(filename)				change file time		//
//	   40	settty			settty(tty_name)			set proc.tty name		//
//	   41	gettty			gettty(buf[ ])				get proc.tty name		//
//	   42	dup				dup(fd)						dup file descriptor		//
//	   43	dup2			dup2(fd1, fd2)				dup fd1 to fd2			//
//	   44	ps				ps()						ps in kernel			//
//	   45	mount			mount(FS, mountPoint)		mount file system		//
//	   46	umount			umount(mountPoint)			umount file system		//
//	   47	getSector		getSector(sector, buf[ ])	read CDROM sector		//
//	   48	cd_cmd			cd_cmd(cmd)					issue cmd to CD driver	//
//	 ------------------------------------------------------------------------	//
//	   50	kill			kill(sig#, pid)				send signal to pid		//
//	   51	signal			signal(sig#, catcher)		install siganl handler	//
//	   52	pause			pause(t)					pause for t seconds		//
//	   53	itimer			itimer(sec, action)			set timer request		//
//	   54	send			send(msg, pid)				send msg to pid			//
//	   55	recv			sender=recv(msg)			receive msg 			//
//	   56	tjoin			tjoin(n)					threads join			//
//	   57	texit			texit(value)				tthread exit			//
//	   58	hits			hits()						I/O buffer hit ratio	//
//	   59	color			color(v)					change display color	//
//	   60	sync			sync()						sync file system		//
//	 ==========================================================================	//
//																				//
/////////////////////////////////////////////////////////////////////////////////


int getc()
{
	int c;
	int n;
	n = read(0, &c, 1);

	/********************************************************************* 
	getc from KBD will NOT get 0 byte but reading file (after redirect 0 
	to file) may get 0 byte ==> MUST return 2-byte -1 to differentiate.
	**********************************************************************/

	if (n == 0 || c == 4)
		return EOF;  
								
	return (c&0x7F);
}


// getline() does NOT show the input chars AND no cooking: 
// for reditected inputs from a file which may contain '\b' chars
int getline(char *s)
{
	int c;
	char *cp = s;

	c = getc();
	while (c != EOF && c != '\r' && c != '\n')
	{
		*cp++ = c;
		c = getc();
	}
	if (c==EOF)
		return 0;

	*cp++ = c;			// a string with last char=\n or \r
	*cp = 0;

	return strlen(s);	// at least 1 because last char=\r
}

clear_str(char *input, int length)
{
	int i = 0;
	
	while (i < length)
	{
		input[i] = 0;
		i++;
	}

	return;
}


// gets() show each input char AND cook input line
int gets(char *s)
{
	int c;
	char *cp, *cq, temp[128];

	cp = temp;			// get chars into temp[] first

	c = getc();
	while (c!= EOF && c != '\r' && c != '\n')
	{
		*cp++ = c;
		putc(c);
		c = getc();
	}
	putc('\r');
	putc('\n');

	if (c==EOF) return 0;

	*cp = 0;   

	// printf("temp=%s\n", temp);

	// cook line in temp[] into s
	cp = temp;
	cq = s; 

	while (*cp)
	{
		if (*cp == '\b')
		{
			if (cq > s)
				cq--;
			cp++;
			continue;
		}
		*cq++ = *cp++;
	}
	*cq = 0;

	//printf("s=%s\n", s);

	return strlen(s) + 1;	// line=CR or \n only return 1
}

int getpid()
{
	return syscall(0, 0, 0);
}

int getppid()
{
	return syscall(1, 0, 0);
}

int getpri()
{
	return syscall(2, 0, 0);
}

int chpri(int value)
{
	return syscall(3, value, 0);
}

int getuid()
{
	return syscall(4,0,0);
}

int chuid(int uid, int gid)
{
	return syscall(5, uid, gid);
}

int tswitch()
{
	return syscall(6, 0, 0);
}

int fork()
{
	return syscall(10, 0, 0);
}

int exec(char *cmd_line)
{
	return syscall(11, cmd_line, 0);
}

int wait(int *status)
{
	return syscall(12, status, 0);
}

int vfork()
{
	return syscall(13, 0, 0);
}

int thread(int fn, int stack, int flag, int ptr)
{
	return syscall(14, fn, stack, flag, ptr);
}

// 15-19: mutex for threads


int mutex_creat()
{
	return syscall(15, 0, 0);
}

int mutex_lock(int *m)
{
	return syscall(16, m, 0);
}

int mutex_unlock(int *m)
{
	return syscall(17, m, 0);
}

int mutex_destroy(int *m)
{
	return syscall(18, m, 0);
}


int mkdir(char *name)
{
	return syscall(20, name, 0);
}

int rmdir(char *name)
{
	return syscall(21, name, 0);
}

int creat(char *filename)
{
	return syscall(22, filename, 30);
}

int link(char *oldfile, char *newfile)
{
	return syscall(23, oldfile, newfile,0);
}

int unlink(char *file)
{
	return syscall(24, file, 0);
}

int symlink(char *oldfile, char *newfile)
{
	return syscall(25, oldfile, newfile);
}

int readlink(char *file, char *linkname)
{
	return syscall(26, file, linkname, 0);
}

int chdir(char *name)
{
	return syscall(27, name, 0);
}

int getcwd(char *cwdname)
{
	return syscall(28, cwdname, 0);
}

int stat(char *filename, struct stat *sPtr)
{   
	return syscall(29, filename, sPtr);
}

int fstat(int fd, struct stat *sptr)
{
	return syscall(30, fd, sptr, 0);
}

int open(char *file, int flag)
{
	return syscall(31, file, flag);
}

int close(int fd)
{
	return syscall(32, fd);
}

u32 lseek(int fd, u32 offset, int ww)
{
	return syscall(33, fd, (u32)offset, ww);
}

int read(int fd, char *buf, int nbytes)
{
	return syscall(34, fd, buf, nbytes);
}

int write(int fd, char *buf, int nbytes)
{
	return syscall(35, fd, buf, nbytes);
}

int pipe(int *pd)
{
	return syscall(36, pd, 0);
}

int chmod(char *file, u16 mode)
{
	return syscall(37, file, mode);
}

int chown(char *file, int uid)
{
	return syscall(38, file, uid);
}

int touch(char *filename)
{
	return syscall(39, filename, 0);
}

int fixtty(char *tty)
{
	return syscall(40, tty, 0);
}

int settty(char *tty)
{
	return syscall(40, tty, 0);
}

int gettty(char *tty)
{
	return syscall(41, tty, 0);
}

int dup(int fd)
{
	return syscall(42, fd, 0);
}

int dup2(int fd, int gd)
{
	return syscall(43, fd, gd);
}

int ps(char *y)
{
	return syscall(44, y, 0);
}

int mount(char *dev, char **mpt)
{
	return syscall(45, dev, mpt);
}

int umount(char *dev)
{
	return syscall(46, dev);
}

/********** CDROM syscalls ******************/
int getSector(u32 sector, char *ubuf, u16 nsector)
{
	return syscall(47, (u32)sector, ubuf, nsector);
}

int do_cmd(u16 cmd, u16 value)
{
	return syscall(48, cmd, value);
}

int kill(int sig, int pid)
{
	return syscall(50, sig, pid);
}

int signal(u16 sig, u16 catcher)
{
	return syscall(51, sig, catcher);
}

int pause(u16 t)
{
	return syscall(52, t);
}

int itimer(u16 t)
{
	return syscall(53, t);
}

int send(char *msg, int pid)
{
	syscall(54, msg, pid);
}

int recv(char *msg)
{
	syscall(55,msg, 0);
}

int do_texit()
{
	int pid = getpid();
	printf("thread %d texit()\n", pid);
	texit(pid);
}

int tjoin(int n)
{
	return syscall(56, n, 0);
}

int texit(int v)
{
	syscall(57, v, 0);
}

int khits(int *requests, int *hits)
{
	syscall(58, requests, hits);
}

int setcolor(int color)
{
	return syscall(59, color, 0);
}

int sync()
{
	return syscall(60, 0, 0);
}

int thinit()
{
	return syscall(61, 0, 0);
}

int exit(int value)
{
	syscall(9, value, 0);
}

int pwd()
{
	char cwd[64];
	getcwd(cwd);
	printf("%s\n", cwd);
}