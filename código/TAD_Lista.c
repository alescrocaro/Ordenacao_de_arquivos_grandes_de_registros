#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TAD_Lista.h"

Lista* criarLista()
{
    Lista* list = malloc(sizeof(Lista));

    list->primeiro = malloc(sizeof(No));
    list->primeiro->palavra = NULL;
    list->ultimo = list->primeiro;
    list->qtd = 0;

    return list;
}

void atribuirPalavra(Lista* list, char* insercao)
{
    char* chave = calloc(strlen(insercao)+1, sizeof(char));
    strcpy(chave, insercao);

    list->ultimo->palavra = chave;

    No* aux = malloc(sizeof(No));
    aux->palavra = NULL;
    aux->prox = NULL;

    list->ultimo->prox = aux;
    list->ultimo = aux;

    list->qtd++;
}

void imprimirLista(Lista* listaPalavras)
{
    No* aux = listaPalavras->primeiro;
    int i = 0;

    if(aux->palavra != NULL)    // se não existir palavras na lista, nem imprime nada.
    {
        while(aux->prox != NULL)
        {
            printf("%s", aux->palavra);
            if(aux->prox->prox != NULL)  // não imprime "," na ultima palavra
            {
                printf(", ");
            }
            aux = aux->prox;
            i++;
        }
    }
    printf("\n");
}

// ordenacao
void bubbleSort(Lista* l)
{
    for(int i = l->qtd - 1; i > 0; i--)
    {
        No* noh = l->primeiro;    // recupera o endereço toda vez no loop
        for(int j = 0; j < i; j++)
        {
            if(noh->prox->palavra == NULL)
            {
                break;
            }
            if(strlen(noh->palavra) > strlen(noh->prox->palavra))   // coloca as menores palavras primeiro
            {
                char* auxS = calloc(47, sizeof(char));
                char* auxS2 = calloc(47, sizeof(char));

                strcpy(auxS, noh->palavra);
                strcpy(auxS2, noh->prox->palavra);
                // free(noh->palavra);

                noh->palavra = auxS2;
                noh->prox->palavra = auxS;
            }

            noh = noh->prox;
        }
    }
}