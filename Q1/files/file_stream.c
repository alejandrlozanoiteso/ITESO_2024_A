#include <stdio.h>

int main(int argc, char * argv[]) {

    FILE * my_file = fopen("Prueba.txt", "wa");
    FILE * fin =  fdopen(0, "r");
    int c;

    do {
        c = fgetc(fin);
        fputc(c, my_file);

    }while(c != EOF);

    return 0;
}