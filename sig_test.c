#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIG_NUM_H	61
#define SIG_NUM_R	62
#define BUF_SIZE	512
#define DEBUG_LVL	1

#if 1
static int fd_cnt;
char file_path[PATH_MAX];
#define DIR_PATH	"/mnt/extra";
#if 0
if (fcntl(fd, F_GETPATH, filePath) != -1)
{
    // do something with the file path
}
int fstat(int fd, struct stat *buf);
#endif
/* implementation of Donal Fellows method */ 
int get_num_fds()
{
     char buf[64];
     struct dirent *d;
     DIR *dp = NULL;

printf("FD_SETSIZE: %d\n", FD_SETSIZE);
     snprintf(buf, 64, "/proc/%i/fd/", getpid());

     fd_cnt = 0;
     dp = opendir(buf);
     while ((d = readdir(dp)) != NULL) {
          fd_cnt++;
     }
     closedir(dp);
     return fd_cnt;
}
#endif

void sig_handler_h(int sig)
{
int i;
	printf("%s(): signal number: %d\n", __func__, sig);
get_num_fds();
	printf("fd_cnt: %d\n", fd_cnt);	

for (i=0;i < fd_cnt; i++) {
if (fcntl(i, F_GETPATH, file_path) != -1) {
if (!strncmp(file_path, DIR_PATH, strlen(DIR_PATH)))
	printf("fd: %d, PATH: %s\n", i, file_path);
}
}

}
void sig_handler_r(int sig)
{
	printf("%s(): signal number: %d\n", __func__, sig);

}

int main(int argc, char **argv)
{

	int fd = -1;
	char buf[BUF_SIZE] = {0};
	int count = 0;
	int ret = 0;
	int debug = 0;

if (argc > 2) {
	printf("<%s> <1>\n", argv[0]);
	return -1;
}

if (argc == 2)	
	debug = atoi(argv[1]);

	fd = open("/mnt/extra/test", O_RDWR|O_CREAT|O_TRUNC, 0666);
	if (fd < 0)
		perror("open");


	signal(SIG_NUM_H, sig_handler_h);
	signal(SIG_NUM_R, sig_handler_r);

	printf("%s(): signal registered\n", __func__);

	ret = sprintf(buf, "%08d\n", count);
#if 0
	ret = write(fd, buf, ret);//sizeof(buf));
#else
	ret = write(fd, buf, sizeof(buf));
#endif
	while(1) {
		memset(buf, 0, sizeof(buf));
#if 0
		lseek(fd, -ret, 1);
		read(fd, buf, ret);
#else
		lseek(fd, -ret, 1);
		read(fd, buf, ret);
#endif
	if (debug)
		printf("count: %d, buf: %s", count, buf);

		count++;
		sleep(1);
		memset(buf, 0, sizeof(buf));
		ret = sprintf(buf, "%08d\n", count);
#if 0
		ret = write(fd, buf, ret);//sizeof(buf));
#else
		ret = write(fd, buf, sizeof(buf));
#endif
	}
}
