//Programa para leer un archivo y copiar su contenido en otro
//Referencia: http://cs.boisestate.edu/~amit/teaching/297/notes/files-and-processes-handout.pdf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 65536

int main(int argc, char * argv[]) {
    FILE *src, *dst;
    size_t in, out;
    char buf[BUF_SIZE];
    int bufsize;

    src = fopen(argv[1], "r");
    fseek(src, 0, SEEK_END);
    bufsize = ftell(src);
    fseek(src, 0, SEEK_SET);

    if(bufsize > BUF_SIZE) {
        fprintf(stderr, "Error: %s max. buffer size is %d\n", argv[0], BUF_SIZE);
        exit (1);
    }

    if (NULL == src) {
        exit (2);
    }

    dst = fopen(argv[2], "w");
    if(dst < 0) {
        exit (3);
    }

    int keep_read = 1;

    while(keep_read == 1) {
        in = fread(buf, 1, bufsize, src);
        if (0 == in) {
            keep_read = 0;
        }
        out = fwrite(buf, 1, in , dst);
        if(0 == out) {
            keep_read = 0;
        }
    }
    
    fclose(src);
    fclose(dst);
    exit (0);
}