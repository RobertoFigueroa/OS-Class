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
#include <errno.h>


#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1
#define MAX_LINE 80

int main(int argc, char** argv) {

    //revisar que argc sea mayor igual a 3
    pid_t pid;
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    char line[MAX_LINE];
    int n, fd_pipe, fd[2];
    const int SIZE = 4096;
    const char* name = "/OS";
    const char* myfifo = "/tmp/myFIFO";
    const char* message_0 = "Hello";
    const char* message_1 = "World!";

    //argument values 
    if(argc < 3) {
        fprintf(stderr, "No parameters passed\n");
        return 1;
    }

    
    sscanf(argv[1], "%d", &n);
    char* x = argv[2];
    
    int buffer_size = strlen(x)+1;

    printf("I am %s\n", x);

    mkfifo(myfifo, 0666);

    fd_pipe = open(myfifo, 0666);
    /*-----Start creating share mem space-----*/
    int fdr;
    char* ptr;
    fdr = shm_open(name, O_EXCL | O_CREAT | O_RDWR, 0666);
    if(fdr == -1){
        //already create
        printf("%s: Already created  shared memory object\n", x);
        fdr = shm_open(name, O_RDWR, 0666);
        int f;
        read(fd_pipe, &f, sizeof(f));
        printf("%s: Recived shm fd -> %d \n",x, f);
    } else {
        printf("%s: Create new shared memory object %d\n", x, fdr);
        write(fd_pipe, &fdr, sizeof(fdr));
    }
    
    close(fd_pipe);
    
    ftruncate(fdr, SIZE);
    ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fdr, 0);
    printf("\n%s: Ptr created with value -> %p", x, ptr);


    if(pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        return 1;
    }


    // pid = fork();

    // if(pid < 0) {
    //     fprintf(stderr, "Fork failed");
    //     return 1;
    // }

    // if(pid > 0) {
    //     for(int i=0; i<SIZE; i++){
    //         if(i%n){
    //             write(fd[WRITE_END], x, buffer_size);
    //         }
    //         close(fd[WRITE_END]);   
    //     }
        
    // } else {
    //     char* msg_read[buffer_size];
    //     while(read(fd[READ_END],msg_read, buffer_size) > 0) {
    //         sprintf(ptr, "%s", (char *) msg_read);
    //         ptr += buffer_size;
    //     }
    //     close(fd[READ_END]);
        
    // }

    // printf("%s: %s\n", x, (char *)ptr);
    shm_unlink(name);

    

    return 0;
}