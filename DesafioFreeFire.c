#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10  // Capacidade máxima da mochila

// ========================= STRUCT ITEM =========================
// Estrutura que representa cada item do inventário do jogador
typedef struct {
    char nome[30];      // Nome do item
    char tipo[20];      // Tipo (arma, munição, cura, ferramenta, etc.)
    int quantidade;     // Quantidade do item
} Item;

// ========================= FUNÇÕES =========================
void inserirItem(Item mochila[], int *quantidade);
void removerItem(Item mochila[], int *quantidade);
void listarItens(Item mochila[], int quantidade);
void buscarItem(Item mochila[], int quantidade);

// ========================= FUNÇÃO PRINCIPAL =========================
int main() {
    Item mochila[MAX_ITENS];
    int quantidade = 0;
    int opcao;

    do {
        printf("\n===== MOCHILA DE LOOT INICIAL =====\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer

        switch (opcao) {
            case 1:
                inserirItem(mochila, &quantidade);
                break;
            case 2:
                removerItem(mochila, &quantidade);
                break;
            case 3:
                listarItens(mochila, quantidade);
                break;
            case 4:
                buscarItem(mochila, quantidade);
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

// ========================= INSERIR ITEM =========================
void inserirItem(Item mochila[], int *quantidade) {
    if (*quantidade >= MAX_ITENS) {
        printf("A mochila esta cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("\n=== CADASTRO DE ITEM ===\n");
    printf("Nome do item: ");
    fgets(novo.nome, 30, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // remove \n

    printf("Tipo do item (arma, municao, cura, ferramenta...): ");
    fgets(novo.tipo, 20, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[*quantidade] = novo;
    (*quantidade)++;

    printf("Item \"%s\" adicionado com sucesso!\n", novo.nome);
}

// ========================= REMOVER ITEM =========================
void removerItem(Item mochila[], int *quantidade) {
    if (*quantidade == 0) {
        printf("A mochila esta vazia! Nenhum item para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item que deseja remover: ");
    fgets(nomeRemover, 30, stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    int encontrado = -1;
    for (int i = 0; i < *quantidade; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("Item \"%s\" nao encontrado na mochila.\n", nomeRemover);
        return;
    }

    for (int i = encontrado; i < *quantidade - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    (*quantidade)--;
    printf("Item \"%s\" removido com sucesso!\n", nomeRemover);
}

// ========================= LISTAR ITENS =========================
void listarItens(Item mochila[], int quantidade) {
    if (quantidade == 0) {
        printf("\nA mochila esta vazia!\n");
        return;
    }

    printf("\n=== ITENS NA MOCHILA ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// ========================= BUSCAR ITEM =========================
void buscarItem(Item mochila[], int quantidade) {
    if (quantidade == 0) {
        printf("A mochila esta vazia! Nao ha itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < quantidade; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            return;
        }
    }

    printf("Item \"%s\" nao encontrado na mochila.\n", nomeBusca);
}
