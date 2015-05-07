#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct auxVertice
{

    int rank;
    int id;
    struct AuxVertice *p;
} Vertice;

typedef struct auxAresta {
    Vertice *origem, *destino;
    int custo;
} Aresta;

typedef struct {
    int verticesQtd;
    int arestasQtd;
    Aresta **arestas;
    Vertice **vertices;
} Grafo;

void make_set(Vertice *v)
{
    v->p = v;
    v->rank = 0;
}

Vertice* find_set(Vertice *v)
{
    if(v != v->p)
        v->p = find_set(v->p);

    return v->p;
}

void link(Vertice *x, Vertice* y)
{
    if(x->rank > y->rank)
    {
        y->p = x;
    }
    else
    {
        x->p = y;
    }
    if(x->rank == y->rank)
        y->rank = y->rank+1;
}

void unionset(Vertice *x, Vertice *y)
{
    link(find_set(x),find_set(y));
}



Grafo* leArq(char *fileName) {
    FILE *file = fopen( fileName, "r" );
    if ( file == NULL ) {
            printf( "Nao foi possivel ler o arquivo %s\n", fileName);
    } else {
        Grafo *grafo = (Grafo*) malloc(sizeof(Grafo));
        int tamvert, tamarest;
        fscanf(file, "%d %d", &tamvert, &tamarest);
        grafo -> verticesQtd = tamvert;
        grafo -> arestasQtd =  tamarest;
        grafo -> arestas = (Aresta **)malloc((sizeof(Aresta*) * tamarest)+1);
        grafo->arestas[tamarest] = 0;
        grafo->vertices = (Vertice **)malloc((sizeof(Vertice*) * tamvert)+1);
        grafo->vertices[tamvert] = 0;

        for(int i = 0; i < tamvert; i++)
        {
            Vertice *v = (Vertice*)malloc(sizeof(Vertice));
            v->id = i;
            make_set(v);
            grafo->vertices[i] = v;
        }

        for(int i = 0; i< tamarest; i++)
        {
            int origem, destino, custo;
            fscanf(file, "%d %d %d", &origem, &destino, &custo);
            Aresta *aresta = (Aresta*) malloc(sizeof(Aresta));
            aresta -> origem = grafo->vertices[origem];
            aresta -> destino = grafo->vertices[destino];
            aresta -> custo =  custo;
            grafo->arestas[i] = aresta;
        }


        fclose( file );
        return grafo;
    }
    return NULL;
}

void salvaArqGrafo(const char * filename, Grafo *grafo) {
    FILE *out = fopen(filename,"w");
    if(out==NULL)
    {
        printf("Nao foi possivel escrever para '%s'\n", filename);
        return;
    }
    int w = 0;
    for(int i = 0;i < grafo->arestasQtd; i++)
    {
        w+=grafo->arestas[i]->custo;
    }
    fprintf(out,"%d\n",w);

    for(int i =0;i < grafo->arestasQtd; i++)
    {
        fprintf(out, "%d %d %d\n",grafo->arestas[i]->origem->id, grafo->arestas[i]->destino->id, grafo->arestas[i]->custo);
    }
    for(int i = 0;i < grafo->verticesQtd; i++)
    {
        Vertice *v = grafo->vertices[i]->p;
        fprintf(out, "%d ",v->id);
    }
    fprintf(out, "\n");
    for(int i = 0;i < grafo->verticesQtd; i++)
    {
        fprintf(out, "%d ",grafo->vertices[i]->rank);
    }
    fclose(out);
}

void sortGrafo(Grafo* grafo){
    for(int i = 0;i < grafo->arestasQtd; i++)
    {
        for(int j = 0;j < grafo->arestasQtd - 1; j++)
        {
            if(grafo->arestas[j]->custo > grafo->arestas[j+1]->custo)
            {
                Aresta *tmp = grafo->arestas[j];
                grafo->arestas[j] = grafo->arestas[j+1];
                grafo->arestas[j+1] = tmp;
            }
        }
    }

}

Grafo* kruskal(Grafo* grafo) {

    sortGrafo(grafo);

    Grafo *grafoMinimo = (Grafo*) malloc(sizeof(Grafo));
    grafoMinimo->verticesQtd = grafo->verticesQtd;
    grafoMinimo->arestasQtd = grafoMinimo->verticesQtd-1;
    grafoMinimo->arestas = (Aresta**)malloc(sizeof(Aresta*) * grafoMinimo->arestasQtd);
    grafoMinimo->vertices = (Vertice**)malloc(sizeof(Vertice*) * grafoMinimo->verticesQtd);
    for(int i = 0;i < grafoMinimo->verticesQtd; i++)
    {
        grafoMinimo->vertices[i] = grafo->vertices[i];
    }

    int elementos = 0;

    for(int i = 0;i < grafo->arestasQtd; i++)
    {
        if(find_set(grafo->arestas[i]->origem) != find_set(grafo->arestas[i]->destino))
        {
            unionset(grafo->arestas[i]->origem, grafo->arestas[i]->destino);
            grafoMinimo->arestas[elementos++] = grafo->arestas[i];
        }
    }


    return grafoMinimo;
}

int main(int argc, char *argv[]) {
    if ( argc != 3 ) {
        printf("Voce deve fornecer um arquivo de entrada e um de saida\n", argc);
        return 0;
    }
    Grafo *grafo = leArq(argv[1]);
    grafo = kruskal(grafo);
    salvaArqGrafo(argv[2], grafo);
    printf("Fim de execucao\n");
    return 0;
}



