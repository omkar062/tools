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
#include <errno.h>

#define SIG_NUM_H	61
#define SIG_NUM_R	62
#define BUF_SIZE	512
#define DEBUG_LVL	1

#if 1
static int fd_cnt;
char file_path[PATH_MAX];
char fd_path[PATH_MAX];
#define DIR_PATH	"/mnt/extra"
#if 0
if (fcntl(fd, F_GETPATH, filePath) != -1)
{
    // do something with the file path
}
int fstat(int fd, struct stat *buf);
#endif
/* implementation of Donal Fellows method */ 
#if 0
void print_type(char *type, struct pid_info_t* info)
{
    static ssize_t link_dest_size;
    static char link_dest[PATH_MAX];
    strncat(info->path, type, sizeof(info->path));
    if ((link_dest_size = readlink(info->path, link_dest, sizeof(link_dest)-1)) < 0) {
        if (errno == ENOENT)
            goto out;
        snprintf(link_dest, sizeof(link_dest), "%s (readlink: %s)", info->path, strerror(errno));
    } else {
        link_dest[link_dest_size] = '\0';
    }
    // Things that are just the root filesystem are uninteresting (we already know)
    if (!strcmp(link_dest, "/"))
        goto out;
    printf("%-9s %5d %10s %4s %9s %18s %9s %10s %s\n",
            info->cmdline, info->pid, info->user, type,
            "???", "???", "???", "???", link_dest);
out:
    info->path[info->parent_length] = '\0';
}
#endif
int get_num_fds()
{
//     char buf[64];
     struct dirent *d;
     DIR *dp = NULL;

printf("FD_SETSIZE: %d\n", FD_SETSIZE);
     snprintf(fd_path, 64, "/proc/%i/fd/", getpid());

     fd_cnt = 0;
     dp = opendir(fd_path);
     while ((d = readdir(dp)) != NULL) {
printf("%s\n", d->d_name);
if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
                continue;
          fd_cnt++;
     }
     closedir(dp);
     return fd_cnt;
}
#endif

void sig_handler_h(int sig)
{
int i;
int ret = 0;
	printf("%s(): signal number: %d\n", __func__, sig);
get_num_fds();
	printf("fd_cnt: %d\n", fd_cnt);	
#if 1
for (i=0;i < fd_cnt; i++) {
//strcat(fd_path, );
snprintf(fd_path, PATH_MAX, "/proc/%i/fd/%i", getpid(),i);
printf("fd_path: %s\n", fd_path);

ret = readlink(fd_path, file_path, sizeof(file_path)-1);
if (ret < 0) {
if (errno != ENOENT)
perror("readlink");
break;
} else {
file_path[ret] = '\0';
printf("fd: %d, PATH: %s\n", i, file_path);
}

if (!strncmp(file_path, DIR_PATH, strlen(DIR_PATH))) {
printf("fd: %d, ONLY PATH: %s\n", i, file_path);
}
}
#endif

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

	printf("%s PID: %d\n", argv[0], getpid());
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
