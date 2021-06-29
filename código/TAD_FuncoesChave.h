#pragma once

#include "trie.h"
#include "TAD_Lista.h"


Lista* TRIE_ChavesComPrefixo(ALFATrie * T, char* prefix);

Lista* TRIE_ChavesQueCasam(ALFATrie *T, char* padrao, int n_extras);

char* TRIE_ChaveMaiorPrefixoDe(ALFATrie* T, char* s);
