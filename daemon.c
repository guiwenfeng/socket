#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void daemonize(void);

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-d") == 0) daemonize();
    
    //sleep(60); // 进程状态：S
    while(1)     // 进程状态：R
    {
        // ...
    }
    
    return 0;
}

void daemonize(void)
{
    int fd;

    if (fork() != 0) exit(0); /* parent exits */
    
    setsid(); /* create a new session */

    /* Every output goes to /dev/null. */
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO) close(fd);
    }
}