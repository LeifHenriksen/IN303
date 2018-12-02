#include <stdio.h>
#include <stdlib.h>


int main(int argc , char** argv){
    FILE* myFile=fopen(argv[1],"r");
    unsigned char charac;
    while((charac=fgetc(myFile))!=255){
        printf("'%c' is %i as int   \n", charac,charac);
    }
    fclose(myFile);
    printf("\n");
}
