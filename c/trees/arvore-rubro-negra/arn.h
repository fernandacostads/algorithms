#ifndef RUBRONEGRA
#define RUBRONEGRA

/* Cria uma arn e retorna um ponteiro para ela. */
void *criarArn();

/* Libera o espaço da arn T da memória junto com todos os seus nós. */
void apagarArn(void *T);

/* Imprime os elementos da arn T em ordem. */
void imprimirArn(void *T);

/* Imprime os elementos da arn T em ordem bonitinha. */
void imprimirArnBonitinha(void *T);

/* Inclui o inteiro k na arn T. */
void incluirArn(void *T, int k);

/* Busca e retorna o ponteiro para um nó x com chave k, caso exista. Retorna null caso tal nó não exista. */
void *buscar(void *T, int k);

/* Retorna a chave do nó x */
int chave(void *x);

/* Remove o nó x da arn T. */
void removerArn(void *T, void *z);

#endif