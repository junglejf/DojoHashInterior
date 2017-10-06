#ifndef CLIENTE_H
#define CLIENTE_H

#define LIBERADO 0
#define OCUPADO 1

#define TAM_NOME 100

#include <stdio.h>

typedef struct Cliente {
	int cod_cliente;
	char nome[TAM_NOME];
	int prox;
	int flag;
} Cliente;

// Imprime cliente
void imprime_cliente(Cliente *cliente);

// Cria cliente. Lembrar de usar free(cliente)
Cliente *cliente(int cod, char *nome, int prox, int flag);

// Salva cliente no arquivo out, na posicao atual do cursor
void salva_cliente(Cliente *cliente, FILE *out);

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
Cliente *le_cliente(FILE *in);

// Compara dois clientes
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_cliente(Cliente *c1, Cliente *c2);

// Retorna tamanho do cliente em bytes
int tamanho_cliente();

#endif