#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * @brief 演示父进程创建子进程并分别输出进程信息。
 * @param 无。
 * @input 无标准输入。
 * @output 向标准输出打印父进程与子进程的PID、PPID信息。
 * @return 成功返回0，失败返回1。
 */
int main(void) {
    /* 调用fork创建子进程，返回值用于区分父子执行路径。 */
    pid_t pid = fork();

    /* fork失败时直接报错并退出。 */
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    /* 子进程分支：打印自身PID和父进程PID。 */
    if (pid == 0) {
        printf("[子进程] PID=%d, PPID=%d\n", getpid(), getppid());
    } else {
        /* 父进程分支：打印自身PID以及新创建子进程PID。 */
        printf("[父进程] PID=%d, 子进程PID=%d\n", getpid(), pid);
    }

    return 0;
}
