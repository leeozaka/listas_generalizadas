#pragma once
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

// -- lista genralizada --
enum { LISTA, ATOMO };

struct info {
  char *str;
};

typedef struct lista_generalizada {
  char terminal;
  union data {
    struct info;
    struct no {
      struct lista_generalizada *head;
      struct lista_generalizada *tail;
    } no;
  } data;
} lista_generalizada;

lista_generalizada *lista_generalizada_init();

char atomo(lista_generalizada *l);
char nula(lista_generalizada *l);

lista_generalizada *head(lista_generalizada *l);
lista_generalizada *tail(lista_generalizada *l);

lista_generalizada *cons(lista_generalizada *h, lista_generalizada *t);
lista_generalizada *criaT(const char *str);

void rm(lista_generalizada **l);

void exibe_recursivo(lista_generalizada *l);
// -- fim lista generalizada --

// -- lista generica --
typedef struct lista_generica {
  lista_generalizada *lista;
  int nivel;
  struct lista_generica *prox;
} lista;

lista *init();

// operacoes de empilhar (1o elemento)
void push(lista **l, lista_generalizada *lhead);
lista_generalizada *pop(lista **l);
lista *peek(lista *l);

// operacoes de fila (ultimo elemento)
void queue(lista **l, lista_generalizada *head, int nivel);

void printa_lista(lista *l);
// -- fim lista generica --

// operacoes com a lista generalizada
lista *gerar_niveis(lista_generalizada *l);
lista *gerar_niveis_ant(lista_generalizada *l);
void ordena_lista(lista_generalizada *l);
void insere_no_nivel(lista_generalizada **l, lista_generalizada *novo_elemento,
                     int nivel_desejado);
void deletar_nivel(lista_generalizada **l, int nivel);
void insere_ordenado(lista_generalizada *l, lista_generalizada *at, int nivel);
