#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

static ALFATrie* AT_Buscar_R(ALFATrie* T, unsigned char *chave, int n, int p)
{
    if(T == NULL)
    {
        return NULL;
    }

    if(p == n)
    {
        return T;
    }

    return AT_Buscar_R(T->filhos[chave[p]-97], chave, n, p+1);  // -97 serve para encontrar a posição no vetor do caractere referente
}

ALFATrie* AT_Buscar(ALFATrie* T, unsigned char *chave)
{
    return AT_Buscar_R(T, chave, strlen(chave), 0);
}

ALFATrie* AT_Criar()
{
    ALFATrie* noh = malloc(sizeof(ALFATrie));

    noh->estado = AT_LIVRE;
    noh->val = 0;
    noh->tam = 0;

    for(int i = 0; i < 26; i++)
    {
        noh->filhos[i] = NULL;
    }

    return noh;
}

static void AT_Inserir_R(ALFATrie **T, unsigned char *chave, int val, int n, int p)
{
    if((*T) == NULL)
    {
        *T = AT_Criar();
    }
    if(p == n-1)    // (n-1) -> dicionario feito com fgets
    {
        (*T)->val = val;
        (*T)->estado = AT_OCUPADO;
        return;
    }
    AT_Inserir_R(&(*T)->filhos[chave[p]-97], chave, val, n, p+1);
}

void AT_Inserir(ALFATrie **T, unsigned char *chave, int val)
{
    AT_Inserir_R(T, chave, val, strlen(chave), 0);
    (*T)->tam += 1;
}

static void AT_Imprimir_R(ALFATrie *T, unsigned char c, int nivel)
{
    if(T == NULL) 
    {
        return;
    }

    for(int i = 0; i < nivel; i++)
    { 
        printf("-");
    }

    char e = (T->estado == 0) ? 'L': 'O';
    printf("(%c) %d (%c)\n", c, T->val, e);

    for(int i = 0; i < 26; i++)
    {
        if(T->filhos[i] != NULL)
        {
            AT_Imprimir_R(T->filhos[i], i+97, nivel+1);
        }
    }
}

void AT_Imprimir(ALFATrie *T)
{
    AT_Imprimir_R(T, 0, 0);
}

int AT_Limpa(ALFATrie *T)
{
    if(T == NULL)
    {
        return 0;
    }

    int tmp = 0;

    for(int i = 0; i < 26; i++)
    {
        if(T->filhos[i] != NULL)
        {
            return AT_Limpa(T->filhos[i]);
        }
        else
        {
            tmp++;
        }
    }

    if(tmp == 26 && T->estado == AT_OCUPADO)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//                              DICIONARIO
ALFATrie* ConstruirDicionario(char* arq_lista_palavras)
{
    FILE* arq = fopen(arq_lista_palavras, "rb");
    char* texto_str = calloc(47, sizeof(char));
    ALFATrie* T = NULL;
    int i = 1;  // se o val da letra é > 0, significa que é o 'fim' de uma palavra. 
                // ja fizemos isso pensando que teria algo sobre colocar em ordem alfabética (se o dicionario fosse passado assim)
    
    while(fgets(texto_str, 47, arq) != NULL)    // insere palavra por palavra na trie dicionario
    {
        strlwr(texto_str);
        AT_Inserir(&T, texto_str, i);
        i++;
    }
    
    fclose(arq);

    return T;
}