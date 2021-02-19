#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    
    printf("Process: %d \n", fork());
    printf("Process: %d \n", fork());
    printf("Process: %d \n", fork());
    printf("Process: %d \n", fork());

    return (0);
}