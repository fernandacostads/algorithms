#ifndef AVL
#define AVL

/* Cria uma árvore AVL e retorna um ponteiro para ela. */
void *criarAvl();

/* Libera o espaço da árvore AVL T da memória junto com todos os seus nós. */
void apagarAvl(void *T);

/* Imprime os elementos da árvore AVL T em ordem. */
void imprimirAvl(void *T);

/* Imprime os elementos da árvore AVL T em ordem bonitinha. */
void imprimirAvlBonitinha(void *T);

/* Inclui o inteiro k na árvore AVL T. */
void incluirAvl(void *T, int k);

/* Busca e retorna o ponteiro para um nó x com chave k, caso exista. Retorna null caso tal nó não exista. */
void *buscarAvl(void *T, int k);

/* Retorna a chave do nó x */
int chaveAvl(void *x);

/* Remove o nó x da árvore AVL T. */
void removerAvl(void *T, void *z);

#endif
