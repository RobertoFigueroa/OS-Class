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


int grid[9][9];


int checkNumbersColumn(int board[9][9]) {
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
            return 0;
        }
    }
    return 1;
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
    for(int number=1; number<=9;number++) {
        for(int  i=0;i<9;i++){
            times=0;
            for(int j=0;j<9;j++){
                if(board[i][j] == number) {
                  times++;  
                }
            }
            if(times!=1){
                return 0;
            }
        }
    }
    return 1;
}

int checkNumbersArray(int row, int column, int board[9][9]){
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
            return 0;
        }
    }
    return 1;
}



int main(int argc, char** argv) {

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
            return 0;
        }
    }


    int pid = getpid();  

    fid = fork();

    if(fid == 0){ 
        //child
        char strPid[20]; //20 should be a nice length for pid
        sprintf(strPid, "%d", pid);
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
        printf("Antes de termina el estado de este proceso y sus threads es:"); 
        char strPid[20]; //20 should be a nice length for pid
        sprintf(strPid, "%d", pid);
        execlp("ps", "ps", "-p", strPid, "-lLF", NULL);
    } 
    else {
        wait(NULL);
    }
    munmap(f, size);
    return 0;
}
