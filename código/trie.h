#pragma once

typedef enum ALFATrie_Estado
{
    AT_LIVRE,
    AT_OCUPADO
} ALFATrie_Estado;

typedef struct ALFATrie
{
    int val;
    int tam;
    struct ALFATrie *filhos[26];
    ALFATrie_Estado estado;
} ALFATrie;

ALFATrie* AT_Criar();

ALFATrie* AT_Buscar(ALFATrie* T, unsigned char *chave);

void AT_Inserir(ALFATrie** T, unsigned char *chave, int val);

void AT_Imprimir(ALFATrie *T);

int AT_Limpa(ALFATrie *T);

//dicionario
ALFATrie* ConstruirDicionario(char* arq_lista_palavras);
