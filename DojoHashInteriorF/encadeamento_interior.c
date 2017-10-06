#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "encadeamento_interior.h"
#include "lista_clientes.h"


void cria_hash(char *nome_arquivo_hash, int tam)
{
    FILE *arq_hash = fopen(nome_arquivo_hash, "w+b");
    int pos_hash = 0;
    Cliente *vazio;
    while(tam > pos_hash){
        vazio = cliente(-1,"",pos_hash,LIBERADO);
        salva_cliente(vazio,arq_hash);
        free(vazio);
        pos_hash++;
    }
    fclose(arq_hash);
}

int busca(int cod_cli, char *nome_arquivo_hash, int tam, int *encontrou)
{
    FILE *arq_hash = fopen(nome_arquivo_hash,"r+b");
    int pos_atual = cod_cli%tam , pos_livre = -1 ;
    *encontrou = 0; 
    fseek(arq_hash,pos_atual*sizeof(Cliente),SEEK_SET); // posiciona ponteiro na lista
    Cliente *p = le_cliente(arq_hash);
    while(!(*encontrou)){                   
        if(p->flag && p->cod_cliente == cod_cli){   // é o elemento desejado?
            pos_livre = pos_atual;
            *encontrou = 1;
        }else{
            if(!p->flag && pos_livre == -1) pos_livre = pos_atual;    // salva 1ª posição livre
            if(p->prox != pos_atual ){ //ele tem próximo que não aponta pra ele mesmo?
                pos_atual = p->prox; // atualiza posição do ponteiro
                fseek(arq_hash, p->prox*sizeof(Cliente), SEEK_SET); // avança próximo cliente
                free(p);
                p = le_cliente(arq_hash);
            }else{
               break;  //chegou no último elemento da lista encadeada
            }    
        }
               
    }
    free(p);
    fclose(arq_hash);
    return pos_livre;
}

int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, int tam)
{   
    int encontrou = 0;
    int i = busca(cod_cli,nome_arquivo_hash,tam,&encontrou);
    FILE *in = fopen(nome_arquivo_hash, "r+b");
    fseek(in, i * sizeof(Cliente), SEEK_SET);
    Cliente *p = le_cliente(in);
    if(i > -1 && p->flag){
        free(p);
        fclose(in);
        return -1;
    }
    free(p);
    i = cod_cli % tam;
    int j = tam -1 , pos_anterior = cod_cli % tam;
    printf("--------tam %d | i: %d | cod: %d\n",tam,i,cod_cli);
    fseek(in, i * sizeof(Cliente), SEEK_SET);
    p = le_cliente(in);
    //insere ínicio com cliente liberado ou vazio
    if(p->flag==LIBERADO){
        p->cod_cliente = cod_cli;
        strcpy(p->nome , nome_cli);
        p->flag = OCUPADO;
        fseek(in,-sizeof(Cliente), SEEK_CUR);
        salva_cliente(p,in);
        free(p);
    }else{
        //percorro a lista 
        while(p->flag){
            fseek(in,p->prox*sizeof(Cliente), SEEK_SET);
            free(p);
            p = le_cliente(in);
            if(p->flag){
                if(i!= p->prox){
                    i = p->prox;
                }else{    //percorrer a lista por todas as h(x) chaves começamdo do fim
                    free(p);
                    pos_anterior = i;
                    i = j;
                    j--;
                    if(i<0) { //não tem espaço 
                        fclose(in);
                        return -1;
                    }    
                    if(i == cod_cli%tam && i==tam) i--; //evita percorrer a lista inicial
                    fseek(in,i*sizeof(Cliente),SEEK_SET);
                    p = le_cliente(in);
                }
            }    
        }
        // insere o cliente no espaço LIBERADO
        p->cod_cliente = cod_cli;
        p->flag = OCUPADO;
        strcpy(p->nome,nome_cli);
        fseek(in,i*sizeof(Cliente), SEEK_SET);
        salva_cliente(p, in);
        free(p);
        if(pos_anterior != i){ //atualiza o ponteiro pro novo cliente inserido em posição com cod -1
            fseek(in,pos_anterior*sizeof(Cliente), SEEK_SET);
            p = le_cliente(in);
            p->prox = i;
            fseek(in,-sizeof(Cliente), SEEK_CUR);
            salva_cliente(p,in);
            free(p);
        }    
    }
    fclose(in);
    printf("i: %d \n ",i);
    return i;
}

int exclui(int cod_cli, char *nome_arquivo_hash, int tam)
{   
    int encontrou = 0;
    int pos = busca(cod_cli, nome_arquivo_hash, tam, &encontrou);
    if(pos >= 0){
        FILE *in = fopen(nome_arquivo_hash, "r+b");
        fseek(in, pos * sizeof(Cliente),SEEK_SET);
        Cliente *p = le_cliente(in);
        p->flag = LIBERADO;
        fseek(in, -sizeof(Cliente),SEEK_CUR);
        salva_cliente(p,in);
        free(p);
        fclose(in);
    }
    return pos;
}
