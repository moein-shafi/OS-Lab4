#include "types.h"
#include "user.h"
#include "fcntl.h"

#define NUMBER_OF_PROCESSES 10
#define NAME_LENGTH 4
#define COUNT_LENGTH 6
#define ONE 1

void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 


int integer_to_string(int x, char str[], int d) 
{ 
    int i = 0; 

    if(x == 0)
      str[i++] = '0';

    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 



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
		char filename[NAME_LENGTH];
		char count[COUNT_LENGTH];

		integer_to_string(getpid(), filename, 0);
		integer_to_string(getsyscallnum(), count, 0);

		int fd = open(filename, O_CREATE | O_RDWR);

		if(fd < 0) 
		{
        	printf(1, "create file failed!\n");
        	exit();
    	}

		if(write(fd, count, sizeof(count)) != sizeof(count))
		{
        	printf(1, "write to file failed!\n");
        	exit();
    	}
    	close(fd);
	}
	else
	{
		for (i = 0; i < NUMBER_OF_PROCESSES; i++)
			wait();
		printf(1, "Per-cpu test is finished!\n");
	}
	
	exit();
}
