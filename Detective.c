#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da sala
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Estrutura da árvore de pistas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Estrutura da tabela hash para pistas → suspeitos
#define TAM_HASH 10
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* prox;
} HashNode;

HashNode* tabelaHash[TAM_HASH];

// Lista de suspeitos
char* suspeitos[4] = {"Murilo", "Ana", "Luca", "Tony"};

// Função hash simples
int hash(char* chave) {
    int soma = 0;
    for(int i = 0; chave[i]; i++) soma += chave[i];
    return soma % TAM_HASH;
}

// Inserir na tabela hash
void inserirNaHash(char* pista, char* suspeito) {
    int idx = hash(pista);
    HashNode* novo = malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
}

// Encontrar suspeito pela pista
char* encontrarSuspeito(char* pista) {
    int idx = hash(pista);
    HashNode* atual = tabelaHash[idx];
    while(atual) {
        if(strcmp(atual->pista, pista) == 0) return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Criar nó de pista na BST
PistaNode* criarPistaNode(char* pista) {
    PistaNode* novo = malloc(sizeof(PistaNode));
    strcpy(novo->pista, pista);
    novo->esquerda = novo->direita = NULL;
    return novo;
}

// Inserir pista na BST
PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if(!raiz) return criarPistaNode(pista);
    if(strcmp(pista, raiz->pista) < 0) raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if(strcmp(pista, raiz->pista) > 0) raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// Mostrar pistas coletadas em ordem
void exibirPistas(PistaNode* raiz) {
    if(!raiz) return;
    exibirPistas(raiz->esquerda);
    printf("- %s (Suspeito: %s)\n", raiz->pista, encontrarSuspeito(raiz->pista));
    exibirPistas(raiz->direita);
}

// Criar sala
Sala* criarSala(char* nome, char* pista) {
    Sala* s = malloc(sizeof(Sala));
    strcpy(s->nome, nome);
    strcpy(s->pista, pista);
    s->esquerda = s->direita = NULL;
    return s;
}

// Explorar salas com coleta de pistas
void explorarSalas(Sala* sala, PistaNode** bst) {
    if(!sala) return;
    char escolha;
    printf("\nVocê está na %s.\n", sala->nome);
    if(strlen(sala->pista) > 0) {
        printf("Você encontrou a pista: %s\n", sala->pista);
        *bst = inserirPista(*bst, sala->pista);
    }

    while(1) {
        printf("\nOpções: (e) Esquerda, (d) Direita, (S) Suspeitos, (X) Sair\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if(escolha == 'S' || escolha == 's') {
            printf("\nSuspeitos disponíveis:\n");
            for(int i = 0; i < 4; i++) {
                printf("%s\n", suspeitos[i]);
            }
        } else if(escolha == 'X' || escolha == 'x') {
            printf("\nVocê decidiu sair da mansão.\n");
            return;
        } else if(escolha == 'e' || escolha == 'E') {
            if(sala->esquerda) {
                printf("Indo para a esquerda (%s)...\n", sala->esquerda->nome);
                explorarSalas(sala->esquerda, bst);
                return;
            } else {
                printf("Não há sala à esquerda.\n");
            }
        } else if(escolha == 'd' || escolha == 'D') {
            if(sala->direita) {
                printf("Indo para a direita (%s)...\n", sala->direita->nome);
                explorarSalas(sala->direita, bst);
                return;
            } else {
                printf("Não há sala à direita.\n");
            }
        } else {
            printf("Opção inválida.\n");
        }
    }
}

// Verificar suspeito final
void verificarSuspeitoFinal(PistaNode* bst) {
    char acusacao[50];
    int contagem;
    printf("\nLista de suspeitos e pistas associadas:\n");
    for(int i = 0; i < 4; i++) {
        printf("%s\n", suspeitos[i]);
    }

    printf("\nQuem você acusa? ");
    scanf("%s", acusacao);

    // Contar pistas que apontam para o suspeito
    contagem = 0;
    PistaNode* stack[100]; // simples pilha para percurso in-order
    int topo = -1;
    PistaNode* atual = bst;
    while(atual || topo != -1) {
        while(atual) {
            stack[++topo] = atual;
            atual = atual->esquerda;
        }
        atual = stack[topo--];
        if(strcmp(encontrarSuspeito(atual->pista), acusacao) == 0) contagem++;
        atual = atual->direita;
    }

    if(contagem >= 2) printf("\nParabéns! Havia pistas suficientes para acusar %s.\n", acusacao);
    else printf("\nInfelizmente, não há pistas suficientes para acusar %s.\n", acusacao);
}

// Função principal
int main() {
    // Inicializar tabela hash
    for(int i=0;i<TAM_HASH;i++) tabelaHash[i]=NULL;

    // Criar mansão e salas
    Sala* hall = criarSala("Hall de Entrada", "");
    Sala* salaEstar = criarSala("Sala de Estar", "Pegadas na lama");
    Sala* biblioteca = criarSala("Biblioteca", "Livro com páginas faltando");
    Sala* escritorio = criarSala("Escritório", "Carta suspeita");
    Sala* cozinha = criarSala("Cozinha", "Chave perdida");
    Sala* despensa = criarSala("Despensa", "Pano sujo");
    Sala* lavanderia = criarSala("Lavanderia", "Produto químico estranho");
    Sala* salaJogos = criarSala("Sala de Jogos", "Baralho rasgado");

    // Conectar salas
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;
    cozinha->esquerda = despensa;
    cozinha->direita = lavanderia;
    lavanderia->direita = salaJogos; // novo caminho

    // Associar pistas a suspeitos
    inserirNaHash("Pegadas na lama", "Murilo");
    inserirNaHash("Chave perdida", "Ana");
    inserirNaHash("Livro com páginas faltando", "Luca");
    inserirNaHash("Pano sujo", "Tony");
    inserirNaHash("Carta suspeita", "Ana");
    inserirNaHash("Produto químico estranho", "Murilo");
    inserirNaHash("Baralho rasgado", "Tony");

    // BST de pistas coletadas
    PistaNode* bstPistas = NULL;

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão, colete pistas e descubra o culpado.\n");

    explorarSalas(hall, &bstPistas);
    printf("\nPistas coletadas:\n");
    exibirPistas(bstPistas);

    verificarSuspeitoFinal(bstPistas);

    return 0;
}

