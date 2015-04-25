#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
typedef int bool;
#define true 1
#define false 0

typedef struct auxAresta {
   int origem;
   int destino;
   double custo;
   struct auxAresta *prox;
} Aresta;

typedef struct {
   int verticesQtd;
   int arestasQtd;
   Aresta *primeiraAresta;
   int *p;
   int *rank;
} Grafo;

Grafo* leArq(char *fileName) {
   FILE *file = fopen( fileName, "r" );
   if ( file == NULL ) {
         printf( "Nao foi possivel ler o arquivo %s\n", fileName);
   } else {
      Grafo *grafo = (Grafo*) malloc(sizeof(Grafo));
      char str[60];
      if  ( fgets (str, 60, file) != NULL ) {
         grafo -> verticesQtd = atoi(strtok(str, " "));
         grafo -> arestasQtd =  atoi(strtok(NULL, " "));
      }

      Aresta *arestaAnterior = NULL;
      grafo -> primeiraAresta = NULL;
      while  ( fgets (str, 60, file) != NULL ) {
         Aresta *aresta = (Aresta*) malloc(sizeof(Aresta));
         if (arestaAnterior != NULL) {
            arestaAnterior -> prox =  aresta;
         }
         aresta -> origem = atoi(strtok(str, " "));
         aresta -> destino =  atoi(strtok(NULL, " "));
         aresta -> custo =  atoi(strtok(NULL, " "));
         arestaAnterior = aresta;
         if (grafo -> primeiraAresta == NULL) {
            grafo -> primeiraAresta = aresta;
         }
      }
      fclose( file );
      return grafo;
   }
   return NULL;
}

void salvaArqGrafo(int arqNumero, Grafo *grafo) {
   double custoArvore = 0;
   Aresta *aresta = grafo -> primeiraAresta;
   while (aresta != NULL) {
      custoArvore += aresta -> custo;
      aresta = aresta -> prox;
   }
   char fileName[20];
   sprintf(fileName, "saida%d.txt", arqNumero);
   
   FILE *file = fopen(fileName, "w");
   fprintf(file, "%f\n", custoArvore);

   aresta = grafo -> primeiraAresta;
   while (aresta != NULL) {
      int origem = aresta -> origem;
      int destino = aresta -> destino;
      double custo = aresta -> custo;
      fprintf(file, "%d %d %f\n", origem, destino, custo);
      aresta = aresta -> prox;
   }

   char pString[50];
   char rankString[50];
   int *p = grafo -> p;
   int *rank = grafo -> rank;

   int tamanhoArray = grafo -> verticesQtd;
   
   int x;
   for (x = 0; x < tamanhoArray; x++) {
      if (x==0) {
         sprintf(pString, "%d", p[x]);
      } else {
         sprintf(pString, "%s %d", pString, p[x]);
      }
   }

   for (x = 0; x < tamanhoArray; x++) {
      if (x==0) {
         sprintf(rankString, "%d", rank[x]);
      } else {
         sprintf(rankString, "%s %d", rankString, rank[x]);
      }
   }

   fprintf(file, "%s\n", pString);
   fprintf(file, "%s\n", rankString);

   fclose(file);
}

Grafo* kruskal(Grafo* grafo) {
   int tamanhoArray = grafo -> verticesQtd;
   int *pais = malloc(tamanhoArray * sizeof(int));
   int *rank = malloc(tamanhoArray * sizeof(int));

   // pedaco de código para teste da saida
   // a quantidade de pais e do rank será sempre a quantidade de vertices declaradas
   for (int x = 0; x < tamanhoArray; x ++) {
      pais[x] = x;
   }
   for (int x = 0; x < tamanhoArray; x ++) {
      rank[x] = x;
   }
   // fim do pedaco de teste
   
   grafo -> p = pais;
   grafo -> rank = rank;
   return grafo;
}

int main(int argc, char *argv[]) { 
   if ( argc < 2 ) {
      printf("Pelo menos um arquivo deve ser fornecido para executar o programa\n");
      return 0;
   } 
   int arqAtual = 1;
   while (arqAtual < argc) {
      Grafo *grafo = leArq(argv[arqAtual]);
      grafo = kruskal(grafo);
      salvaArqGrafo(arqAtual, grafo);
      arqAtual++;
   }
   printf("Fim de execucao\n");
   return 0;
}



