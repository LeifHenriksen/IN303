#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
/*--------------------A LIRE-----------------------------------------
-Les delays sont pour debugger suelement
-Le programe arrive a faire l'arbre, tu peux tester avec "this is an example of a huffman tree " on trouve la meme chosse que sur wikipedia donc c'est bonne. La seul chosse
c'est que on a des occu en plus, une seulement dans cette exemple mais je pense que c'est car on a le character 0, que je ne c'est pass pour quoi il existe.
-les mallocs et reallocs sont bipolairs donc j'ai utilise des tab statiques parfois
- breve, le truc marche bien, la seul chose c'est que on a une occu en plus et
il faut donner des nombres aux noeuds, tu peux voir  // char nom; dans la struct
mais je n'ai pas reussi a le faire
- Donc 
1-) voir si le occu +1 est un probleme ou non 
2-) donner un nombre aux noeuds
2.1-)on peut fair aussi un .h pour et .cpp pour les fonctions 
apres on peut faire le code, puis compreser et finalement decompreser
----------------------------------------------FIN-----------------
*/


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
  int perePres;
  // char* nom;
  struct occuCharac* pere;
  struct occuCharac* FG;
  struct occuCharac* FD;

}occuCharac;


typedef struct elementDic{
  int codeAscii;
  int codeArbre[256];
  int tailleCode;
  int nboccu;
}elementDic;


void nouveuNoeud(int indiceA , int indiceB , int *taille , occuCharac* T );
int NbDeSansPeres(int taille , occuCharac* T);
void ajoutPere(int indicefils,int indicepere , occuCharac* T);
void ajoutFeuilleG(int indicefils,int indicepere ,occuCharac* T );
void ajoutFeuilleD(int indicefils,int indicepere ,occuCharac* T );

int EstUneFeuille(occuCharac x);










int* tabOccu(char* fileName){
  FILE *myFile=fopen(fileName, "r");
  
  if(myFile==NULL)
    {
        //printf("merci de mettre une file \n");
      exit(2);        
    }
  unsigned char charac;
  int* tab=malloc(sizeof(int) * 256);
  for (int i=0;i<256;i++)
    {
      tab[i]=0;
    }
  while ((charac=fgetc(myFile))!=255){
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

    occuCharac* laTabStruct=malloc(nbrCases * sizeof(occuCharac));

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
	    laTabStruct[compteurCases].perePres=0;
	    compteurCases++;
	  }
      }
    *CompteurTailleStruct = compteurCases + 1 ;

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


void printArbre(occuCharac* tabOccu,int taille){
    for(int i=0;i<taille;i++){
        printf("%x pos %i char %i x %i pere(%d) %x g %x d %x\n",
               &tabOccu[i],
               i,tabOccu[i].codeAscii,tabOccu[i].occu,
               tabOccu[i].perePres,
               tabOccu[i].pere,
               tabOccu[i].FG,
               tabOccu[i].FD);
    }
}



void PlusPetiteSomme(int* indiceA , int* indiceB , int taille, occuCharac* T){
  int PPE = -1;
 
  for(int i = 0 ; i<taille ; i++)
    {
      //initia
      if(T[i].perePres == 0 && PPE == -1)
	PPE = T[i].occu;
      if(T[i].perePres == 0 && T[i].occu<=PPE)
	{
	  PPE = T[i].occu;
	  *indiceA = i;
	}
    }

  PPE = -1;
  for(int i = 0 ; i<taille ; i++)
    {
      //initia
      if(T[i].perePres == 0 && PPE == -1 && indiceA[0] != i)
	PPE = T[i].occu;
      if(T[i].perePres == 0 && T[i].occu<=PPE && indiceA[0] != i)
	{
	  PPE = T[i].occu;
	  *indiceB = i;
	}
    }
  
  
//printf("indiceA = %i , indiceB = %i  " , indiceA[0] , indiceB[0]);
 
}

void createurArbre(int *taille , occuCharac* T){
  int *indiceA= (int*) malloc(sizeof(int));
   int *indiceB= (int*) malloc(sizeof(int));
  //???????? ne marche pas
 // int *indiceA;
 // int *indiceB;
  while(NbDeSansPeres(*taille , T) != 1)
    {
      PlusPetiteSomme(indiceA , indiceB , *taille , T);
      nouveuNoeud(*indiceA , *indiceB , taille , T);
    }
  
  //for(int i = 0 ; i<*taille; i++)
	//printf("creqteur arbre  : codeAscii ='%i' est present %i fois \n" , T[i].codeAscii ,T[i].occu );
  //free(indiceA);
  //free(indiceB);
}

void nouveuNoeud(int indiceA , int indiceB , int *taille , occuCharac* T )
{
  
  // T = (occuCharac*) realloc(T , *taille + 1);
  //si realloc next size unavailable

  
  T[*taille].occu=T[indiceA].occu + T[indiceB].occu;
  //  T[TailleTabStruct].nom=;
  //T[*taille ].codeAscii=-1;
  T[*taille].pere=NULL;
  T[*taille].perePres=0;


  ajoutFeuilleD(indiceA , *taille , T);
  ajoutFeuilleG(indiceB , *taille , T);
  ajoutPere(indiceA , *taille , T);
  ajoutPere(indiceB , *taille, T);
  
  ++*taille;
}

int NbDeSansPeres(int taille , occuCharac* T){
  int compteur = 0;
  for(int i = 0 ; i<taille ; i++)
    if(T[i].perePres == 0)
      compteur = compteur+1;
  
  //printf("taille = %i , compteur = %i \n" , taille , compteur);
  return compteur;
 
  //si c'est egal a 1 on arrete car c'est le sommet
}

void ajoutPere(int indicefils,int indicepere , occuCharac* T){
  T[indicefils].pere = &T[indicepere];
  T[indicefils].perePres=1;
}

void ajoutFeuilleD(int indicefils,int indicepere ,occuCharac* T ){ T[indicepere].FD= &T[indicefils];}

void ajoutFeuilleG(int indicefils,int indicepere ,occuCharac* T){ T[indicepere].FG= &T[indicefils];}

int EstUneFeuille(occuCharac x){

  return (x.FG == NULL && x.FD == NULL) ;

}

void copieTab(int *T , int *R){
  for(int i = 0 ; i<256 ; i++)
    R[i] = T[i];

}



void CreationDic(int *tabCode ,elementDic *D , occuCharac T , int tailleCode , int *tailleDic){

  if(EstUneFeuille(T)){
  
     copieTab(tabCode , D[*tailleDic].codeArbre);
     D[*tailleDic].codeAscii = T.codeAscii;
     D[*tailleDic].tailleCode = tailleCode;
     D[*tailleDic].nboccu = T.occu;
     *tailleDic = *tailleDic + 1;
  }
  else
    {
      tabCode[tailleCode] = 1;
      CreationDic(tabCode, D , *(T.FG) , tailleCode + 1 , tailleDic);
      tabCode[tailleCode] = 0;
      CreationDic(tabCode, D  , *(T.FD) , tailleCode + 1 , tailleDic);
    }
  
}

void PrintDic(int tailleDic , elementDic *D){
  for(int i = 0 ; i<tailleDic ; i++){
    printf("D[%i] : codeAscii = %i ,  char = %c,  nboccu = %i  , tailleCode = %i , codeArbre = " , i , D[i].codeAscii  , D[i].codeAscii, D[i].nboccu,D[i].tailleCode);
    
    for(int j = 0 ; j<D[i].tailleCode ; j++)
      printf(" [%i] " , D[i].codeArbre[j]);
    printf("\n");
  }
}


char* getCodeFromChar(unsigned char monChar, elementDic* monDico, int tailleDico){
  int i=0;
  while (i<tailleDico && monDico[i].codeAscii!=monChar){
    i++;
  }
  if(i>=tailleDico){
      printf("'%c' n'est pas dans le dico (ascii : %i) code \n",monChar,monChar);
}
  char* res=malloc(monDico[i].tailleCode);
  for (int j=0;j<monDico[i].tailleCode;j++){
    res[j]=monDico[i].codeArbre[j];
  }
  //printf("char %c code %s \n",monChar, res);
  return(res);
}


int getTailleCodeChar(unsigned char monChar, elementDic* monDico, int tailleDico){
    int i=0;
  while (i<tailleDico && monDico[i].codeAscii!=monChar){
    i++;
  }
  if(i>=tailleDico){
    printf("'%c' n'est pas dans le dico (ascii : %i) taille \n",monChar,monChar);
    }
  return(monDico[i].tailleCode);
}


unsigned char bitsToA(char* binString){
    unsigned char res;
    for(int i=0;i<8;i++){
        res=(2*res+(binString[i]=='0' ? 0:1));
    }
    return (res);
}


char* charToBin(unsigned char monChar){
    char* res=malloc(8);
    int i=0;
    while(monChar!=0){
        res[7-i]=(monChar%2==1 ? '1' : '0');
        monChar=monChar/2;
        i++;
    }
    while(i<8){
        res[7-i]='0';
        i++;
    }
    return(res);
}


void printString(char* string, int taille){
    for (int i=0;i<taille;i++){
        printf("%c",string[i]);
    }
    printf(" ma string\n");
}

        //filsGauche : 1 ; filsDroite : 0
void decompresseur(char* nomSource, char* nomSortie, occuCharac racine){
    FILE *fileSource=fopen(nomSource,"r");
    FILE *fileSortie=fopen(nomSortie,"w");
    unsigned char charac;
    char* buffer;
    occuCharac posArbre=racine;
    while((charac=fgetc(fileSource))!=255){
        buffer=charToBin(charac); 
       // printf("%c charac  %x ",charac,charac);
        printString(buffer,8);
       
        for(int posBuffer=0;posBuffer<8;posBuffer++){
            if(EstUneFeuille(posArbre)){
                printf("%d ascii d'une feuille\n",posArbre.codeAscii);
                fputc((unsigned char)posArbre.codeAscii,fileSortie);
                posArbre=racine;
                }
                if (buffer[posBuffer]=='1'){
                    printf(" got 1\n");
                    posArbre=*posArbre.FG;
                }
                else{
                    printf(" got 0\n");
                    posArbre=*posArbre.FD;
                }
        }
        free(buffer);
    }
//    fclose(fileSource);
  //  fclose(fileSortie);
}


void printTree(occuCharac* T, int taille){
    for(int i =0; i<taille;i++){
        printf("P%x [label =\"%d %c x %d\"];\n",&T[i],i,T[i].codeAscii,T[i].occu);
    }
    for(int i =0; i<taille;i++){
        printf("P%x -> P%x [color=\"green\"];\n",&T[i],T[i].FG);
        printf("P%x -> P%x [color=\"blue\"];\n",&T[i],T[i].FD);
        printf("P%x -> P%x [color=\"red\"];\n",&T[i],T[i].pere);
    }
}



void compresseur(char* nomSource, char* nomSortie, elementDic* dico, int tailleDico){
  FILE *fileSource=fopen(nomSource, "r");
  FILE *fileSortie=fopen(nomSortie,"w");
  unsigned char charac;
  char* buffer=malloc(8);
  int posBuff=0;
  while ((charac=fgetc(fileSource))!=255){
      printf("'%c' trouve dans le file (ascii : %i)\n",charac,charac);
      int tailleCode=getTailleCodeChar(charac,dico,tailleDico);
      printf("%i la taille du code de ce charac %c",tailleCode,charac);
      char* codeChar=getCodeFromChar(charac,dico,tailleDico);
      printString(codeChar,tailleCode);
	  for(int i=0;i<tailleCode;i++){
			if(posBuff==8){
                //printString(buffer,8);
				fputc(bitsToA(buffer),fileSortie);
				posBuff=0;
				}
			buffer[posBuff]=codeChar[i]+'0';
            posBuff++;
		  }
	  }
	  fclose(fileSortie);
      fclose(fileSource);
}


int main(int argc , char** argv)
{
  
  int *TailleTabStruct = (int*) malloc(sizeof(int));
  occuCharac* T;
	  
  int tabCode[256];
  elementDic* D = (elementDic*) malloc(sizeof(elementDic) * 1000);
  int *tailleDic = (int*) malloc(sizeof(int));
  int tailleCode = 0;

  T = tabStruct(tabOccu(argv[1]) , TailleTabStruct);
  createurArbre(TailleTabStruct , T);
  //printf("tailletabstruct %i\n",*TailleTabStruct);
 //printTabStruct(T , *TailleTabStruct);
  //printTree(T,*TailleTabStruct);
 
 printf("--------------------------------------------\n");
  
  CreationDic(tabCode , D , T[*TailleTabStruct-1] , tailleCode , tailleDic);
  printf("diccréé");
  //printf("tailleDic = %i \n" , *tailleDic);  
  //PrintDic(*tailleDic , D);
  //printf("--------------------------------------------\n");
  compresseur(argv[1],argv[2],D,*tailleDic);
  //printAscii(argv[1],D,*tailleDic);
  decompresseur(argv[2],argv[3],T[*TailleTabStruct-1]);
  return(0);
}
