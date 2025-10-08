#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

// ===================== STRUCTS =====================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No *proximo;
} No;

// ===================== DECLARAÇÕES =====================
void inserirItem(Item mochila[], int *qtd, No **inicio);
void removerItem(Item mochila[], int *qtd, No **inicio);
void listarItens(Item mochila[], int qtd, No *inicio);
void buscarItem(Item mochila[], int qtd, No *inicio);

void ordenarVetor(Item mochila[], int qtd);
int buscarSequencialVetor(Item mochila[], int qtd, const char *nome, int *comparacoes);
int buscarBinariaVetor(Item mochila[], int qtd, const char *nome, int *comparacoes);
int buscarSequencialLista(No *inicio, const char *nome, int *comparacoes);

// ===================== MAIN =====================
int main() {
    Item mochilaVetor[MAX_ITENS];
    int qtdVetor = 0;
    No *mochilaLista = NULL;
    int opcao;

    do {
        printf("\n===== SISTEMA DE MOCHILA DE LOOT =====\n");
        printf("=== Mochila ===\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                inserirItem(mochilaVetor, &qtdVetor, &mochilaLista);
                break;
            case 2:
                removerItem(mochilaVetor, &qtdVetor, &mochilaLista);
                break;
            case 3:
                listarItens(mochilaVetor, qtdVetor, mochilaLista);
                break;
            case 4:
                buscarItem(mochilaVetor, qtdVetor, mochilaLista);
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// ===================== INSERIR =====================
void inserirItem(Item mochila[], int *qtd, No **inicio) {
    if (*qtd >= MAX_ITENS) {
        printf("⚠️  Mochila cheia!\n");
        return;
    }

    Item novo;
    printf("Nome: ");
    fgets(novo.nome, 30, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';
    printf("Tipo: ");
    fgets(novo.tipo, 20, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    mochila[*qtd] = novo;
    (*qtd)++;

    // Insere também na lista encadeada
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->dados = novo;
    novoNo->proximo = *inicio;
    *inicio = novoNo;

    printf("✅ Item inserido com sucesso!\n");
}

// ===================== REMOVER =====================
void removerItem(Item mochila[], int *qtd, No **inicio) {
    if (*qtd == 0 && *inicio == NULL) {
        printf("⚠️  Mochila vazia!\n");
        return;
    }

    char nome[30];
    printf("Nome do item a remover: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    // Remover do vetor
    int encontrado = 0;
    for (int i = 0; i < *qtd; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < *qtd - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*qtd)--;
            encontrado = 1;
            break;
        }
    }

    // Remover da lista encadeada
    No *atual = *inicio, *anterior = NULL;
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual != NULL) {
        if (anterior == NULL)
            *inicio = atual->proximo;
        else
            anterior->proximo = atual->proximo;
        free(atual);
        encontrado = 1;
    }

    if (encontrado)
        printf("🗑️  Item removido com sucesso!\n");
    else
        printf("❌ Item nao encontrado!\n");
}

// ===================== LISTAR =====================
void listarItens(Item mochila[], int qtd, No *inicio) {
    if (qtd == 0 && inicio == NULL) {
        printf("⚠️  A mochila esta vazia!\n");
        return;
    }

    printf("\n=== Mochila (Vetor e Lista) ===\n");
    printf("------------------------------------------------------------\n");
    printf("| %-3s | %-20s | %-10s | %-5s |\n", "Nº", "Nome", "Tipo", "Qtd");
    printf("------------------------------------------------------------\n");

    int index = 1;
    No *temp = inicio;
    while (temp != NULL) {
        printf("| %-3d | %-20s | %-10s | %-5d |\n",
               index++, temp->dados.nome, temp->dados.tipo, temp->dados.quantidade);
        temp = temp->proximo;
    }

    printf("------------------------------------------------------------\n");
}

// ===================== BUSCAR =====================
void buscarItem(Item mochila[], int qtd, No *inicio) {
    if (qtd == 0 && inicio == NULL) {
        printf("⚠️  Mochila vazia!\n");
        return;
    }

    char nome[30];
    printf("Nome do item para buscar: ");
    fgets(nome, 30, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int compSeqVetor = 0, compBinVetor = 0, compSeqLista = 0;
    int posSeq = buscarSequencialVetor(mochila, qtd, nome, &compSeqVetor);

    ordenarVetor(mochila, qtd);
    int posBin = buscarBinariaVetor(mochila, qtd, nome, &compBinVetor);
    int posLista = buscarSequencialLista(inicio, nome, &compSeqLista);

    printf("\n=== Itens encontrados ===\n");
    printf("------------------------------------------------------------\n");
    printf("| %-3s | %-20s | %-10s | %-5s |\n", "Nº", "Nome", "Tipo", "Qtd");
    printf("------------------------------------------------------------\n");

    if (posSeq != -1) {
        printf("| %-3d | %-20s | %-10s | %-5d |\n",
               posSeq + 1, mochila[posSeq].nome, mochila[posSeq].tipo, mochila[posSeq].quantidade);
    } else if (posLista != -1) {
        No *temp = inicio;
        int i = 0;
        while (temp != NULL) {
            if (i == posLista) {
                printf("| %-3d | %-20s | %-10s | %-5d |\n",
                       i + 1, temp->dados.nome, temp->dados.tipo, temp->dados.quantidade);
                break;
            }
            temp = temp->proximo;
            i++;
        }
    } else {
        printf("| %-46s |\n", "Item nao encontrado!");
    }
    printf("------------------------------------------------------------\n");

    printf("Comparacoes (Sequencial Vetor): %d\n", compSeqVetor);
    printf("Comparacoes (Binaria Vetor): %d\n", compBinVetor);
    printf("Comparacoes (Sequencial Lista): %d\n", compSeqLista);
}

// ===================== BUSCAS =====================
int buscarSequencialVetor(Item mochila[], int qtd, const char *nome, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < qtd; i++) {
        (*comparacoes)++;
        if (strcmp(mochila[i].nome, nome) == 0)
            return i;
    }
    return -1;
}

int buscarBinariaVetor(Item mochila[], int qtd, const char *nome, int *comparacoes) {
    int ini = 0, fim = qtd - 1, meio;
    *comparacoes = 0;
    while (ini <= fim) {
        meio = (ini + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(mochila[meio].nome, nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) ini = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

int buscarSequencialLista(No *inicio, const char *nome, int *comparacoes) {
    *comparacoes = 0;
    int pos = 0;
    No *temp = inicio;
    while (temp != NULL) {
        (*comparacoes)++;
        if (strcmp(temp->dados.nome, nome) == 0)
            return pos;
        temp = temp->proximo;
        pos++;
    }
    return -1;
}

// ===================== ORDENAR =====================
void ordenarVetor(Item mochila[], int qtd) {
    Item temp;
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = 0; j < qtd - i - 1; j++) {
            if (strcmp(mochila[j].nome, mochila[j + 1].nome) > 0) {
                temp = mochila[j];
                mochila[j] = mochila[j + 1];
                mochila[j + 1] = temp;
            }
        }
    }
}
