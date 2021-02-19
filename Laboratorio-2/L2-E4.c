#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ITERATIONS 4000000

int main() {
    
    pid_t t = fork();

    if(t == 0) {
        for(int i=1;i<MAX_ITERATIONS+1; i++) {
            printf("Iteraction: %d\n", i);
        }
    }
    else {
        while(1==1);
    }

    return 0;
}
