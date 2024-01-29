#include <stdio.h>
#include <unistd.h>

#define MAX_CHAR    80
#define PIPE_IN     1
#define PIPE_OUT    0

int main(int argc, char * argv[]) {

    int fds[2];
    char buff[MAX_CHAR];
    char pipeBuffOut[MAX_CHAR];

    /*Crea Pipe File Descriptors*/
    int ret = pipe(fds);

    /*Lee linea de consola*/
    char * str = fgets(buff, MAX_CHAR, stdin);

    /*Escribe linea en entrada del Pipe*/
    ret = write(fds[PIPE_IN], buff, MAX_CHAR);
    /*Lee la salida del Pipe*/
    ret = read(fds[PIPE_OUT], pipeBuffOut, MAX_CHAR);

    /*Imprime la salida del Pipe*/
    printf("%s",pipeBuffOut);

    return 0;
}