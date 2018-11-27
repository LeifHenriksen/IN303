#include <stdio.h>
#include <stdlib.h>

char bToA(char* binString){     //binString de la forme 10101000
    char res;
        for (int i=0; i<8; i++){
            res=(2*res+(binString[i]=='0' ? 0:1));
        }
        return(res);
}

int main(int argc , char** argv){
    printf("le char est '%c' \n",bToA(argv[1]));
    return(0);
}
