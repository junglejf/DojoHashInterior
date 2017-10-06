#ifndef LISTA_CLIENTES_H
#define LISTA_CLIENTES_H

#include "cliente.h"

typedef struct ListaClientes {
	Cliente **lista;
	int qtd;
} ListaClientes;

// Imprime clientes
void imprime_clientes(ListaClientes *lc);

// Cria lista de clientes. Lembrar de usar libera_clientes(lista_clientes)
ListaClientes *cria_clientes(int qtd, ...);

// Salva lista de clientes no arquivo nome_arquivo
void salva_clientes(char *nome_arquivo, ListaClientes *lc);

// Le lista de clientes do arquivo nome_arquivo
ListaClientes *le_clientes(char *nome_arquivo);

// Compara duas listas de clientes
// Retorna 1 se os clientes das duas listas forem iguais
// e 0 caso contrario
int cmp_clientes(ListaClientes *c1, ListaClientes *c2);

// Desaloca lista de clientes
void libera_clientes(ListaClientes *lc);

#endif