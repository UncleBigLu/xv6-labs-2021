#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_IT_CNT 36
#define START_IT_CNT 1

void fork_and_wait(int cnt)
{
    cnt++;
    if(cnt == MAX_IT_CNT)
    {
        return;
    }
    int p[2];
    if(pipe(p) < 0){
        fprintf(2, "%d pipe failed\n", cnt);
        exit(1);
    }
    int pid = fork();
    if(pid > 0){
        close(p[0]);
        if(write(p[1], &cnt, 4) != 4){
            fprintf(2, "%d write failed\n", cnt);
            exit(1);
        }
        wait(0);
        exit(0);
    }
    else if(pid == 0){
        close(p[1]);
        int buf;
        if(read(p[0], &buf, 4) != 4){
            fprintf(2, "%d read failed\n", cnt);
            exit(1);
        }
        printf("prime %d\n", buf);
        close(p[0]);
        fork_and_wait(cnt);
    }
}

int
main(int argc, char *argv[])
{
    fork_and_wait(START_IT_CNT);
    exit(0);
}

