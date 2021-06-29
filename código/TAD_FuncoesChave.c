#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TAD_FuncoesChave.h"


///////////////////////////// CHAVES COM PREFIXO /////////////////////////////

static void TRIE_ChavesComPrefixo_R(ALFATrie * T, char* prefix, Lista* list, char* auxS, int* j)    // o j serve para controle de posicao na string
{ 
    if(T == NULL)
    {
        return;
    }

    if(T->val != 0)
    {
        int tam = strlen(auxS);
        auxS[tam+1] = 0;
        atribuirPalavra(list, auxS);
    }

    for(int i = 0; i < 26; i++)
    {
        if(T->filhos[i] != NULL)
        {
            auxS[(*j)++] = (char)(i+97);
            TRIE_ChavesComPrefixo_R(T->filhos[i], prefix, list, auxS, j);
        }
        auxS[(*j)] = 0;
    }
    (*j)--;
}

Lista* TRIE_ChavesComPrefixo(ALFATrie *T, char* prefix)
{
    Lista* list = criarLista();
    T = AT_Buscar(T, prefix);

    char* auxS = calloc(47, sizeof(char));
    strcpy(auxS, prefix);

    int j = strlen(auxS);

    TRIE_ChavesComPrefixo_R(T, prefix, list, auxS, &j); 

    return list;
}

///////////////////////////// CHAVES QUE CASAM /////////////////////////////

static void TRIE_ChavesQueCasam_R(ALFATrie *T, char* auxString, Lista* list, int p)
{
    if(T == NULL)
    {
        return;
    }

    char* chave = calloc(47, sizeof(char));
    strcpy(chave, auxString);
    
    if(p == strlen(chave))    // se a profundidade atual da palavra for = o tamanho da mesma, insere na lista
    {
        if(T->val != 0)
        {
            atribuirPalavra(list, chave);
            free(chave);
        }
        return;
    }

    if(auxString[p] == '*')     // verifica todas as chaves que casam com o padrao, conforme a posicao do coringa '*' (se a pos atual da palavra for um coringa)
    {
        for(int i = 0; i < 26; i++)
        {
            if(T->filhos[i] != NULL)
            {
                auxString[p] = (char)(i+97);
                TRIE_ChavesQueCasam_R(T->filhos[i], auxString, list, p+1);
                auxString[p] = '*';
            }
        }
    }
    else
    {
        TRIE_ChavesQueCasam_R(T->filhos[auxString[p]-97], auxString, list, p+1);
    }
}

Lista* TRIE_ChavesQueCasam(ALFATrie *T, char* padrao, int n_extras)
{
    Lista* list = criarLista();

    int tamChave = strlen(padrao) + n_extras;
    char* auxString = calloc(tamChave + 1, sizeof(char));
    memset(auxString, '*', tamChave);   // insere '*' nos lugares necessarios, conforme o padrao e o n_extras passados para a func
    memcpy(auxString, padrao, strlen(padrao));

    TRIE_ChavesQueCasam_R(T, auxString, list, 0);
    
    return list;
}

///////////////////////////// CHAVE MAIOR PREFIXO DE /////////////////////////////

static void TRIE_ChaveMaiorPrefixoDe_R(ALFATrie* T, Lista* list, char* s)
{
    ALFATrie* r = AT_Buscar(T, s);    // evita pegar prefixos maiores que não sejam palavras

    if(r != NULL)
    {
        atribuirPalavra(list, s);
    }

    if(strlen(s) == 0)
        return;

    char* auxString = calloc(strlen(s), sizeof(char));
    memcpy(auxString, s, strlen(s)-1);    // vai diminuindo a palavra a cada chamada recursiva

    TRIE_ChaveMaiorPrefixoDe_R(T, list, auxString);
    // free(auxString);
}

static char* maiorPrefix(ALFATrie* T, Lista* list)  // devolve o maior prefixo da lista de prefixos
{
    char* prefix = calloc(47, sizeof(char));
    int maior = -1;
    int tmp = -1;
    No* aux = list->primeiro;
    while(aux->prox != NULL)
    {
        tmp = strlen(aux->palavra);
        ALFATrie* r = AT_Buscar(T, aux->palavra);

        if(tmp > maior && r->val != 0)
        {
            maior = tmp;
            prefix = aux->palavra;
        }
        aux = aux->prox;
    }

    return prefix;
}

char* TRIE_ChaveMaiorPrefixoDe(ALFATrie* T, char* s)
{
    Lista* list = criarLista();

    TRIE_ChaveMaiorPrefixoDe_R(T, list, s);

    char* prefix = maiorPrefix(T, list);

    return prefix;
}