#include <stdio.h>
#include <stdlib.h>
#include "arn.h"

#define RUBRO 0
#define NEGRO 1

/* Implementação de uma lista duplamente  encadeada com nó cabeça. */

typedef struct no_s {
  int chave;
  char cor;
  struct no_s *pai;
  struct no_s *esq;
  struct no_s *dir;
} no;

typedef struct arn_s {
  no *raiz;
  no *nulo;
} arn;

/* Declaração das funções estáticas */

static void apagarRecursivo(arn * T, no *x);
static void imprimirRecursivo(arn * T, no *x);
static void imprimirRecursivoBonitinha(arn * T, no *x, int prof);
static void consertarInclusaoRN(arn *T, no *z);
static void rotacaoEsq(arn *T, no *x);
static void rotacaoDir(arn *T, no *x);
static void transplantar(arn *T, no *u, no *v);
static no *min(arn *T, no *x);
static void consertarRemocaoRN(arn *T, no *x);

/* Cria uma arn e retorna um ponteiro para ela. */
void *criarArn(){
  arn *T;

  T = (arn *) malloc(sizeof(arn));
  T->nulo = (no *) malloc(sizeof(no));
  T->nulo->cor = NEGRO;
  T->raiz = T->nulo;
  return (void *)T;
}

/* Libera o espaço da arn T da memória junto com todos os seus nós. */
void apagarArn(void *T){
  arn *Ti;

  Ti = (arn *) T;
  apagarRecursivo(Ti, Ti->raiz);
  free(Ti->nulo);
  free(Ti);
}

static void apagarRecursivo(arn * T, no *x) {
  if (x != T->nulo) {
    apagarRecursivo(T, x->esq);
    apagarRecursivo(T, x->dir);
    free(x);
  }
}

/* Imprime os elementos da arn T em ordem. */
void imprimirArn(void *T){
  arn *Ti;

  Ti = (arn *) T;
  imprimirRecursivo(Ti, Ti->raiz);
  printf("\n");
}

static void imprimirRecursivo(arn * T, no *x) {
  if (x != T->nulo) {
    imprimirRecursivo(T, x->esq);
    printf("%d ", x->chave);
    imprimirRecursivo(T, x->dir);
  }
}

void imprimirArnBonitinha(void *T) {
  arn *Ti;

  Ti = (arn *) T;
  imprimirRecursivoBonitinha(Ti, Ti->raiz, 0);
}

static void imprimirRecursivoBonitinha(arn * T, no *x, int prof) {
  if (x != T->nulo) {
    imprimirRecursivoBonitinha(T, x->dir, prof+1);
    for (int i = 0; i < prof; i++) {
      printf("  ");
    }
    printf("%d", x->chave);
    if (x->cor == RUBRO)
      printf("R\n");
    else
      printf("N\n");
    imprimirRecursivoBonitinha(T, x->esq, prof+1);
  }
}


/* Inclui o inteiro k na arn T. */
void incluirArn(void *T, int k){
  no *x, *y, *z;
  arn *Ti;
  //printf("Inicio Inclusao %d\n", k);

  Ti = (arn *) T;

  z = (no *) malloc(sizeof(no));
  z->chave = k;

  y = Ti->nulo;
  x = Ti->raiz;

  while (x != Ti->nulo) {
    y = x;
    if (k < x->chave)
      x = x->esq;
    else
      x = x->dir;
  }

  z->pai = y;
  if (y == Ti->nulo)
    Ti->raiz = z;
  else if (k < y->chave)
    y->esq = z;
  else
    y->dir = z;

  z->esq = z->dir = Ti->nulo;
  z->cor = RUBRO;
  //printf("Inicio Consertar Inclusao %d\n", k);
  consertarInclusaoRN(Ti, z);
  //printf("Fim Consertar Inclusao %d\n", k);
}

static void consertarInclusaoRN(arn *T, no *z) {
  no *y;

  while (z->pai->cor == RUBRO) {
    if (z->pai == z->pai->pai->esq) {
      y = z->pai->pai->dir;
      if (y->cor == RUBRO) { // Caso 1
        z->pai->cor = NEGRO;
        y->cor = NEGRO;
        z = y->pai;
        z->cor = RUBRO;
      }
      else {
        if (z == z->pai->dir) { //Caso 2
          z = z->pai;
          rotacaoEsq(T, z);
        }
        // Caso 3
        z->pai->cor = NEGRO;
        z->pai->pai->cor = RUBRO;
        rotacaoDir(T, z->pai->pai);
      }
    }
    else { // Casos simétricos
      y = z->pai->pai->esq;
      if (y->cor == RUBRO) { // Caso 1
        z->pai->cor = NEGRO;
        y->cor = NEGRO;
        z = y->pai;
        z->cor = RUBRO;
      }
      else {
        if (z == z->pai->esq) { //Caso 2
          z = z->pai;
          rotacaoDir(T, z);
        }
        // Caso 3
        z->pai->cor = NEGRO;
        z->pai->pai->cor = RUBRO;
        rotacaoEsq(T, z->pai->pai);
      }      
    }
  }
  T->raiz->cor = NEGRO;
}

static void rotacaoEsq(arn *T, no *x) {
  no *y;

  y = x->dir;

  x->dir = y->esq;
  if (y->esq != T->nulo)
    y->esq->pai = x;

  y->pai = x->pai;
  if (x->pai == T->nulo)
    T->raiz = y;
  else if (x == x->pai->esq)
    x->pai->esq = y;
  else
    x->pai->dir = y;

  y->esq = x;
  x->pai = y;
}

static void rotacaoDir(arn *T, no *x) {
  no *y;

  y = x->esq;

  x->esq = y->dir;
  if (y->dir != T->nulo)
    y->dir->pai = x;

  y->pai = x->pai;
  if (x->pai == T->nulo)
    T->raiz = y;
  else if (x == x->pai->dir)
    x->pai->dir = y;
  else
    x->pai->esq = y;

  y->dir = x;
  x->pai = y;

}


/* Busca e retorna o ponteiro para um nó x com chave k, caso exista. Retorna null caso tal nó não exista. */
void *buscar(void *T, int k){
  arn *Ti;
  no *x;

  Ti = (arn *) T;
  x = Ti->raiz;

  Ti->nulo->chave = k;

  while (x->chave != k) {
    if (k < x->chave)
      x = x->esq;
    else
      x = x->dir;
  }

  if (x == Ti->nulo)
    return NULL;
  return (void *)x;
}

/* Retorna a chave do nó x */
int chave(void *x){
  return ((no *)x)->chave;
}

static void transplantar(arn *T, no *u, no *v) {
  if (u->pai == T->nulo)
    T->raiz = v;
  else if (u == u->pai->esq)
    u->pai->esq = v;
  else
    u->pai->dir = v;
  v->pai = u->pai;
}

static no *min(arn *T, no *x) {
  while (x->esq != T->nulo)
    x = x->esq;
  return x;
}

/* Remove o nó x da arn T. */
void removerArn(void *T, void *z){
  arn *Ti;
  no *x, *y, *zi;
  char cor_orig_y;

  Ti = (arn *) T;
  zi = (no *)z;

  y = zi;
  cor_orig_y = y->cor;
  if (zi->esq == Ti->nulo) {
    x = zi->dir;
    transplantar(Ti, zi, x);
  }
  else if (zi->dir == Ti->nulo) {
    x = zi->esq;
    transplantar(Ti, zi, x);
  }
  else {
    y = min(Ti, zi->dir);
    x = y->dir;
    cor_orig_y = y->cor;
    transplantar(Ti, y, x);
    y->esq = zi->esq;
    zi->esq->pai = y;
    y->dir = zi->dir;
    zi->dir->pai = y;
    transplantar(Ti, z, y);
    y->cor = zi->cor;
  }
  if (cor_orig_y == NEGRO)
    consertarRemocaoRN(Ti, x);
}

static void consertarRemocaoRN(arn *T, no *x) {
  no *w;

  while (x != T->raiz && x->cor == NEGRO) {
    if (x == x->pai->esq) {
      w = x->pai->dir;
      if (w->cor == RUBRO) { // Caso 1
        w->cor = NEGRO;
        x->pai->cor = RUBRO;
        rotacaoEsq(T, x->pai);
        w = x->pai->dir;
      }
      if (w->esq->cor == NEGRO && w->dir->cor == NEGRO) { // Caso 2
        w->cor = RUBRO;
        x = x->pai;
      }
      else {
        if (w->dir->cor == NEGRO) { // Caso 3
          w->esq->cor = NEGRO;
          w->cor = RUBRO;
          rotacaoDir(T, x->pai);
          w = x->pai->dir;
        }
        // Caso 4
        w->cor = x->pai->cor;
        x->pai->cor = NEGRO;
        w->dir->cor = NEGRO;
        rotacaoEsq(T, x->pai);
        x = T->raiz;
      }
    }
    else { //4 casos simétricos
      w = x->pai->esq;
      if (w->cor == RUBRO) { // Caso 1
        w->cor = NEGRO;
        x->pai->cor = RUBRO;
        rotacaoDir(T, x->pai);
        w = x->pai->esq;
      }
      if (w->dir->cor == NEGRO && w->esq->cor == NEGRO) { // Caso 2
        w->cor = RUBRO;
        x = x->pai;
      }
      else {
        if (w->esq->cor == NEGRO) { // Caso 3
          w->dir->cor = NEGRO;
          w->cor = RUBRO;
          rotacaoEsq(T, x->pai);
          w = x->pai->esq;
        }
        // Caso 4
        w->cor = x->pai->cor;
        x->pai->cor = NEGRO;
        w->esq->cor = NEGRO;
        rotacaoDir(T, x->pai);
        x = T->raiz;
      }
    }
  }
  x->cor = NEGRO;
}