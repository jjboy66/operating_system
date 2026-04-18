#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief 演示wait()阻塞效果：父进程在子进程结束前一直等待。
 * @param 无。
 * @input 无标准输入。
 * @output 打印等待前后日志、子进程退出状态。
 * @return 成功返回0，失败返回1。
 */
int main(void) {
    /* 创建子进程，用于演示父进程wait阻塞。 */
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        /* 子进程模拟耗时任务。 */
        printf("[子进程] 开始执行耗时任务...\n");
        sleep(2);
        printf("[子进程] 任务结束，准备退出。\n");
        exit(42);
    } else {
        int status = 0;

        printf("[父进程] 调用wait()，此处将阻塞直到子进程结束。\n");

        /* wait阻塞父进程，直到任一子进程结束并返回其PID。 */
        pid_t done = wait(&status);
        if (done < 0) {
            perror("wait failed");
            return 1;
        }

        /* 根据status解析子进程是否正常退出及退出码。 */
        if (WIFEXITED(status)) {
            printf("[父进程] 子进程PID=%d正常退出，退出码=%d\n", done, WEXITSTATUS(status));
        } else {
            printf("[父进程] 子进程PID=%d非正常结束。\n", done);
        }
    }

    return 0;
}
