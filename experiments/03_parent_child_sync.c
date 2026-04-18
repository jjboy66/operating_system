#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief 使用管道完成父子进程同步：父进程等待子进程“准备完成”信号。
 * @param 无。
 * @input 无标准输入。
 * @output 打印父子同步过程日志。
 * @return 成功返回0，失败返回1。
 */
int main(void) {
    int pipefd[2];

    /* 创建无名管道：pipefd[0]读端，pipefd[1]写端。 */
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return 1;
    }

    /* fork创建子进程，父子共享管道描述符。 */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        char signal = 'R';

        /* 子进程关闭读端，仅保留写端发送同步信号。 */
        close(pipefd[0]);

        /* 子进程执行准备工作（用sleep模拟）。 */
        printf("[子进程] 正在准备数据...\n");
        sleep(1);

        /* 向父进程发送“就绪”信号。 */
        if (write(pipefd[1], &signal, sizeof(signal)) != sizeof(signal)) {
            perror("child write failed");
            close(pipefd[1]);
            exit(1);
        }

        printf("[子进程] 已发送同步信号。\n");
        close(pipefd[1]);
        exit(0);
    } else {
        char signal = '\0';

        /* 父进程关闭写端，仅保留读端等待子进程信号。 */
        close(pipefd[1]);

        printf("[父进程] 等待子进程就绪...\n");

        /* 阻塞读：未收到子进程信号前父进程不会继续执行。 */
        if (read(pipefd[0], &signal, sizeof(signal)) != sizeof(signal)) {
            perror("parent read failed");
            close(pipefd[0]);
            wait(NULL);
            return 1;
        }

        /* 收到信号后执行后续任务。 */
        if (signal == 'R') {
            printf("[父进程] 收到就绪信号，开始后续处理。\n");
        }

        close(pipefd[0]);
        wait(NULL);
    }

    return 0;
}
