// Q1
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
int main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	struct stat filestat;
	if (stat(argv[1], &filestat) < 0)
		return 1;
	printf("%s details : \n", argv[1]);
	printf(" file size: %ld \n", filestat.st_size);
	printf(" No.of hard links: %ld \n", filestat.st_nlink);
	printf(" file inode: %ld \n", filestat.st_ino);
	printf(" file permissions:");
	printf(S_ISDIR(filestat.st_mode) ? "d" : "-");
	printf((filestat.st_mode & S_IWUSR) ? "w" : "-");
	printf((filestat.st_mode & S_IXUSR) ? "x" : "-");
	printf((filestat.st_mode & S_IRGRP) ? "r" : "-");
	printf((filestat.st_mode & S_IWGRP) ? "w" : "-");
	printf((filestat.st_mode & S_IXGRP) ? "x" : "-");
	printf((filestat.st_mode & S_IROTH) ? "r" : "-");
	printf((filestat.st_mode & S_IWOTH) ? "w" : "-");
	printf((filestat.st_mode & S_IXOTH) ? "x" : "-");

	printf("\n");
	char timestr[50];
	struct tm *modified_time = localtime(&filestat.st_mtime);
	strftime(timestr, sizeof(timestr), "%b %d %l:%M %p", modified_time);
	printf("Modified time: %s \n", timestr);

	struct tm *access_time = localtime(&filestat.st_atime);
	strftime(timestr, sizeof(timestr), "%b %d %l : %M %p", access_time);
	printf("Access time: %s \n", timestr);

	return 0;
}

// Q2
#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
void child_process_code()
{
	int j, k = 5000;
	char filename[10] = "hello";
	FILE *fp = fopen(filename, "a");
	for (j = 0; j < 10; j++)
	{
		while (k--)
			;

		k = 5000;
		fprintf(fp, "%d - ", j);
	}
	fprintf(fp, "\n");
	fclose(fp);
}
void main()
{
	int status = 0, i;
	struct rusage usage;
	for (i = 0; i < 10; i++)
	{
		if (fork() == 0)
		{
			child_process_code();
			exit(0);
		}
	}
	while ((wait(&status)) > 0)
		;
	getrusage(RUSAGE_CHILDREN, &usage);
	printf("Total time spent in user mode by children : %ld s %ld ms\n",
		   usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
	printf("Total time spent in kernel mode by children : %ld s %ld ms\n",
		   usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
}