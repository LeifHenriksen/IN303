#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

typedef struct occuCharac {
  int occu;
  int codeAscii;
  // char nom;
  struct occuCharac* pere;
  struct occuCharac* FG;
  struct occuCharac* FD;

}occuCharac;

void nouveuNoeud(int indiceA , int indiceB , int *taille , occuCharac* T );
int NbDeSansPeres(int taille , occuCharac* T);
int* tabOccu(char* fileName){
  FILE *myFile=fopen(fileName, "r");
  
  if(myFile==NULL)
    {
      exit(2);        
    }
  int charac;
  printf("--------- \n");
  int* tab=(int*) malloc(sizeof(int) * 256);
  for (int i=0;i<256;i++)
    {
      tab[i]=0;
    }
  while ((charac=fgetc(myFile))!=EOF){
    tab[charac]++;
  }
  return(tab);
}


occuCharac* tabStruct(int* tabInt , int *CompteurTailleStruct){

  int nbrCases=0;

    for(int i=0; i<256;i++)
      {
        if (tabInt[i]!=0)
	  {
            nbrCases++;
	  }
      }

    occuCharac* laTabStruct=malloc(nbrCases);

    int compteurCases=0;


    for(int i=0;i<256;i++)
      {
        if (tabInt[i]!=0)
	  {
            laTabStruct[compteurCases].occu=tabInt[i];
            laTabStruct[compteurCases].codeAscii=i;
	    //   laTabStruct[compteurCases].nom=(char*) malloc(sizeof(char));
	    // laTabStruct[compteurCases].nom[0]= i ;
	    laTabStruct[compteurCases].pere= NULL;
	    laTabStruct[compteurCases].FD=NULL;
	    laTabStruct[compteurCases].FG=NULL;
	    compteurCases++;
	  }
      }
    *CompteurTailleStruct = compteurCases + 1;
    return(laTabStruct);
}

void printTabStruct( occuCharac* T , int taille)
{
  for(int i = 0 ; i < taille ; i++)
    {
      printf("T[%i] = \"%c\" = %i , nbocc = %i \n" , i ,T[i].codeAscii,T[i].codeAscii  ,T[i].occu );
      if(T[i].pere!=NULL && T[i].FG == NULL && T[i].FD == NULL)
	printf("pere = %c   \n" ,(T[i].pere)->codeAscii );
      
      if(T[i].pere!=NULL && T[i].FG != NULL && T[i].FD != NULL)
	printf("je suis un noeud mon pere = %c , FG = %c , FD = %c \n" ,(T[i].pere)->codeAscii ,(T[i].FG)->codeAscii ,(T[i].FD)->codeAscii);
      else
	if ( T[i].FG != NULL && T[i].FD != NULL)
	  printf("je suis le sommet FG = %c , FD = %c \n" ,(T[i].FG)->codeAscii ,(T[i].FD)->codeAscii);
      
    }
}

void PlusPetiteSomme(int* indiceA , int* indiceB , int taille, occuCharac* T){

  int min;
  for(int i = 0 ; i< taille - 2 ;i++ )
    {
      if(T[i].pere == NULL && T[i].occu != 0)
	{
	  for(int j = i+1 ; j < taille -1 ; j++)
	    {
	      if(T[j].pere == NULL && T[j].occu != 0)
		{
		  min=(T[i].occu + T[j].occu);
		  if((T[i].occu + T[j].occu)<min)
		    {
		      min  = T[i].occu + T[j].occu;
		      indiceA[0] = i;
		      indiceB[0] = j;
		    }
		}
	    }
	}
    }
  if(T[indiceB[0]].pere != NULL) 
    printf("sum min = % i , indice i = %i, indice j = %i , pere de j = %i , pere de i %i" , min ,  indiceA[0]  ,
	   indiceB[0] , (T[ indiceB[0]].pere)->codeAscii , (T[indiceA[0]].pere)->codeAscii);
  delay(1000); 
}

void createurArbre(int *taille , occuCharac* T){
  //int *indiceA= (int*) malloc(sizeof(int));
  // int *indiceB= (int*) malloc(sizeof(int));
  //???????? ne marche pas
  int indiceA[1];
  int indiceB[1];
  while(NbDeSansPeres(*taille , T) != 1)
    {
      PlusPetiteSomme(indiceA , indiceB , *taille , T);
      nouveuNoeud(indiceA[0] , indiceB[0] , taille , T);
    }
  
  
  //free(indiceA);
  //free(indiceB);
}

void nouveuNoeud(int indiceA , int indiceB , int *taille , occuCharac* T )
{
  
  // T = (occuCharac*) realloc(T , *taille + 1);
  //si realloc next size unavailable

  
  T[*taille].occu=T[indiceA].occu + T[indiceB].occu;
  //  T[TailleTabStruct].nom=;
  T[*taille].codeAscii=3;
  T[*taille].pere=NULL;
  T[*taille].FD= &T[indiceA];
  T[*taille].FG= &T[indiceB];
  T[indiceA].pere = &T[*taille];
  T[indiceB].pere = &T[*taille];
  ++*taille;
}

int NbDeSansPeres(int taille , occuCharac* T){
  int compteur = 0;
  for(int i = 0 ; i<taille ; i++)
    if(T[i].pere == NULL)
      compteur = compteur+1;
  printf("taille = %i , compteur = %i \n" , taille , compteur);
  return compteur;
  delay(1000); 
}

void ajoutPere(){}

void ajoutFeuilleD(){}

void ajoutFeuilleG(){}






int main(int argc , char** argv){

  int *TailleTabStruct = (int*) malloc(sizeof(int));
  //int taille;
  //TailleTabStruct = &taille;
  occuCharac* T;
  //  int *indiceA= (int*) malloc(sizeof(int));
  // int *indiceB= (int*) malloc(sizeof(int));
 

 
  T = tabStruct(tabOccu(argv[1]) , TailleTabStruct);
  printf("-------------------------------------\n \n  %i \n" , *TailleTabStruct);
  printTabStruct(T , *TailleTabStruct);
  createurArbre(TailleTabStruct , T);
  printf("-------------------------------------\n \n  %i \n" , *TailleTabStruct);
  printTabStruct(T , *TailleTabStruct);
  //PlusPetiteSomme(indiceA,indiceB,*TailleTabStruct , T);
  //printf("indiceA = %i , indiceB = %i \n" , *indiceA , *indiceB);
  


 
  return(0);


}
