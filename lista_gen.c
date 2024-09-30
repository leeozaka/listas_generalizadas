#include "lista_gen.h"

// --- lista-generalizada ---

// lista_init gera uma lista com todos os argumentos zerados
// atencao: string nao inicializada
lista_generalizada *lista_generalizada_init() {
  return calloc(1, sizeof(lista_generalizada));
}

char nula(lista_generalizada *l) { return l == NULL; }

// nula vem antes de l->terminal para que evite um erro de segmentacao
char atomo(lista_generalizada *l) { return !nula(l) && l->terminal == ATOMO; }

lista_generalizada *head(lista_generalizada *l) {
  return nula(l) ? NULL : l->data.no.head;
}

lista_generalizada *tail(lista_generalizada *l) {
  return nula(l) ? NULL : l->data.no.tail;
}

// criaT: inicializa um novo nó de lista_generalizada
// com o tipo ATOMO
// um atomo é a parte irredutível de uma lista generalizada,
// e serve para guardar algum tipo de dado, nesse caso, uma string
lista_generalizada *criaT(const char *str) {
  lista_generalizada *t = lista_generalizada_init();
  t->terminal = ATOMO;
  t->data.str = malloc(strlen(str) * sizeof(char));
  strcpy(t->data.str, str);

  return t;
}

// cons: cria uma nova sequencia de listas generalizadas
// onde head pode ser um atomo, ou uma lista em um nivel abaixo
// @param h pode ser qualquer tipo de lista_generalizada
lista_generalizada *cons(lista_generalizada *h, lista_generalizada *t) {
  if (atomo(t))
    return NULL;

  lista_generalizada *cons = lista_generalizada_init();
  cons->terminal = LISTA;
  cons->data.no.head = h;
  cons->data.no.tail = t;

  return cons;
}

void exibe_recursivo(lista_generalizada *l) {
  if (!nula(l)) {
    if (atomo(l)) {
      printf("%s", l->data.str);
    } else {
      printf("[");
      while (!nula(l)) {
        exibe_recursivo(head(l));
        l = tail(l);
        if (!nula(l))
          printf(",");
      }
      printf("]");
    }
  }
}

// -- fim lista generalizada --
// -- lista de auxilio --

lista *lista_init() { return calloc(1, sizeof(lista)); }

void push(lista **l, lista_generalizada *head) {
  lista *new = lista_init();
  new->prox = *l;
  new->lista = head;

  *l = new;
}

lista_generalizada *pop(lista **l) {
  if (!*l) {
    return NULL;
  }

  lista_generalizada *aux = (*l)->lista;
  lista *aux2 = *l;

  *l = (*l)->prox;
  free(aux2);
  return aux;
}

lista *peek(lista *l) { return l ? l : NULL; }

void queue(lista **l, lista_generalizada *head, int nivel) {
  if (!head)
    return;

  lista *new = lista_init();

  new->lista = head;
  new->nivel = nivel;

  if (!(*l)) {
    *l = new;
    return;
  }

  lista *aux = *l;
  while (aux->prox)
    aux = aux->prox;
  aux->prox = new;
}

void printa_lista(lista *l) {
  while (l) {
    printf("[%d] %p %s ", l->nivel, l->lista, l->prox ? "->" : "");
    l = l->prox;
  }
  printf("\n");
}

// -- fim lista --
// -- operacoes com lista generalizada --

// devolve uma lista com o endereco de todos os nós lista da
// lista_generalizada
lista *gerar_niveis(lista_generalizada *l) {
  lista *l1 = NULL, *l2 = NULL;
  lista *niveis = NULL;

  int nivel_atual = 0;

  queue(&l1, l, nivel_atual);
  queue(&l2, l, nivel_atual);
  queue(&niveis, l, nivel_atual);

  while (l1) {
    l = pop(&l1);
    nivel_atual = peek(niveis)->nivel;
    pop(&niveis);

    while (l) {
      if (!atomo(head(l))) {
        queue(&l1, head(l), nivel_atual + 1);
        queue(&l2, head(l), nivel_atual + 1);
        queue(&niveis, l, nivel_atual + 1);
      }
      l = tail(l);
    }
  }

  while (niveis) {
    pop(&niveis);
  }

  return l2;
}

// devolve uma lista com o endereco de todos
// os anteriores aos nós lista da
// lista_generalizada
lista *gerar_niveis_ant(lista_generalizada *l) {
  lista *l1 = NULL, *l2 = NULL;
  lista *niveis = NULL;

  int nivel_atual = 1;

  queue(&l1, l, nivel_atual);
  queue(&niveis, l, nivel_atual);

  while (l1) {
    l = pop(&l1);
    nivel_atual = peek(niveis)->nivel;
    pop(&niveis);

    while (l) {
      if (!atomo(head(l))) {
        queue(&l1, head(l), nivel_atual + 1);
        queue(&l2, l, nivel_atual + 1);
        queue(&niveis, l, nivel_atual + 1);
      }
      l = tail(l);
    }
  }

  return l2;
}

// considerando strings para essa parte
void ordena_lista(lista_generalizada *l) {
  lista *niveis = gerar_niveis(l);
  lista_generalizada *aux;
  lista_generalizada *aux2;

  while (niveis) {
    aux = pop(&niveis);
    while (aux) {
      aux2 = tail(aux);
      while (aux2) {
        if (atomo(head(aux)) && atomo(head(aux2))) {
          if (strcmp(head(aux)->data.str, head(aux2)->data.str) > 0) {
            void *end = aux->data.no.head;
            aux->data.no.head = aux2->data.no.head;
            aux2->data.no.head = end;
          }
        }
        aux2 = tail(aux2);
      }
      aux = tail(aux);
    }
  }
}

void deletar_nivel(lista_generalizada **l, int nivel) {
  lista *niveis = gerar_niveis(*l);
  lista *niveis_ant = gerar_niveis_ant(*l);
  lista *niveis_out = NULL;

  while (niveis) {
    lista *aux = peek(niveis);
    pop(&niveis);

    if (aux->nivel >= nivel)
      queue(&niveis_out, aux->lista, aux->nivel);
  }

  // o codigo abaixo só funciona pro terceiro nivel em diante por enquanto
  while (niveis_out) {
    lista *niveis_out_aux = niveis_ant;

    lista_generalizada *sub_ant = NULL;
    lista_generalizada *ant = NULL;
    lista_generalizada *atual = NULL;

    while (niveis_out_aux && head(niveis_out_aux->lista) != niveis_out->lista) {
      niveis_out_aux = niveis_out_aux->prox;
    }
    // ant: head aponta pro atual
    ant = niveis_out_aux->lista;

    niveis_out_aux = niveis_ant;
    while (niveis_out_aux && head(niveis_out_aux->lista) != ant) {
      niveis_out_aux = niveis_out_aux->prox;
    }
    // sub_ant: trocar o head que aponta pro nivel para o proximo elemento
    sub_ant = niveis_out_aux->lista;

    // head de ant = atual
    atual = niveis_out->lista;

    while (atual) {
      lista_generalizada *x = atual;
      atual = tail(atual);

      free(head(x));
      free(x);
    }

    sub_ant->data.no.head = tail(ant);
    free(ant);

    pop(&niveis_out);
  }
}

void insereNoNivel(lista_generalizada **lgen, lista_generalizada *novo_elemento,
                   int nivel_desejado) {
  lista *l = NULL;
  push(&l, *lgen);
  int nivel_atual = 1;
  lista_generalizada *atual = *lgen;

  if (nivel_desejado == 1) {
    lista_generalizada *novo_no = cons(novo_elemento, *lgen);
    *lgen = novo_no;
    return;
  }

  while (l) {
    if (!nula(atual)) {
      atual = pop(&l);
      while (!nula(atual) && !atomo(head(atual)) &&
             nivel_atual != nivel_desejado - 1) {
        push(&l, atual);
        atual = head(atual);
        nivel_atual++;
      }
    }

    if (nivel_atual == nivel_desejado - 1) {
      if (!atomo(head(atual))) {
        lista_generalizada *novo_no = cons(novo_elemento, head(atual));
        atual->data.no.head = novo_no;
      } else {
        if (!nula(tail(atual))) {
          push(&l, atual);
          nivel_atual++;
        }
      }
    }

    if (l) {
      atual = pop(&l);
      nivel_atual--;
    }

    atual = tail(atual);
    if (!nula(atual))
      push(&l, atual);
  }
}

void insere_ordenado(lista_generalizada *l, lista_generalizada *at, int nivel) {
  if (!atomo(at)) {
    return;
  }

  lista *niveis = gerar_niveis(l);
  lista *niveis_ant = gerar_niveis_ant(l);
  lista *niveis_out = NULL;

  while (!niveis_out && niveis) {
    lista *aux = peek(niveis);
    pop(&niveis);

    if (aux->nivel == nivel) {
      queue(&niveis_out, aux->lista, aux->nivel);
    }
  }

  while (niveis) {
    pop(&niveis);
  }

  lista_generalizada *atual = pop(&niveis_out);
  lista_generalizada *ant = NULL;

  while (atual && strcmp(head(atual)->data.str, at->data.str) < 0) {
    ant = atual;
    atual = tail(atual);
  }

  lista_generalizada *novo;

  if (!ant) {
    while (head(peek(niveis_ant)->lista) != atual) {
      pop(&niveis_ant);
    }

    novo = cons(at, atual);
    peek(niveis_ant)->lista->data.no.head = novo;
  } else {
    cons(at, NULL);
    ant->data.no.tail = novo;
  }

  while (niveis_out) {
    pop(&niveis_out);
  }
}
