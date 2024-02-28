#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		write(2, "Usage: sleep <time>\n", 20);
		exit(1);
	}
	int sleepTime = atoi(argv[1]);
	sleep(sleepTime);
	exit(0);
}
