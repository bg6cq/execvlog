#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 原始execve函数指针
static int (*original_execve)(const char *filename, char *const argv[], char *const envp[]) = NULL;

// 日志文件路径
#define LOG_FILE "/tmp/execvlog.log"

// 获取当前时间字符串
void get_time_str(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

// 写入日志
void write_log(const char *filename, char *const argv[]) {
    char time_buf[64];
    get_time_str(time_buf, sizeof(time_buf));
    
    // 构建命令行字符串
    char cmdline[1024] = {0};
    if (argv) {
        for (int i = 0; argv[i] != NULL; i++) {
            if (i > 0) strncat(cmdline, " ", sizeof(cmdline) - strlen(cmdline) - 1);
            strncat(cmdline, argv[i], sizeof(cmdline) - strlen(cmdline) - 1);
        }
    }
    
    // 打开日志文件（追加模式）
    FILE *log = fopen(LOG_FILE, "a");
	
    if (log) {
        fprintf(log, "[%s] UID=%d PID=%d CMD=%s ARGS=%s\n", 
                time_buf, getuid(), getpid(), filename, cmdline);
        fclose(log);
    }
}

// 重写的execve函数
int execve(const char *filename, char *const argv[], char *const envp[]) {
    // 初始化原始execve函数
    if (!original_execve) {
        original_execve = dlsym(RTLD_NEXT, "execve");
    }
    
    // 记录进程信息
    write_log(filename, argv);
    
    // 调用原始execve函数
    return original_execve(filename, argv, envp);
}

// 构造函数，在库加载时自动执行
__attribute__((constructor)) void init(void) {
    // 确保日志文件存在并可写
    int fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd >= 0) {
        close(fd);
    }
}
