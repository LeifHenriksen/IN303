#include <stdio.h>
#include <stdlib.h>

struct occuCharac{
    int occu;
    int codeAscii;
};

int* tabOccu(char* fileName){
    FILE *myFile=fopen(fileName, "r");
    
    if(myFile==NULL){
        exit(2);        
    }
    int charac;
    int* tab=malloc(256);
    for (int i=0;i<256:i++){
        tab[i]=0;
    }
    while ((charac=fgetc(myFile))!=EOF){
        tab[charac]++;
    }
    return(tab);
}


struct occuCharac* tabStruct(int* tabInt){
    int nbrCases=0;
    for(int i=0; i<256;i++){
        if (tabInt[i]!=0){
            nbrCases++;
        }
    }
    struct occuCharac* laTabStruct=malloc(nbrCases);
    int compteurCases=0;
    for(int i=0;i<256;i++){
        if (tabInt[i]!=0){
            laTabStruct[compteurCases].occu=tabInt[i];
            laTabStruct[compteurCases].codeAscii=i;
            printf("%i fois '%c' \n",tabInt[i],(char)i);
            compteurCases++;
        }
    }
    return(laTabStruct);
}


int main(int argc , char** argv){
    tabStruct(tabOccu(argv[1]));
    return(0);
}
