#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define gettid() syscall(SYS_gettid)
#define NUM_THREADS 5
#define MAX_INTERATIONS 100

struct myFile {
    FILE* file;
}; 

sem_t sem;
int shared_resources = MAX_INTERATIONS;


void thread_sleep() {
    time_t t;
    srand((unsigned) time(&t));
    int time_2_sleep = rand() % 2;
    sleep(time_2_sleep);
}

void *resources(void * file){
    int tid = gettid();

    fprintf(((struct myFile *) file)->file, "Iniciando thread %d\n", tid);

    for(int i=0; i<MAX_INTERATIONS; i++){
        fprintf(((struct myFile *) file)->file, "Iniciando iteracion %d\n", i+1);
        sem_wait(&sem);

        shared_resources--; //get the resource
        fprintf(((struct myFile *) file)->file, "%d -- (!) - Recurso tomado\n", tid);
        fprintf(((struct myFile *) file)->file, "%d -- (.) - Usando recurso\n", tid);
        
        thread_sleep();
        shared_resources++; //free resource

        fprintf(((struct myFile *) file)->file, "%d --  :) - Recurso usado\n", tid);
        
        sem_post(&sem); 
        
        fprintf(((struct myFile *) file)->file, "%d -- ( ) - Recurso devuelto\n", tid);
        
    }
        
        fprintf(((struct myFile *) file)->file, "%d -- Terminando semaforo\n", tid);
        fprintf(((struct myFile *) file)->file, "%d -- Muriendo ...\n", tid);

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    
    FILE *fptr; //file pointer
    
    //opening file in write mode
    fptr = fopen("output_semaphore.txt", "w");

    if(fptr < 0) {
        printf("Error in fiile");
        return 1; //error, halt program
    }

    struct myFile* my_file = (struct myFile *) malloc(sizeof(struct myFile));
    my_file->file = fptr;

    printf("Iniciando ...\n");
    fprintf(fptr, "iniciando programa\n");
    fprintf(fptr, "creado threads\n");
    

    pthread_t threads[NUM_THREADS];
    sem_init(&sem, 0, 1);

    for(int i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, *resources, (void *)my_file);
    }

    for(int l=0; l<NUM_THREADS; l++) {
        pthread_join(threads[l], NULL);
    }

    fclose(fptr); //close file
    sem_destroy(&sem);
    return 0;
}