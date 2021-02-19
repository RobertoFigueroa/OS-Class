#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ITERATIONS 1000000

int main() {

    pid_t t1, t2, t3;
    clock_t tinit, tfinal;

    tinit = clock();
    t1 = fork();
    if(t1 == 0){
        //child
        t2 = fork();
        if(t2 == 0) {
            t3 = fork();
            if(t3 == 0) {
                for(int i=0;i<MAX_ITERATIONS;i++){
                    printf("Indice: %d\n", i);
                }
                wait(NULL);
            } else {
                for(int k=0;k<MAX_ITERATIONS;k++) {
                    printf("Indice: %d\n", k);
                }
            }
        } else {
            for(int j=0;j<MAX_ITERATIONS;j++) {
                printf("Indice: %d\n", j);
            }
            wait(NULL);
        }
    } else {
        //parent
        wait(NULL);
        tfinal = clock();
        double ttotal = (double)(tfinal - tinit)/CLOCKS_PER_SEC;
        printf("Total time: %f \n", ttotal);
        return 0;
    }
}