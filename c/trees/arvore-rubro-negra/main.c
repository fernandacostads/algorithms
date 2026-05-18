#include <stdio.h>
#include <stdlib.h>
#include "arn.h"

int main(void) {
  void *T, *x;

  T = criarArn();

  printf("Árvore criada\n");

  for(int i = 1; i <= 10; i++) {
    printf("Incluir %d\n", i);
    incluirArn(T, i);
    imprimirArnBonitinha(T);
  }
  printf()
  x = buscar(T, 8);
  removerArn(T, x);
  imprimirArnBonitinha(T);

  apagarArn(T);

  return 0;
}