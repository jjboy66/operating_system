#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * @brief 输出当前进程信息，便于观察进程家族树结构。
 * @param role 进程角色字符串（如“父进程”“第一代子进程”）。
 * @input role由调用者传入。
 * @output 向标准输出打印角色、PID、PPID信息。
 * @return 无返回值。
 */
void print_process_info(const char *role) {
    /* 输出角色与进程标识，用于构建实验观察结果。 */
    printf("[%s] PID=%d, PPID=%d\n", role, getpid(), getppid());
}

/**
 * @brief 构建一个简单三层进程家族树（父 -> 子 -> 孙）。
 * @param 无。
 * @input 无标准输入。
 * @output 打印父进程、子进程、孙进程的层级信息。
 * @return 成功返回0，失败返回1。
 */
int main(void) {
    /* 先输出父进程信息作为树根。 */
    print_process_info("父进程(树根)");

    /* 第一次fork：父进程创建第一代子进程。 */
    pid_t first_child = fork();
    if (first_child < 0) {
        perror("fork first child failed");
        return 1;
    }

    if (first_child == 0) {
        /* 第一代子进程分支：输出自身信息。 */
        print_process_info("第一代子进程");

        /* 第二次fork：第一代子进程创建第二代子进程（孙进程）。 */
        pid_t grandchild = fork();
        if (grandchild < 0) {
            perror("fork grandchild failed");
            return 1;
        }

        if (grandchild == 0) {
            /* 孙进程分支：输出信息后结束。 */
            print_process_info("第二代子进程(孙进程)");
            exit(0);
        }

        /* 第一代子进程等待孙进程结束，保持层级关系清晰。 */
        if (wait(NULL) < 0) {
            perror("wait grandchild failed");
            exit(1);
        }
        exit(0);
    }

    /* 父进程等待第一代子进程结束，避免僵尸进程。 */
    if (wait(NULL) < 0) {
        perror("wait first child failed");
        return 1;
    }
    return 0;
}
