#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int main(void) {
  void *T, *x;

  T = criarAvl();

  printf("Árvore AVL criada\n\n");

  for(int i = 1; i <= 10; i++) {
    printf("Incluir %d\n", i);
    incluirAvl(T, i);
    imprimirAvlBonitinha(T);
    printf("\n");
  }

  printf("Árvore em ordem: ");
  imprimirAvl(T);

  printf("\nBuscando 8...\n");
  x = buscarAvl(T, 8);
  if (x != NULL) {
    printf("Encontrado: %d\n", chaveAvl(x));
    printf("Removendo 8...\n");
    removerAvl(T, x);
    imprimirAvlBonitinha(T);
  }

  apagarAvl(T);

  return 0;
}
