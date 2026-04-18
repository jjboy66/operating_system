#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief 父进程执行本地循环任务，子进程通过exec执行完全不同程序，实现并发。
 * @param 无。
 * @input 无标准输入。
 * @output 打印父进程循环日志及子进程exec程序输出。
 * @return 成功返回0，失败返回1。
 */
int main(void) {
    /* fork后子进程将调用exec替换自身映像。 */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        /* 子进程：使用execl执行/bin/ls，程序行为与父进程完全不同。 */
        printf("[子进程] 即将exec执行 /bin/ls -l\n");
        execl("/bin/ls", "ls", "-l", (char *)NULL);

        /* 仅在exec失败时执行此分支。 */
        perror("execl failed");
        exit(1);
    } else {
        /* 父进程：并发执行自身任务，体现与子进程同时推进。 */
        for (int i = 1; i <= 3; ++i) {
            printf("[父进程] 正在执行第 %d 轮本地任务...\n", i);
            sleep(1);
        }

        /* 等待子进程结束，回收资源。 */
        wait(NULL);
        printf("[父进程] 子进程已结束。\n");
    }

    return 0;
}
