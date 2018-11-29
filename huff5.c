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
int nbFeuilles(occuCharac *T);









int* tabOccu(char* fileName){
  FILE *myFile=fopen(fileName, "r");
  
  if(myFile==NULL)
    {
        printf("merci de mettre une file \n");
      exit(2);        
    }
  int charac;
  printf("--------- \n");
  int* tab=malloc(sizeof(int) * 256);
  for (int i=0;i<256;i++)
    {
      tab[i]=0;
    }
  while ((charac=fgetc(myFile))!=EOF){
    tab[charac]++;
  }
for(int i = 0 ; i<256 ; i++)
  printf("%c = %i num %i \n", i , tab[i],i );
//---------------EOF---------------//
 tab[3]++;
//-------------------------------//
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

    occuCharac* laTabStruct=malloc((nbrCases + 1)* sizeof(occuCharac));

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
     *CompteurTailleStruct = compteurCases + 1;
     /*
     //-----------------------EOF---------------------------//
    
    laTabStruct[compteurCases].occu=1;
    laTabStruct[compteurCases].codeAscii=-1;
    laTabStruct[compteurCases].pere= NULL;
    laTabStruct[compteurCases].FD=NULL;
    laTabStruct[compteurCases].FG=NULL;
    laTabStruct[compteurCases].perePres=0;
    compteurCases++;
    *CompteurTailleStruct = compteurCases + 1;
    //------------------------------------------------------//
    
    */
    for(int i = 0 ; i<compteurCases ; i++)
      printf("'%c' est present %i fois \n" , laTabStruct[i].codeAscii ,laTabStruct[i].occu );
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
  
  
// printf("indiceA = %i , indiceB = %i  " , indiceA[0] , indiceB[0]);
 
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
   
  for(int i = 0 ; i<*taille; i++)
  printf("creqteur arbre  : codeAscii ='%i' est present %i fois \n" , T[i].codeAscii ,T[i].occu );
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
  
 // printf("taille = %i , compteur = %i \n" , taille , compteur);
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


//-------------------------------------COMPRESEUR--------------------------------------------//
//nbchars 8bits , char 8bits , occu sizeof(int);

//occu -> bin

char* intTobin(int entier){
  int d, compteur;
  char *Tbin;
   
   compteur= 0;
   Tbin = (char*)malloc(32+1);
   

   for (int j  = 31 ; j >= 0 ; j--)
     {
       d = entier >> j;
       
       if (d & 1)
	 Tbin[compteur]= 1 + '0';
       else
	 Tbin[compteur] = 0 + '0';
       
       compteur++;
     }
   Tbin[compteur] = '\0';
   
   return  Tbin;
   
}

//inutile
//nbchars , char
char* charTobin(char c)
{
  int d, compteur;
  char *Tbin;
  
  compteur = 0;
  Tbin = (char*)malloc(8+1);
  
 
  
  for (int i = 8 ; i >= 0 ; i--)
    {
      d = c >> i;
      
      if (d & 1)
	Tbin[compteur]= 1 + '0';
      else
        Tbin[compteur] = 0 + '0';
      
      compteur++;
    }
  Tbin[compteur] = '\0';
  
  return  Tbin;
}


//str = le code binare
char createurBuffer(char* str){
  char buff;
  char bit;
  
  for(int i = 0 ; i<8 ; i++)
    {
      bit = str[i]-'0';
      
      buff = buff | bit;
      if(i!=8-1)
	buff = buff<<1; 
    }
  return buff;
}


void compreseurArbre(occuCharac *T , char* file , int tailleDic)
{
  FILE *FileComprime;
  FileComprime = fopen(file ,"w");
  
  char buff;
  char *buffint;

  char nbChars;
  nbChars =tailleDic;
  
  //mettre le nb de chars <-> nbF
  
  //buff = createurBuffer(charTobin(nbF));

  buff = nbChars;
  fputc(buff , FileComprime);
  
  //mettre char 8bits , occu sizeof(int);
  
  for(int i = 0 ; i<tailleDic ; i++)
    {
      buff = T[i].codeAscii;
      fputc(buff , FileComprime);
      //---------mettre le nb occu ------//
      buffint = intTobin(T[i].occu);
      buff = createurBuffer(buffint);
      fputc(buff , FileComprime);
      buff = createurBuffer(buffint+8);
      fputc(buff , FileComprime);
      buff = createurBuffer(buffint+16);
      fputc(buff , FileComprime);
      buff = createurBuffer(buffint+24);
      fputc(buff , FileComprime);
    }




  
  //par fois ne marche pas
  fclose (FileComprime);
}

int nbFeuilles(occuCharac *T)
{
  int nbfeuilles = 0;
  int i = 0;
  while(EstUneFeuille(T[i]))
    {
      i++;
      nbfeuilles =  nbfeuilles + 1 ;
    }
  return nbfeuilles;
}

//------------------------------------------------------------------------//

//--------------------------decompreseur---------------------------------//
/*
char binTochar(char* file, int indice)
{
  FILE *fileComprime;
  fileComprime = fopen(file , "r");

  for(int i = 0 ;  ; )
  char buffer;
  char bit;
  //char* str = malloc(8);
  for(int i = 0 ; i<8 ; i++)
    {
      bit = str[i]-'0';
      
      x = x | bit;
      if(i!=8-1)
	x = x<<1; 
    }
  
  
}
int binToint(){}

void decompreseurArbre(char* file , int *T){
  charTobin;
  
  for()
    {
      T[binTochar] = binToint;	
    }
}

int nbBitsArbreComprime(char* file){
  FILE *fileComprime;
  fileComprime = fopen(file , "r");

  fgetc(fileComprime);
  
}

*/

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
 //printTabStruct(T , *TailleTabStruct);
  
  CreationDic(tabCode , D , T[*TailleTabStruct-1] , tailleCode , tailleDic);
  printf("tailleDic = %i \n" , *tailleDic);
  PrintDic(*tailleDic , D);


  char *c;
  c = charTobin('A');
  for(int i = 0 ; i<8 ; i++ )
    printf("%s \n" , c);

  c = intTobin(999);
  printf("%s \n" , c);

  
  compreseurArbre(T , "lala.txt" , *tailleDic);
  
  
  
  return(0);
}