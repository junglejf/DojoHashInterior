#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define BEFORE_TEST before();

#define NOME_ARQUIVO_HASH "tabHash.dat"

#include <limits.h>
#include <stdlib.h>

#include "ufftest.h"
#include "lista_clientes.h"
#include "cliente.h"
#include "encadeamento_interior.h"


void before() {
	remove(NOME_ARQUIVO_HASH);
}

BEGIN_TESTS();

TEST("Cria Tabela Vazia. Testa o caso de tabela hash com compartimentos vazios");
if (!skip) {
	// Estamos usando tabela hash de tamanho 7
	// Adicionar 7 compartimentos que nao apontam para ninguem (prox = -1)
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(-1, "", 0, LIBERADO),
		cliente(-1, "", 1, LIBERADO),
		cliente(-1, "", 2, LIBERADO),
		cliente(-1, "", 3, LIBERADO),
		cliente(-1, "", 4, LIBERADO),
		cliente(-1, "", 5, LIBERADO),
		cliente(-1, "", 6, LIBERADO));

	cria_hash(NOME_ARQUIVO_HASH, 7);
        
	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);
        
	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

TEST("Busca chave que eh encontrada na primeira tentativa");
if (!skip) {
	ListaClientes *tab_cliente;

	tab_cliente = cria_clientes(7,
		cliente(-1, "", 0, LIBERADO),
		cliente(50, "Carlos", 1, OCUPADO),
		cliente(-1, "", 2, LIBERADO),
		cliente(-1, "", 3, LIBERADO),
		cliente(-1, "", 4, LIBERADO),
		cliente(-1, "", 5, LIBERADO),
		cliente(-1, "", 6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);
        
	int encontrou = -1;
	int end = busca(50, NOME_ARQUIVO_HASH, 7, &encontrou);
        
	// endereco retornado deve ser igual a 1
	ASSERT_EQUAL(end, 1);
	// encontrou deve ser 1
	ASSERT_EQUAL(encontrou, 1);

	libera_clientes(tab_cliente);
}

TEST("Busca chave que existia mas foi removida");
if (!skip) {
	ListaClientes *tab_cliente;

	tab_cliente = cria_clientes(7,
		cliente(-1, "", 0, LIBERADO),
		cliente(50, "Carlos", 1, LIBERADO),
		cliente(-1, "", 2, LIBERADO),
		cliente(-1, "", 3, LIBERADO),
		cliente(-1, "", 4, LIBERADO),
		cliente(-1, "", 5, LIBERADO),
		cliente(-1, "", 6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);
        printf("teste 3 :\n");
	int encontrou = -1;
	int end = busca(50, NOME_ARQUIVO_HASH, 7, &encontrou);
        printf("--------------:\n");
	// endereco retornado deve ser igual a 1
	ASSERT_EQUAL(end, 1);
	// encontrou deve ser 0
	ASSERT_EQUAL(encontrou, 0);

	libera_clientes(tab_cliente);
}

TEST("Busca chave que eh encontrada na segunda tentativa");
if (!skip) {
	ListaClientes *tab_cliente;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, OCUPADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);

	int encontrou = -1;
	int end = busca(10, NOME_ARQUIVO_HASH, 7, &encontrou);

	// endereco retornado deve ser igual a 4
	ASSERT_EQUAL(end, 4);
	// encontrou deve ser 1
	ASSERT_EQUAL(encontrou, 1);

	libera_clientes(tab_cliente);
}

TEST("Busca chave inexistente");
if (!skip) {
	ListaClientes *tab_cliente;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, LIBERADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);

	int encontrou = -1;
	int end = busca(17, NOME_ARQUIVO_HASH, 7, &encontrou);

	// endereco retornado deve ser igual a 3, pois ele achou um LIBERADO durante a busca
	ASSERT_EQUAL(end, 3);
	// encontrou deve ser 0, pois chave nao foi encontrada na tabela
	ASSERT_EQUAL(encontrou, 0);

	libera_clientes(tab_cliente);
}

TEST("Busca chave que havia sido removida, mas foi reinserida mais adiante no arquivo");
if (!skip) {
	ListaClientes *tab_cliente;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",     0, OCUPADO),
		cliente(-1,  "",         1, LIBERADO),
		cliente(51,  "Ana",      2, OCUPADO),
		cliente(59,  "Maria",    4, OCUPADO),
		cliente(10,  "Janio",    6, LIBERADO),
		cliente(103, "Pedro",    5, OCUPADO),
		cliente(10,  "Mauricio", 6, OCUPADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);

	int encontrou = -1;
	int end = busca(10, NOME_ARQUIVO_HASH, 7, &encontrou);

	// endereco retornado deve ser igual a 6
	ASSERT_EQUAL(end, 6);
	// encontrou deve ser 1, pois chave foi encontrada na tabela
	ASSERT_EQUAL(encontrou, 1);

	libera_clientes(tab_cliente);
}

TEST("Insercao na 1 tentativa. Existe compartimento vazio na tabela para receber o registro");
if (!skip) {
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(-1,  "",       0, LIBERADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, OCUPADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);
	int end = insere(49, "Joao", NOME_ARQUIVO_HASH, 7);
	ASSERT_EQUAL(end, 0);
	free(tab_cliente->lista[0]);
	tab_cliente->lista[0] = cliente(49, "Joao", 0, OCUPADO);
	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);

	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

TEST("Insercao de registro com chave que ja existe");
if (!skip) {
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, OCUPADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);
        
	int end = insere(49, "Jorge", NOME_ARQUIVO_HASH, 7);
	ASSERT_EQUAL(end, -1);
        
	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);

	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

TEST("Insercao no final da lista encadeada");
if (!skip) {
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, OCUPADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);
        printf("\n linha235 \n");
	int end = insere(87, "Mariana", NOME_ARQUIVO_HASH, 7);
	ASSERT_EQUAL(end, 6);
        printf("\n ---------- \n");
	tab_cliente->lista[4]->prox = 6;
	free(tab_cliente->lista[6]);
	tab_cliente->lista[6] = cliente(87, "Mariana", 6, OCUPADO);
	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);
        imprime_clientes(tab_cliente);
        printf("\n ----------| \n");
        imprime_clientes(tab_cliente_saida);
	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

TEST("Insercao de registro no espaco vazio deixado por registro excluido");
if (!skip) {
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, LIBERADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);
        
	int end = insere(87, "Mariana", NOME_ARQUIVO_HASH, 7);
	ASSERT_EQUAL(end, 3);

	free(tab_cliente->lista[3]);
	tab_cliente->lista[3] = cliente(87, "Mariana", 4, OCUPADO);
	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);

	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

TEST("Exclusao de chave inexistente");
if (!skip) {
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, OCUPADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);

	int end = exclui(87, NOME_ARQUIVO_HASH, 7);
	ASSERT_EQUAL(end, -1);

	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);

	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

TEST("Exclusao do primeiro noh da lista encadeada de um determinado compartimento");
if (!skip) {
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, OCUPADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);

	int end = exclui(59, NOME_ARQUIVO_HASH, 7);
	ASSERT_EQUAL(end, 3);

	tab_cliente->lista[3]->flag = LIBERADO;
	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);

	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

TEST("Exclusao do ultimo noh da lista encadeada de um determinado compartimento");
if (!skip) {
	ListaClientes *tab_cliente, *tab_cliente_saida;

	tab_cliente = cria_clientes(7,
		cliente(49,  "Joao",   0, OCUPADO),
		cliente(-1,  "",       1, LIBERADO),
		cliente(51,  "Ana",    2, OCUPADO),
		cliente(59,  "Maria",  4, OCUPADO),
		cliente(10,  "Janio",  4, OCUPADO),
		cliente(103, "Pedro",  5, OCUPADO),
		cliente(-1,  "",       6, LIBERADO));
	salva_clientes(NOME_ARQUIVO_HASH, tab_cliente);
        
	int end = exclui(10, NOME_ARQUIVO_HASH, 7);
	ASSERT_EQUAL(end, 4);
        
        
	tab_cliente->lista[4]->flag = LIBERADO;
	tab_cliente_saida = le_clientes(NOME_ARQUIVO_HASH);
        
        
        
	ASSERT_EQUAL_CMP(tab_cliente, tab_cliente_saida, cmp_clientes);
	libera_clientes(tab_cliente);
	libera_clientes(tab_cliente_saida);
}

END_TESTS();