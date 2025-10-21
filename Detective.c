#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para os cômodos da mansão (árvore binária)
typedef struct Sala {
    char nome[50];
    char pista[100]; // pode estar vazia se não houver pista
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Estrutura para as pistas coletadas (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Função para criar uma nova sala
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    if (pista != NULL)
        strcpy(novaSala->pista, pista);
    else
        novaSala->pista[0] = '\0';
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para criar um novo nó de pista
PistaNode* criarPista(const char* pista) {
    PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Inserir pista na BST
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) return criarPista(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    // se já existe, não insere duplicata
    return raiz;
}

// Percurso em ordem para exibir pistas
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Função para explorar a mansão e coletar pistas
void explorarSalasComPistas(Sala* salaAtual, PistaNode** pistas) {
    if (salaAtual == NULL) return;

    printf("\nVocê está na sala: %s\n", salaAtual->nome);
    if (strlen(salaAtual->pista) > 0) {
        printf("Você encontrou uma pista: %s\n", salaAtual->pista);
        *pistas = inserirPista(*pistas, salaAtual->pista);
    }

    char opcao;
    printf("Escolha o caminho: esquerda (e), direita (d), sair (s): ");
    scanf(" %c", &opcao);

    if (opcao == 'e')
        explorarSalasComPistas(salaAtual->esquerda, pistas);
    else if (opcao == 'd')
        explorarSalasComPistas(salaAtual->direita, pistas);
    else if (opcao == 's')
        return;
    else {
        printf("Opção inválida! Tente novamente.\n");
        explorarSalasComPistas(salaAtual, pistas);
    }
}

int main() {
    // Criando o mapa da mansão
    Sala* hall = criarSala("Hall de Entrada", "Chave perdida");
    hall->esquerda = criarSala("Sala de Estar", "Livro com página faltando");
    hall->direita = criarSala("Cozinha", "");
    hall->esquerda->esquerda = criarSala("Biblioteca", "Gaveta perdida");
    hall->esquerda->direita = criarSala("Quarto", "Lençol manchado");
    hall->direita->esquerda = criarSala("Jardim", "");
    hall->direita->direita = criarSala("Despensa", "Pegadas de lama");

    PistaNode* minhasPistas = NULL;

    printf("=== Bem-vindo ao Detective Quest ===\n");
    explorarSalasComPistas(hall, &minhasPistas);

    printf("\n=== Pistas coletadas em ordem alfabética ===\n");
    exibirPistas(minhasPistas);

    return 0;
}
