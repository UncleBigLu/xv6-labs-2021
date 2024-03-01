#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int p1[2]; 
	int p2[2];
	pipe(p1);
	pipe(p2);
	
	if(fork() == 0){
		uint8 rx_buf;
		close(p1[1]);
		close(p2[0]);
		if(read(p1[0], &rx_buf,1) < 0)
		{
			exit(1);
		}
		printf("%d: received ping\n", getpid());
		if(write(p2[1], &rx_buf, 1) < 0)
		{	
			exit(1);
		}
		close(p1[0]);
		close(p2[1]);
		exit(0);
	}
	else {
		uint8 rx_buf, tx_buf = 0;
		close(p2[1]);
		close(p1[0]);
		if(write(p1[1], &tx_buf, 1) < 0)
		{
			fprintf(2, "parent write pipe error\n");
			exit(1);
		}
		if(read(p2[0], &rx_buf, 1) < 0)
		{
			fprintf(2, "parent read pipe error\n");
			exit(1);
		}
		printf("%d: received pong\n", getpid());
		close(p2[0]);
		close(p1[1]);
		exit(0);
	}
}
