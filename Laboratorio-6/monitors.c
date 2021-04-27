#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)
#define NUM_THREADS 5
#define MAX_INTERATIONS 100

int shared_resources = MAX_INTERATIONS;


struct myFile {
    FILE* file;
};


struct monitor {
    sem_t OKtoUse; //
    sem_t OKtoReturn;
    int sourceCount; 
    int isBusyUsing;
    int consumeRequested;
};

struct monitor monitor_data;


void monitor_start2use() {
    if(monitor_data.isBusyUsing || monitor_data.sourceCount != 0){
        sem_wait(&(monitor_data.OKtoUse)); //waits because there is a thread using the resource and there are not enough resources
    }
    monitor_data.isBusyUsing++; //1 means true   
}

void monitor_enduse() {
    monitor_data.isBusyUsing--; //free busy variable 0 means false
    if(monitor_data.consumeRequested) {
        sem_post(&(monitor_data.OKtoReturn));
    } else {
        sem_post(&(monitor_data.OKtoUse));
    }
}

void monitor_startReturn() {
    if(monitor_data.isBusyUsing) {
        monitor_data.consumeRequested++;
        sem_wait(&(monitor_data.OKtoReturn));
        monitor_data.consumeRequested--;
    }
    monitor_data.sourceCount++;
    sem_post(&(monitor_data.OKtoReturn));
}


void monitor_endRetur() {
    monitor_data.sourceCount--;
    if(monitor_data.sourceCount == 0){
        sem_post(&(monitor_data.OKtoReturn));
    }
}

int monitor_initialize() {
    //init struct values
    monitor_data.sourceCount = 0;
    monitor_data.isBusyUsing = 0;
    monitor_data.consumeRequested = 0;

    // init semaphores
    if(sem_init(&(monitor_data.OKtoUse), 0, 1) == 0 && sem_init(&(monitor_data.OKtoReturn), 0, 1) == 0) {
        return 0;
    } else {
        printf("Error in semaphore initiliazation");
    }
    return 1;

}

void monitor_destroy() {
    sem_destroy(&(monitor_data.OKtoUse));
    sem_destroy(&(monitor_data.OKtoReturn));
}

sem_t sem;

int decrease_count(int count) {
    if(shared_resources < count) {
        return -1;
    } else {
        shared_resources -= count;
        return 0;
    }
}

int increase_count(int count) {
    shared_resources += count;
    return 0;
}
 
void thread_sleep() {
    time_t t;
    srand((unsigned) time(&t));
    int time_2_sleep = rand() % 2;
    sleep(time_2_sleep);
}

void *resources(void * file){
    int tid = gettid();
    int resources_2_use = 4;
    fprintf(((struct myFile *) file)->file, "Iniciando thread %d\n", tid);

    for(int i=0; i<MAX_INTERATIONS; i++){
        fprintf(((struct myFile *) file)->file, "Iniciando iteracion %d\n", i+1);
        monitor_start2use();

        decrease_count(resources_2_use); //get the resource
        fprintf(((struct myFile *) file)->file, "%d -- (!) - Recurso tomado\n", tid);
        fprintf(((struct myFile *) file)->file, "%d -- (.) - Usando recurso\n", tid);
        
        thread_sleep();
        increase_count(resources_2_use); //free resource

        fprintf(((struct myFile *) file)->file, "%d --  :) - Recurso usado\n", tid);
        monitor_enduse();
        
        monitor_startReturn();
        fprintf(((struct myFile *) file)->file, "%d -- ( ) - Recurso devuelto\n", tid);
        monitor_endRetur();
        
    }
        
        fprintf(((struct myFile *) file)->file, "%d -- Terminando semaforo\n", tid);
        fprintf(((struct myFile *) file)->file, "%d -- Muriendo ...", tid);

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    
    FILE *fptr; //file pointer
    
    //opening file in write mode
    fptr = fopen("output_monitor.txt", "w");

    if(fptr < 0) {
        printf("Error in fiile");
        return 1; //error, halt program
    }



    struct myFile* my_file = (struct myFile *) malloc(sizeof(struct myFile));
    my_file->file = fptr;

    printf("Iniciando ...\n");
    //init monitor struct
    if(monitor_initialize() == 0) {
        fprintf(fptr, "Initialized monitor\n");
    }
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


    monitor_destroy();
    fclose(fptr); //close file
    return 0;
}