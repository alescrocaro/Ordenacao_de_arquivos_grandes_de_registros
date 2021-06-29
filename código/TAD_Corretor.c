#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "TAD_FuncoesChave.h"
#include "trie.h"


Lista* arquivoParaLista(ALFATrie* dicionario, char* texto, int* qtdPalavras, int* qtdErradas)  // dicionario e arquivo de texto -> retorna lista com palavras erradas
{
    FILE* arq = fopen(texto, "rb");
    if(arq == NULL)
    {
        printf("\nErro ao abrir arquivo!\n");
        return -1;
    }

    Lista* list = criarLista();
    char* aux = calloc(47, sizeof(char));
    char c;

    char s[2];
    s[1] = 0;


    do  // percorre o arquivo todo, pegando caractere por caractere; quando encontra algo que não é uma letra (encontrou uma palavra inteira) verifica se é incorreta, se for salva ela na lista
    {
        c = getc(arq);//printf("%c", c);

        if(!ispunct(c) && !isspace(c) && !isdigit(c))
        {
            c = tolower(c);
            s[0] = (char)(c);
            strcat(aux, s);
        }
        else
        {
            ALFATrie* r = AT_Buscar(dicionario, aux);
            if(strcmp(aux, ""))   // aumenta qtd de palavras, se for uma palavra valida (diferente de nada)
            {
                (*qtdPalavras)++;
            }

            if(r == NULL)   // verifica se palavra não existe na trie dicionario (palavra incorreta)
            {
                (*qtdErradas)++;
                atribuirPalavra(list, aux);
            }
            else if(r != NULL && r->val == 0)   // verifica se a string existe na trie, mas não é uma palavra completa
            {
                int tmp = -1;
                tmp = strcmp(aux, "");
                if(tmp != 0)
                {
                    (*qtdErradas)++;
                    atribuirPalavra(list, aux);
                }
            }
                
            r = NULL;
            strcpy(aux, "");
        }
    } while(c != EOF);

    fclose(arq);
    return list;
}

static void PreencherTrieAux(ALFATrie** T, Lista* correcoes)   //insere as possiveis correcoes numa trie auxiliar
{
    int i = 1;
    No* auxLista = correcoes->primeiro;

    if(auxLista->palavra != NULL)
    {
        while(auxLista->prox != NULL)
        {
            char* auxS = calloc(strlen(auxLista->palavra)+2, sizeof(char)); // como usamos o fgets para construir o dicionario, e mudamos o AT_Inserir com um -1 para correção de bugs (\n), aqui tivemos que modificar a string inserindo o \n no final, assim conseguimos inserir o ultimo caracter da mesma na trie
            strcpy(auxS, auxLista->palavra);
            auxS[strlen(auxLista->palavra)] = '\n';
            //printf("%s\n", auxS);
            AT_Inserir(T, auxS, i);
            i++;
            free(auxS);
            
            auxLista = auxLista->prox;
        }
    }
}

//                                   REGRA 1

static void regraUm(ALFATrie* dicionario, char* p, ALFATrie** TrieAux)
{
    Lista* correcoes;
    
    char* auxP = calloc(47, sizeof(char));
    strcpy(auxP, p);
    int tamP = strlen(auxP);

    for(int i = 0; i < tamP; i++)   // insere '*' em cada posicao da string e salva as palavras resultantes
    {
        auxP[i] = '*';

        correcoes = TRIE_ChavesQueCasam(dicionario, auxP, 0);

        PreencherTrieAux(TrieAux, correcoes);

        //free(correcoes);
        strcpy(auxP, p);    // reseta a palavra pra nao duplicar os *
    }
    // free(auxP);
}

//                                   REGRA 2

static void regraDois(ALFATrie* dicionario, char* p, ALFATrie** TrieAux)
{
    Lista* correcoes;
    int n = strlen(p);
    char* auxP = calloc(n, sizeof(char));
    strcpy(auxP, p);

    for(int i = n-2; i < n; i++)    // exclui as ultimas letras da string
    {
        auxP[i] = '\0';
    }
    correcoes = TRIE_ChavesComPrefixo(dicionario, auxP);    // salva, em correcoes, todas palavras possiveis resultantes dos prefixos com n-3 e n-2
    
    PreencherTrieAux(TrieAux, correcoes);
    // exclusao de lista cm bug
    // free(auxP);
}



//                                   REGRA 3
static void regraTres(ALFATrie* dicionario, char* p, ALFATrie** TrieAux)
{
    char* maiorPrefix = TRIE_ChaveMaiorPrefixoDe(dicionario, p);    // salva o maior prefixo de p em maiorPrefix
    Lista* correcoes = TRIE_ChavesQueCasam(dicionario, maiorPrefix, 0);    //salva a palavra que casa com o maior prefixo de maiorPrefix
    
    if(correcoes->primeiro->palavra != NULL )   // insere a palavra resultante na lista, que depois é inserida na trie
    {
        char* palavraCerta = calloc(strlen(maiorPrefix), sizeof(char));
        strcpy(palavraCerta, correcoes->primeiro->palavra);
        atribuirPalavra(correcoes, palavraCerta);
        PreencherTrieAux(TrieAux, correcoes);
    }
    // free(palavraCerta) BUG
    // exclusao lista BUG
}

//                                   REGRA teste
/*
    esta regra utiliza a regra 1 duas vezes, assim encontra sugestoes para palavras com 2 letras erradas, como no caso de cemiveruo
*/
static void regraTeste(ALFATrie* dicionario, char* p, ALFATrie** TrieAux)
{
    Lista* correcoes;
    
    char* auxP = calloc(47, sizeof(char));
    strcpy(auxP, p);
    int tamP = strlen(auxP);

    for(int i = 0; i < tamP; i++)
    {
        auxP[i] = '*';
        for(int j = 0; j < tamP; j++)
        {
            auxP[j] = '*';

            correcoes = TRIE_ChavesQueCasam(dicionario, auxP, 0);

            PreencherTrieAux(TrieAux, correcoes);

            strcpy(auxP, p);    
            if(j != tamP - 1)    // depois de resetar a palavra, recoloca o *, exceto no ultimo looping do for, para nao ficar com 3 * em uma verificacao
            {
                auxP[i] = '*';
            }
        }
    }
    // free(auxP);
    // exclusao de lista BUG
}

//                                   REGRA 5
/*
    detectar palavra com uma letra sobrando, como "pungiu", "desapparecer", "permiptia"
*/

/*
static Lista* possiveisPalavras(char* p)
{
    int tamP = strlen(p);
    char* auxP = calloc(tamP, sizeof(char));
    strcpy(auxP, p);

    Lista* palavras = criarLista();
    No* auxL = palavras->primeiro;
    for(int i = 0; i < tamP; i++)
    {
        auxP[i] = 0;    // exclui uma letra da palavra
        int j = i;
        while(j < tamP)    // reorganiza a string
        {
            auxP[j] = auxP[j+1];
            j++;
        }
        auxL->palavra = auxP;
        atribuirPalavra(palavras, auxL->palavra);
        
        strcpy(auxP, p);
        auxL = auxL->prox;
    }

    return palavras;
}

static void regraCinco(ALFATrie* dicionario, char* p, ALFATrie** TrieAux)
{
    Lista* palavras = possiveisPalavras(p);
    No* noh = palavras->primeiro;
    
    while(noh->prox != NULL)
    {
        // if(strlen(noh->palavra) > )
    }
}
*/


//                                   CHAMA TODAS REGRAS
void CorrigirOrtografia(ALFATrie* dicionario)
{
    int qtdPalavras = 0;
    int qtdErradas = 0;
    int qtdSugestoes = 0;

    Lista* palavrasErradas = arquivoParaLista(dicionario, "all.txt", &qtdPalavras, &qtdErradas);
    if(palavrasErradas == -1)
    {
        return;
    }

    No* aux = palavrasErradas->primeiro;

    time_t t = clock();
    while(aux->prox != NULL)    // analisa todas palavras erradas
    {
        ALFATrie* T = NULL;
        printf("\npalavra que não está no dicionário: %s\nSugestões:\n", aux->palavra);

        
        
        regraUm(dicionario, aux->palavra, &T);

        if(strlen(aux->palavra) > 5)
        {
            regraDois(dicionario, aux->palavra, &T);
        }

        regraTres(dicionario, aux->palavra, &T);

        regraTeste(dicionario, aux->palavra, &T);

        // cria lista com todas sugestões
        Lista* listAux = TRIE_ChavesComPrefixo(T, "");

        qtdSugestoes = qtdSugestoes + listAux->qtd;

        // ordena lista de sugestoes
        bubbleSort(listAux);

        // free(noh);
        // free(nohTam);
        
        //AT_Imprimir(T);
        imprimirLista(listAux);
        aux = aux->prox;
        // free(listAux);
    }
    t = clock() - t;
    printf("\ntempo gasto: %fs\n", ((float)t/(CLOCKS_PER_SEC)));
    printf("Total de palavras do texto: %d\n", qtdPalavras);
    printf("Total de palavras ERRADAS do texto: %d\n", qtdErradas);

    float media = qtdSugestoes/qtdErradas;
    printf("Media de sugestoes por palavra incorreta: %f\n", media);
}
