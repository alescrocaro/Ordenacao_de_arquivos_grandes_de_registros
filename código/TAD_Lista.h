#pragma once

typedef struct No
{
    char* palavra;
    struct No* prox;
} No;

typedef struct Lista
{
    No* primeiro;
    No* ultimo;
    int qtd;
} Lista;


Lista* criarLista();

void atribuirPalavra(Lista* list, char* insercao);

void imprimirLista(Lista* list);

//ordenacao
void bubbleSort(Lista* l);