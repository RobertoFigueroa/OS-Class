#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <omp.h>


int grid[9][9];


int checkNumbersColumn(int board[9][9]) {
    int result = 1;
    omp_set_num_threads(9);
    omp_set_nested(1==1);
    #pragma omp parallel for schedule(dynamic)
    for(int number=1; number<=9;number++) {
        int times=0;
        for(int  i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(board[j][i] == number) {
                  times++;  
                }
            }
        }
        if(times!=1){
            result = 0;
        }
    }
    return result;
}



void *checkNumbersClmn(void *param) {
    int result = 0;
    int threadid = syscall(SYS_gettid);
    int times;
    for(int number=1; number<=9;number++) {
        for(int  i=0;i<9;i++){
            times=0;
            for(int j=0;j<9;j++){
                if(grid[j][i] == number) {
                  times++;  
                }
            }
            if(times!=1){
                pthread_exit(&result);
            }
        }
    printf("En la revision de columnas el siguiente es un thread en ejecucion: %d\n", threadid);
    }
    result = 1;
    pthread_exit(&result);
}


int checkNumbersRow(int board[9][9]){
    int times;
    int result = 1;
    omp_set_nested(1==1);
    omp_set_num_threads(9);
    #pragma omp parallel for /*schedule(dynamic)*/
    for(int number=1; number<=9;number++) {
        for(int  i=0;i<9;i++){
            times=0;
            for(int j=0;j<9;j++){
                if(board[i][j] == number) {
                  times++;  
                }
            }
            if(times!=1){
                result = 0;
            }
        }
    }
    return result;
}

int checkNumbersArray(int row, int column, int board[9][9]){
    int result = 1;
    omp_set_num_threads(9);
    omp_set_nested(1==1);
    #pragma omp parallel for schedule(dynamic)
    for(int number=1; number<=9;number++) {
        int times=0;
        for(int  i=row;i<row+3;i++){
            for(int j=column;j<column+3;j++){
                if(board[j][i] == number) {
                  times++;  
                }
            }
        }
        if(times!=1){
            result = 0;
        }
    }
    return result;
}


int main(int argc, char** argv) {
    omp_set_num_threads(1);

    unsigned char *f;
    pid_t fid;
    pthread_t tid;
    pthread_attr_t attr;
    struct stat s;
    int size, index=0;
    int fd = open(argv[1], O_RDONLY);
    //getting the file size
    int status = fstat(fd, &s);
    size = s.st_size;

    f = (char *) mmap(0,size,PROT_READ, MAP_PRIVATE, fd, 0);
    for(int i=0; i<9; i++){
        for(int j=1; j<10; j++){
            grid[i][j-1] = f[index]-'0'; //convert number readed as  character into int
            index++;
        }
    }

    for(int i=0;i<=8;i=i+3){
        if(checkNumbersArray(i, i, grid) == 0){
            exit(0);
        }
    }


    int pid = getpid();  
    char strPid[20];
    sprintf(strPid, "%d", pid);

    fid = fork();

    if(fid == 0){ 
        //child
        printf("El thread en el que se ejecuta el main: %d\n", pid); 
        execlp("ps", "ps", "-p", strPid, "-lLF", NULL);
    }
    else {
        //parent
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, checkNumbersClmn, *grid); //this time *grid is no useful
        void *retval;
        pthread_join(tid, &retval);
        if(checkNumbersRow(grid)==1){
            printf("El sudoku esta resuelto\n");
        }
        wait(NULL);

    }

    if(fork() == 0) {
        printf("zAntes de termina el estado de este proceso y sus threads es:\n"); 
        execlp("ps", "ps", "-p", strPid, "-lLF", NULL);
    } 
    else {
        wait(NULL);
    }
    munmap(f, size);
    return 0;
}
