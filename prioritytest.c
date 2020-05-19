#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "prioritylock.h"

#define NUMBER_OF_PROCESSES 10
#define ONE 1

int main(int argc, char const *argv[])
{
	struct prioritylock *lk;
	char *name = "test";
	int pid = getpid();
	int i;

	init_prioritylock(lk, name);

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
		acquire_priority(lk);
        int z = 1;
        for(int j = 0; j < 20000000; j+=1)
            z += (j + 1);
		printf (2, "", z);	
		show_acquiring_info(lk);
		release_priority(lk);
	}
	else
	{
		for (i = 0; i < NUMBER_OF_PROCESSES; i++)
			wait();
		printf(1, "Scheduling test is finished!\n");
	}

	exit();
	return 0;
}