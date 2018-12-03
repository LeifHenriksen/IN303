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

void printTint(int *T){
  for(int i = 0 ; i<256 ; i++)
    if(T[i]!=0)
      printf("T[%i] : char = %c , occu = %i  \n", i , i , T[i] );
}







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
  //for(int i = 0 ; i<256 ; i++)
  //printf("%c = %i num %i \n", i , tab[i],i );
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
    // *CompteurTailleStruct = compteurCases+1; // original
    //changement car dic trop petit
    *CompteurTailleStruct = compteurCases+2;
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


  ajoutFeuilleD(indiceA , *taille, T);
  ajoutFeuilleG(indiceB , *taille, T);
  ajoutPere(indiceA , *taille, T);
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

  if(EstUneFeuille(T) ){
  
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


//-------------------------------------COMPRESEUR ARBRE--------------------------------------------//
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
	 Tbin[compteur]= 1;
       else
	 Tbin[compteur] = 0;
       
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
  char buff = 0;
  char bit = 0;
  
  for(int i = 0 ; i<8 ; i++)
    {
      bit = str[i];
      buff = buff | bit;
      if(i!=7)
	buff = buff<<1; 
    }
  return buff;
}


int nbrCharFile(char* nomSource){
    FILE* fileSource=fopen(nomSource,"r");
    char myChar;
    int res=0;
    while((myChar=fgetc(fileSource))!=EOF){
        res++;
    }
    return(res);
}


unsigned char bitsToA(char* binString){
    unsigned char res;
    for(int i=0;i<8;i++){
        res=(2*res+(binString[i]=='0' ? 0:1));
    }
    return (res);
}


void compreseurArbre(occuCharac *T , char* file , int tailleDic)
{
  FILE *FileComprime;
  FileComprime = fopen(file ,"w");
  
  char buff;
  char *buffint = malloc(32);
  for(int i=0 ; i<32 ; i++)
    buffint[i]=0;
  char nbChars;
  nbChars = tailleDic;
  
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


      //buffint = intTobin(1);
      // printf("%i \n" , T[i].occu);
      //   for(int i = 0;i<32;i++)
      //	printf("%i " , buffint[i]);
      //printf("\n \n");

      //--------------------0
      buff = createurBuffer(buffint);
      fputc(buff , FileComprime);
      
      //printf("%i " , buff);
      //printf("\n \n");

      //----------------0
      buff = createurBuffer(buffint+8);
      fputc(buff , FileComprime);

      //---------------0
      // printf("%i " , buff);
      //printf("\n \n");
      buff = createurBuffer(buffint+16);
      fputc(buff , FileComprime);

      //----------------1
      //printf("%i " , buff);
      // printf("\n \n");
      buff = createurBuffer(buffint+24);
      fputc(buff , FileComprime);
      
      //printf("%i " , buff);
      //printf("\n \n");
    }




  
  //par fois ne marche pas
  // fclose (FileComprime);
}









/*

void compresseurArbre(occuCharac *T , FILE* nomSortie , int tailleDic)
{


  FILE *FileComprime = nomSortie;
  char buff;
  char *buffint = malloc(32);
  for(int i=0 ; i<32 ; i++)
    buffint[i]=0;
  char nbChars;
  nbChars = tailleDic;
  
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


      //buffint = intTobin(1);
      // printf("%i \n" , T[i].occu);
      //   for(int i = 0;i<32;i++)
      //	printf("%i " , buffint[i]);
      //printf("\n \n");

      //--------------------0
      buff = createurBuffer(buffint);
      fputc(buff , FileComprime);
      
      //printf("%i " , buff);
      //printf("\n \n");

      //----------------0
      buff = createurBuffer(buffint+8);
      fputc(buff , FileComprime);

      //---------------0
      // printf("%i " , buff);
      //printf("\n \n");
      buff = createurBuffer(buffint+16);
      fputc(buff , FileComprime);

      //----------------1
      //printf("%i " , buff);
      // printf("\n \n");
      buff = createurBuffer(buffint+24);
      fputc(buff , FileComprime);
      
      //printf("%i " , buff);
      //printf("\n \n");
    }




  
  //par fois ne marche pas
  // fclose (FileComprime);
}
*/
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

//--------------------------decompreseur ARBRE---------------------------------//
//nbchars 8bits , char 8bits , occu sizeof(int);


void binToint(char c , char* strInt , int indice)
{
  
  char buffer;
  buffer = c;
  //  printf("buffer = %i \n" , buffer);
  // strInt[31]=1;
  //si >0 alors 0 sinon 1
  for(int i =indice ; i<indice+8 ; i++)
    {
     
      if(buffer >= 0)
	strInt[i]=0;
      else
	strInt[i]=1;
     

      if(i != indice + 7)
	buffer = buffer<<1;

      // printf("buffer = %i " , buffer);
    }

  /*
    char buffer;
    buffer = c;
    
    //si >0 alors 0 sinon 1
    for(int i = indice ; i<indice+8 ; i++)
    {
    if(buffer >= 0)
    strInt[i]=0;
    else
    strInt[i]=1;
    
    buffer = buffer<<1;
    }
  */
  
}


int strToint(char* str){

  int x = 0;
  int bit = 0;
 
 
  for(int i = 0 ; i<32; i++)
    {


      
      bit =str[i];
      x = x | bit;
      if(i!=31)
	x = x<<1;

      
      


      /*
      char* y;
      y = intTobin(x);
      for(int i = 0;i<32;i++)
	printf("%c " , y[i]);
      printf("\n");*/
    }
 
  return x;
}


void decompreseurArbre(FILE* fileC , int *T){

 FILE *fileComprime=fileC;
 
 

 char buffint;
 char buffchar;
 char *strInt = malloc(32);
 //nbchars

 int nbchars;
 nbchars = fgetc(fileComprime);
 printf("nb chars = %i \n" , nbchars);

 
 
 for(int i = 0 ; i<nbchars ; i++)
    {
      buffchar = fgetc(fileComprime);
      

      buffint = fgetc(fileComprime);
      binToint(buffint , strInt , 0);

      // printf("%i " , buffint);
      //printf("\n \n");

      //for(int i = 0;i<8;i++)
      //  printf("%i " , strInt[i]);
      //printf("\n");
	
      buffint = fgetc(fileComprime);
      binToint(buffint , strInt , 8);

      //printf("%i " , buffint);
      //printf("\n \n");

      //      	for(int i = 0;i<16;i++)
      //  printf("%i " , strInt[i]);
      //printf("\n");
	
      buffint = fgetc(fileComprime);
      binToint(buffint , strInt , 16);

      //printf("%i " ,buffint);
      //printf("\n \n");

      //for(int i = 0;i<24;i++)
      //  printf("%i " , strInt[i]);
      //printf("\n");
	
      buffint = fgetc(fileComprime);
      binToint(buffint , strInt , 24);

      //printf("%i " , buffint);
      //printf("\n \n");
      T[buffchar] = strToint(strInt);
      
      /*
	printf("T[%i] : char = %c , occu = %i  \n", buffchar , buffchar , T[buffchar] );
      for(int i = 0;i<32;i++)
	printf("%i " , strInt[i]);
	printf("\n");
      */    
    }

}




//--------------------------------------------------------------//


//---------------------FELIX--------------------------------------//

void getCodeFromChar(unsigned char monChar, elementDic* monDico, int tailleDico , char* res){
  //res est un ptr vers une variable declare en compresseur

  // printf("\n 1---------- \n");

  int i=0;
  while (i<tailleDico && monDico[i].codeAscii!=monChar){
    i++;
  }
  if(i>=tailleDico){
    printf("'%c' n'est pas dans le dico (ascii : %i) code \n",monChar,monChar);
  }
  //printf("\n 2---------- \n");
  //  char* res=(char*) malloc(monDico[i].tailleCode);
  //printf("\n 3---------- \n");

  for (int j=0;j<monDico[i].tailleCode;j++){
    res[j]=monDico[i].codeArbre[j];
  }
  
  // printf("\n char %c code %s \n",monChar, res);
  //return(res);
}


int getTailleCodeChar(unsigned char monChar, elementDic* monDico, int tailleDico){
  int i=0;
  while (i<tailleDico && monDico[i].codeAscii!=monChar){
    i++;
  }
  if(i>=tailleDico){
    printf("'%c' n'est pas dans le dico (ascii : %i) taille \n",monChar,monChar);
  }
  /*
int present = 0;
  int indice;
  for(int i = 0 ; i<tailleDico ; i++)
  if(monDico[i].codeAscii == monChar)
      indice = i;
      // printf("indice = %i \n" , indice);
 */  
  return(monDico[i].tailleCode);
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
void decompresseur(FILE* nomSource, FILE* nomSortie, occuCharac racine, int nbrChar){
    //FILE *fileSource=fopen(nomSource,"r");
    //FILE *fileSortie=fopen(nomSortie,"w");
    unsigned char charac;
    char* buffer;
    occuCharac posArbre=racine;
    int compteurChar=0;
    while((charac=fgetc(nomSource))!=255){
        buffer=charToBin(charac); 
        //printf("%c charac  %x ",charac,charac);
        printString(buffer,8);
       
        for(int posBuffer=0;posBuffer<8;posBuffer++){
            if(EstUneFeuille(posArbre) && (compteurChar<nbrChar)){
                //printf("%d ascii d'une feuille\n",posArbre.codeAscii);
                fputc((unsigned char)posArbre.codeAscii,nomSortie);
                posArbre=racine;
                compteurChar++;
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
/*
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
*/

void compresseur(FILE* nomSource, FILE *nomSortie, elementDic* dico, int tailleDico){
  //FILE *fileSource=fopen(nomSource, "r");
  //FILE *fileSortie=fopen(nomSortie,"w");

  FILE *fileSource = nomSource;
  FILE *fileSortie = nomSortie;
  //on pase code char comme pointeur dans la fonction getCodeFromChar, et getCodeFromChar est un void, le malloc
  //qui etais dans getCodeFromChar donne segmentation fault avec les gros fichiers
  char* codeChar = malloc(256);

  unsigned char charac;
  char* buffer=malloc(8);
  int posBuff=0;

  while ((charac=fgetc(fileSource))!=255){
    // printf("'%c' trouve dans le file (ascii : %i)\n",charac,charac);
    int tailleCode=getTailleCodeChar(charac,dico,tailleDico);
    //printf("%i la taille du code de ce charac %c",tailleCode,charac);
    getCodeFromChar(charac,dico,tailleDico, codeChar);
    //printString(codeChar,tailleCode);
    for(int i=0;i<tailleCode;i++){
      if(posBuff==8){
	//	printString(buffer,8);
	fputc(bitsToA(buffer),fileSortie);
	posBuff=0;
      }
      buffer[posBuff]=codeChar[i]+'0';
      posBuff++;
    }
  }
  // fclose(fileSortie);
  //fclose(fileSource);
}

//--------------------------------------------------------------------------//





//--------------------------------------FINAL-----------------------------//

void compresseurTotal(char* nomSource, char* nomSortie, elementDic* dico, int tailleDic , occuCharac *T){
  FILE *fileSource=fopen(nomSource, "r");
  FILE *fileSortie=fopen(nomSortie,"w");

  compresseurArbre(T , fileSortie , tailleDic);
  compresseur(fileSource,fileSortie,dico,tailleDic);

  fclose(fileSortie);
  // fclose(fileSource);
}




void decompresseurTotal(char* nomSource, char* nomSortie){


  //decompreseurLeif
  //creation de le nouveau occuChara* T avec le tabOccu que nous donne le decompreseurLeif
  //decompresseurFelix qui utilise T
  
    FILE* fileSource=fopen(nomSource,"r");
    FILE* fileSortie=fopen(nomSortie,"w");

    int *tabInt=malloc(256*sizeof(int));
    for (int i=0;i<256;i++)
      {
	tabInt[i]=0;
      }
    int nbrChars=0;

    decompreseurArbre(fileSource, tabInt);
    
    for(int i = 0 ; i<256 ; i++) 
      printf("t 1 = %i ",tabInt[i]);
    
    for(int i=0;i<256;i++)
      {
	if (tabInt[i]!=0)
	  {
	    
	    nbrChars=nbrChars+tabInt[i];
	    
	  }
      }
    
    printf("%d chars en tout à decompresser\n",nbrChars);
    
    int* tailleTabStruct=malloc(sizeof(int));
    occuCharac* T;
    
    T=tabStruct(tabInt,tailleTabStruct);
    createurArbre(tailleTabStruct,T);
    printTabStruct(T, *tailleTabStruct);


    /*int tabDic=malloc[256];
    elementDic* dico=malloc(sizeof(elementDic)*1000);
    int tailleCode=0;
    int *tailleDico=malloc(sizeof(int));
    CreationDic(tabDic,dico,T[*tailleTabStruct-1],tailleCode,tailleDico); 
    decompresseur(fileSource,fileSortie,T[*tailleTabStruct-1],nbrChars);

    */
}

//-------------------------------------------------------------------------//


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


  for(int i = 0 ; i < *TailleTabStruct ; i++)
    printf("nboccu = %i , ascii = %i \n" , T[i].occu ,T[i].codeAscii );
  
  /*
  char *c;
  c = charTobin('A');
  for(int i = 0 ; i<8 ; i++ )
    printf("%s \n" , c);
  c = intTobin(999);
  printf("%s \n" , c);
  */


  int newNbOccu[256];
  for(int i = 0 ; i<256 ; i++)
    newNbOccu[i]=0;
  
  //-----------------------FINAL-----------------------------------//
  
  printf("\n -------tailledic = %i ---------\n" ,*tailleDic );
 
  printf("-------COMPRESEUR-------------------------------------\n");

  compresseurTotal(argv[1] , argv[2] , D, *tailleDic ,T );
  
  

  printf("-----------------DECOMPRESEUR---------------------------\n");
 
  decompresseurTotal(argv[2],argv[3]);
  
   return(0);
}
