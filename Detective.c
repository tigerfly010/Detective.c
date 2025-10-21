#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------- DEFINIÇÃO DA ESTRUTURA ----------------------

typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// ---------------------- FUNÇÕES ----------------------


Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}


void explorarSalas(Sala* atual) {
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        o
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🔚 Não há mais caminhos. Fim da exploração!\n");
            break;
        }

        printf("Escolha o caminho: (e) esquerda, (d) direita, (s) sair: ");
        char escolha;
        scanf(" %c", &escolha);

        if (escolha == 'e' || escolha == 'E') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("⚠️  Não há sala à esquerda!\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("⚠️  Não há sala à direita!\n");
            }
        } else if (escolha == 's' || escolha == 'S') {
            printf("👋 Saindo da exploração...\n");
            break;
        } else {
            printf("⚠️  Opção inválida! Tente novamente.\n");
        }
    }
}


void liberarMansao(Sala* raiz) {
    if (raiz != NULL) {
        liberarMansao(raiz->esquerda);
        liberarMansao(raiz->direita);
        free(raiz);
    }
}

// ---------------------- MAIN ----------------------

int main() {
    
    Sala* hall = criarSala("Hall de entrada");
    hall->esquerda = criarSala("Sala de estar");
    hall->direita = criarSala("Cozinha");
    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita = criarSala("Quarto");
    hall->direita->esquerda = criarSala("Jardim");
    hall->direita->direita = criarSala("Garagem");

    printf("🏠 Bem-vindo(a) à mansão do Detective Quest!\n");

    explorarSalas(hall);  

    liberarMansao(hall);  
    return 0;
}
