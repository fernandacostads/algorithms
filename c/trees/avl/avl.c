#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

/* Implementação de uma Árvore AVL. */

typedef struct no_s {
  int chave;
  int altura;
  struct no_s *pai;
  struct no_s *esq;
  struct no_s *dir;
} no;

typedef struct avl_s {
  no *raiz;
  no *nulo;
} avl;

/* Declaração das funções estáticas */

static void apagarRecursivo(avl *T, no *x);
static void imprimirRecursivo(avl *T, no *x);
static void imprimirRecursivoBonitinha(avl *T, no *x, int prof);
static int altura(no *x);
static int fatorBalanceamento(no *x);
static void atualizarAltura(no *x);
static void rotacaoEsq(avl *T, no *x);
static void rotacaoDir(avl *T, no *x);
static void rotacaoEsqDir(avl *T, no *x);
static void rotacaoDirEsq(avl *T, no *x);
static void consertarInclusaoAvl(avl *T, no *z);
static no *min(avl *T, no *x);
static void transplantar(avl *T, no *u, no *v);
static void consertarRemocaoAvl(avl *T, no *x);

/* Cria uma árvore AVL e retorna um ponteiro para ela. */
void *criarAvl(){
  avl *T;

  T = (avl *) malloc(sizeof(avl));
  T->nulo = (no *) malloc(sizeof(no));
  T->nulo->altura = -1;
  T->nulo->pai = NULL;
  T->nulo->esq = NULL;
  T->nulo->dir = NULL;
  T->raiz = T->nulo;
  return (void *)T;
}

/* Libera o espaço da árvore AVL T da memória junto com todos os seus nós. */
void apagarAvl(void *T){
  avl *Ti;

  Ti = (avl *) T;
  apagarRecursivo(Ti, Ti->raiz);
  free(Ti->nulo);
  free(Ti);
}

static void apagarRecursivo(avl *T, no *x) {
  if (x != T->nulo) {
    apagarRecursivo(T, x->esq);
    apagarRecursivo(T, x->dir);
    free(x);
  }
}

/* Imprime os elementos da árvore AVL T em ordem. */
void imprimirAvl(void *T){
  avl *Ti;

  Ti = (avl *) T;
  imprimirRecursivo(Ti, Ti->raiz);
  printf("\n");
}

static void imprimirRecursivo(avl *T, no *x) {
  if (x != T->nulo) {
    imprimirRecursivo(T, x->esq);
    printf("%d ", x->chave);
    imprimirRecursivo(T, x->dir);
  }
}

void imprimirAvlBonitinha(void *T) {
  avl *Ti;

  Ti = (avl *) T;
  imprimirRecursivoBonitinha(Ti, Ti->raiz, 0);
}

static void imprimirRecursivoBonitinha(avl *T, no *x, int prof) {
  if (x != T->nulo) {
    imprimirRecursivoBonitinha(T, x->dir, prof+1);
    for (int i = 0; i < prof; i++) {
      printf("  ");
    }
    printf("%d (h:%d)\n", x->chave, x->altura);
    imprimirRecursivoBonitinha(T, x->esq, prof+1);
  }
}

static int altura(no *x) {
  if (x == NULL) return -1;
  return x->altura;
}

static int fatorBalanceamento(no *x) {
  if (x == NULL) return 0;
  return altura(x->esq) - altura(x->dir);
}

static void atualizarAltura(no *x) {
  if (x == NULL) return;
  int altEsq = altura(x->esq);
  int altDir = altura(x->dir);
  x->altura = (altEsq > altDir ? altEsq : altDir) + 1;
}

static void rotacaoDir(avl *T, no *x) {
  no *y = x->esq;
  
  x->esq = y->dir;
  if (y->dir != T->nulo)
    y->dir->pai = x;
  
  y->pai = x->pai;
  if (x->pai == T->nulo)
    T->raiz = y;
  else if (x == x->pai->esq)
    x->pai->esq = y;
  else
    x->pai->dir = y;
  
  y->dir = x;
  x->pai = y;
  
  atualizarAltura(x);
  atualizarAltura(y);
}

static void rotacaoEsq(avl *T, no *x) {
  no *y = x->dir;
  
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
  
  atualizarAltura(x);
  atualizarAltura(y);
}

static void rotacaoEsqDir(avl *T, no *x) {
  rotacaoEsq(T, x->esq);
  rotacaoDir(T, x);
}

static void rotacaoDirEsq(avl *T, no *x) {
  rotacaoDir(T, x->dir);
  rotacaoEsq(T, x);
}

/* Inclui o inteiro k na árvore AVL T. */
void incluirAvl(void *T, int k){
  no *x, *y, *z;
  avl *Ti;

  Ti = (avl *) T;

  z = (no *) malloc(sizeof(no));
  z->chave = k;
  z->altura = 0;

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
  consertarInclusaoAvl(Ti, z);
}

static void consertarInclusaoAvl(avl *T, no *z) {
  no *x = z;
  
  while (x != T->nulo) {
    atualizarAltura(x);
    int fb = fatorBalanceamento(x);
    
    if (fb > 1) {
      if (fatorBalanceamento(x->esq) < 0)
        rotacaoEsqDir(T, x);
      else
        rotacaoDir(T, x);
    } else if (fb < -1) {
      if (fatorBalanceamento(x->dir) > 0)
        rotacaoDirEsq(T, x);
      else
        rotacaoEsq(T, x);
    }
    
    x = x->pai;
  }
}

/* Busca e retorna o ponteiro para um nó x com chave k, caso exista. */
void *buscarAvl(void *T, int k){
  no *x;
  avl *Ti;

  Ti = (avl *) T;
  x = Ti->raiz;

  while (x != Ti->nulo && k != x->chave) {
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
int chaveAvl(void *x){
  no *node;

  node = (no *) x;
  return node->chave;
}

static no *min(avl *T, no *x) {
  while (x->esq != T->nulo) {
    x = x->esq;
  }
  return x;
}

static void transplantar(avl *T, no *u, no *v) {
  if (u->pai == T->nulo)
    T->raiz = v;
  else if (u == u->pai->esq)
    u->pai->esq = v;
  else
    u->pai->dir = v;
  
  if (v != T->nulo)
    v->pai = u->pai;
}

/* Remove o nó x da árvore AVL T. */
void removerAvl(void *T, void *z){
  no *x, *y, *w;
  avl *Ti;

  Ti = (avl *) T;
  w = (no *) z;

  if (w == Ti->nulo) return;

  no *balancear_de = w->pai;

  if (w->esq == Ti->nulo) {
    transplantar(Ti, w, w->dir);
  } else if (w->dir == Ti->nulo) {
    transplantar(Ti, w, w->esq);
  } else {
    y = min(Ti, w->dir);
    if (y->pai != w) {
      balancear_de = y->pai;
      transplantar(Ti, y, y->dir);
      y->dir = w->dir;
      y->dir->pai = y;
    }
    transplantar(Ti, w, y);
    y->esq = w->esq;
    y->esq->pai = y;
  }

  free(w);
  
  /* Corrigir balanceamento a partir do ancestral afetado */
  if (balancear_de != Ti->nulo)
    consertarInclusaoAvl(Ti, balancear_de);
}
