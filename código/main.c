#include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
#include "trie.h"
#include "TAD_Lista.h"
// #include "TAD_FuncoesChave.h"
#include "TAD_Corretor.h"

int main(int argc, char** argv)
{
    ALFATrie* D = ConstruirDicionario("dicionario.txt");
    //AT_Imprimir(D);

    CorrigirOrtografia(D);

    AT_Limpa(D);

    system("pause");
}