#include "types.h"
#include "user.h"

#define NUMBER_OF_PROCESSES 10
#define ONE 1

int main(int argc, char const *argv[])
{
	int pid = getpid();
	int i;

	for (i = ONE; i < NUMBER_OF_PROCESSES; ++i)
	{
		if (pid > 0)
		{
			pid = fork();
		}
	}

	if(pid < 0)
    {
        printf(2, "fork failed!\n");
    }
	else if (pid == 0)
	{
		printf(1, "%d\n", getsyscallnum());
	}
	else
	{
		for (i = 0; i < NUMBER_OF_PROCESSES; i++)
			wait();
		printf(1, "Per-cpu test is finished!\n");
	}
	
	exit();
}
