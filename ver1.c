#include <stdio.h>
#include <stdlib.h>

typedef struct noeud noeud;
//----------------------si on fait T[x].nom =(char*) malloc(1) ca case tout donc j'ai mis le truc static
struct noeud{
  int nbocc;
  char nom[10];
  noeud *pere;
  noeud *fG;
  noeud *fD;
};

//--------------------print
void printNoeuds(noeud *T , int tailleT){

  printf("hola \n");
  for(int i = 0 ; i < tailleT ; i++)
    printf("nom = %c  : int = %i , nbocc = %i \n" ,T[i].nom[0] ,T[i].nom[0],T[i].nbocc);
  
}
//----------------nbocc +1 
void plusnbocc(noeud *n){
  n->nbocc = n->nbocc + 1;
}


//---------------BOOL si char present renvoie 1 et nbooc de char +1 , sinon faux
int estPresente(noeud *T , int c , int tailleT){


  int present = 0;
  if(tailleT==0)
    return 0;
  else
    {
      for(int i = 0 ; i<tailleT ; i++)
	if(T[i].nom[0] == c)
	  {
	    plusnbocc(&T[i]);
	    present = 1;
	  }
    }
  return present;
}




int main(int argc , char** argv){
  //fopen(name , mode)
  FILE *file = fopen(argv[1] , "r");
  
  if(file == NULL)
    {
      fprintf(stderr , "\n no file \n");
      exit(2);//2 pour dire que c'est un erreur
    }
  
  noeud *T ;
  int tailleT = 0;
  T =(noeud*) malloc(sizeof(noeud));

  int ch;
  int indice = 0;

  //-------------------------------------compteur des char
  while(((ch=fgetc(file))!=EOF))
    {
      if(ch>31||ch == '\n')
	{
	  if(!estPresente(T , ch ,tailleT))
	    {
	      
	      //printf("\n  \"%c\"  " , ch);
	      //T =(noeud*) realloc(T , sizeof(noeud));
	      //  T[indice].nom =(char*) malloc(1);
	      //case les chars ???????????????????
	      
	      T[indice].nom[0]=ch;
	      T[indice].nbocc = 1; 
	      indice = indice+1;
	      tailleT = tailleT + 1;
	    }
	}
    }
  //-----------------------------------
  
  
 printNoeuds(T , tailleT);
 
 free(T);

  return 0;
}

//ça marche mais si on enleve le  if(ch>31||ch == '\n') il prende des valeurs bizares comme NULL ou 3
//que ne sont pas des vrais chars
