#include <stdio.h>
#include <time.h>

#define MAX_ITERATIONS 1000000

int main() {
    
    clock_t tinit, tfinal;

    tinit = clock();
    for(int i=0;i<MAX_ITERATIONS;i++) {
        printf("Indice: %d\n", i);
    }
    for(int j=0;j<MAX_ITERATIONS;j++) {
        printf("Indice: %d\n", j);
    }
    for(int k=0;k<MAX_ITERATIONS;k++) {
        printf("Indice: %d\n", k);
    }
    tfinal = clock();

    double ttotal = (double)(tfinal - tinit)/CLOCKS_PER_SEC;
    printf("Total time: %f\n", ttotal);

    return 0;
}
