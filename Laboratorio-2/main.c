#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
	int f = fork();


    if(f < 0) {
        fprintf(stderr, "Fork failed");
        return 1;
    }


	if(f==0){
		execl("./ipc","./ipc","10", "a", NULL);
	} else {
		execl("./ipc","./ipc","10", "P", NULL);
	}

	// usleep(1);
    
	return (0);
}
