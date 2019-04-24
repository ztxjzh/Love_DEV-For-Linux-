
//
// Created by zhangtx on 2018/4/24.
//
//MAKE USE g++ test_get.cpp -std=c++11 -lpthread

#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string>

using namespace std;

char test_order[512] = {"OBJECT"};
long nob = 0;
long retime = 1,i_time = 0;

int Random(unsigned long long start, unsigned long long end){
    unsigned long long dis = end - start;
    return rand() % dis + start;
}

void *connect_ncos(void *arg) {
  while(i_time--){
    srand((unsigned long long)getpid());
    
    struct timeval t_start, t_end;
    long cost_time = 0;
    gettimeofday(&t_start, NULL);
    long start05 = ((long) t_start.tv_sec) * 1000 + (long) t_start.tv_usec / 1000;
    
    unsigned long addnum = Random(1, 10);
    std::string random_num(to_string(addnum));

    std::string get_obj = "OBJECT";
    get_obj += random_num;
    std::string exec_run = "s3cmd get s3://BUCKET/" + get_obj;
    exec_run += "  --force";
    const char* order = exec_run.c_str();
    int ret = system(order);
    char OBJ_NAME[128] = {};
    sprintf(OBJ_NAME, "OBJECT_TEST%d", getpid());
    
    gettimeofday(&t_end, NULL);
    long end05 = ((long) t_end.tv_sec) * 1000 + (long) t_end.tv_usec / 1000;
    cost_time = end05 - start05;
    
    char log_out[128];
    sprintf(log_out, "echo proc[%lu]:Take -%ld-ms >> ./_TAKE.log ", (unsigned long) getpid(), cost_time);
    system(log_out);
  }
    return NULL;
}


int main(int argc, char *argv[]) {
    pid_t pid = 0;
    srand((unsigned long long)time(NULL));   
    FILE *open_order = NULL;
    open_order = fopen("./_test.order", "r");
    if (open_order == NULL) {
        perror("Read order err. run default. ");
    } else {
      fscanf(open_order, "%[^\n]s", test_order);
      printf("USE ORDER:%s\n",test_order);
    }
    
    printf("Father: pid->[%d]\n\n", getpid());
    i_time = (int) strtoull(argv[1], NULL, 10);
    if (argv[1] == NULL || 0 == strncasecmp(argv[1], "--help", 6)) {
        printf("TEST_START --[cycles number] [concurrent number]\n OP = $1 * $2\n");
        exit(0);
    }
    
    if (argv[2] == NULL || argv[2][0] == '\0') {
        printf("NOT FOUND CONCURRENT NUMBER ,SET IT\n");
        exit(0);
    }
    
    int i = 0, n = (int) strtoull(argv[2], NULL, 10);
    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork error");
            return -1;
        } else if (pid == 0) {
            //printf("child: fpid==[%d], cpid==[%d]\n", getppid(), getpid());
            break;
        }
    }
    
    if (pid < 0) {
        perror("fork error");
        return -1;
    } else if (pid > 0) {
        int status;
        while (1) {
            pid_t wpid = waitpid(pid, &status, WNOHANG);
            
            if (wpid == 0) {
                sleep(3);
                continue;
            } else if (wpid == -1) {
                printf("no child is living.\n");
                exit(0);
            } else if (wpid > 0) {
                if (WIFEXITED(status)) {
                    printf("child nornal exit,status==[%d]\n", WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("child killed by signo==[%d]\n", WTERMSIG(status));
                }
            }
        }
    } else if (pid == 0) {
        connect_ncos(argv[1]);
        return 11;
    }
    
    return 0;
}

