#include "Functions.h"
#include <sys/stat.h>
#include <string>

void daemon(std::vector<std::string> finanse_need_cfg)

{
    pid_t pid, sid;

    pid = fork();
    if(pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if(sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //daemon_work(std::move(finanse_need_cfg));

    exit(EXIT_SUCCESS);
}