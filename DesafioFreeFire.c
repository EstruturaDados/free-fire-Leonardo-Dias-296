#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

typedef enum {
    ORDENAR_NOME = 1,
    ORDENAR_TIPO,
    ORDENAR_PRIORIDADE
} CriterioOrdenacao;

Item mochilaVetor[MAX_ITENS];
int qtdItensVetor = 0;
No* mochilaLista = NULL;

int ordenadoPorNome = 0;
int contadorComparacoes = 0;

void cabecalho() {
    printf("=====================================================\n");
    printf("      PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
    printf("=====================================================\n");
    printf("Itens na Mochila: %d/%d\n", qtdItensVetor, MAX_ITENS);
    printf("Status da Ordenacao por Nome: %s\n", ordenadoPorNome ? "ORDENADO" : "NAO ORDENADO");
    printf("-----------------------------------------------------\n");
    printf("1. Adicionar Componente\n");
    printf("2. Descartar Componente\n");
    printf("3. Listar Componentes (Inventario)\n");
    printf("4. Organizar Mochila (Ordenar Componentes)\n");
    printf("5. Busca Binaria por Componente-Chave (por nome)\n");
    printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
    printf("-----------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void inserirItemVetor() {
    if (qtdItensVetor >= MAX_ITENS) {
        printf("\nMochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }
    Item novo;
    printf("Nome: ");
    scanf(" %[^\n]", novo.nome);
    printf("Tipo: ");
    scanf(" %[^\n]", novo.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    printf("Prioridade (1 a 5): ");
    scanf("%d", &novo.prioridade);
    mochilaVetor[qtdItensVetor++] = novo;
    ordenadoPorNome = 0;
    printf("\nComponente adicionado com sucesso!\n");
}

void removerItemVetor() {
    if (qtdItensVetor == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    char nome[30];
    printf("Nome do componente a descartar: ");
    scanf(" %[^\n]", nome);
    int i, encontrado = 0;
    for (i = 0; i < qtdItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nome) == 0) {
            encontrado = 1;
            for (int j = i; j < qtdItensVetor - 1; j++) {
                mochilaVetor[j] = mochilaVetor[j + 1];
            }
            qtdItensVetor--;
            printf("\nComponente removido com sucesso!\n");
            break;
        }
    }
    if (!encontrado) printf("\nComponente nao encontrado.\n");
}

void listarItens() {
    if (qtdItensVetor == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    printf("\n=== Componentes na Mochila ===\n");
    printf("%-20s %-15s %-10s %-10s\n", "Nome", "Tipo", "Qtd", "Prioridade");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < qtdItensVetor; i++) {
        printf("%-20s %-15s %-10d %-10d\n",
               mochilaVetor[i].nome,
               mochilaVetor[i].tipo,
               mochilaVetor[i].quantidade,
               mochilaVetor[i].prioridade);
    }
}

int compararItens(Item a, Item b, CriterioOrdenacao criterio) {
    contadorComparacoes++;
    if (criterio == ORDENAR_NOME) return strcmp(a.nome, b.nome);
    if (criterio == ORDENAR_TIPO) return strcmp(a.tipo, b.tipo);
    return a.prioridade - b.prioridade;
}

void insertionSort(Item vetor[], int n, CriterioOrdenacao criterio) {
    contadorComparacoes = 0;
    for (int i = 1; i < n; i++) {
        Item chave = vetor[i];
        int j = i - 1;
        while (j >= 0 && compararItens(vetor[j], chave, criterio) > 0) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = chave;
    }
    if (criterio == ORDENAR_NOME) ordenadoPorNome = 1;
    else ordenadoPorNome = 0;
}

void ordenarMochila() {
    if (qtdItensVetor == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    int opcao;
    printf("\nOrdenar por:\n1. Nome\n2. Tipo\n3. Prioridade\nEscolha: ");
    scanf("%d", &opcao);
    if (opcao < 1 || opcao > 3) {
        printf("\nOpcao invalida.\n");
        return;
    }
    CriterioOrdenacao criterio = opcao;
    clock_t inicio = clock();
    insertionSort(mochilaVetor, qtdItensVetor, criterio);
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nMochila organizada com sucesso!\n");
    printf("Comparacoes realizadas: %d\n", contadorComparacoes);
    printf("Tempo de execucao: %.5f s\n", tempo);
}

void buscaBinaria() {
    if (!ordenadoPorNome) {
        printf("\nA mochila precisa estar ordenada por NOME para usar busca binaria.\n");
        return;
    }
    if (qtdItensVetor == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    char nomeBusca[30];
    printf("Nome do componente para busca: ");
    scanf(" %[^\n]", nomeBusca);
    int inicio = 0, fim = qtdItensVetor - 1, meio, encontrado = 0;
    contadorComparacoes = 0;
    clock_t inicioTempo = clock();
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        contadorComparacoes++;
        int cmp = strcmp(mochilaVetor[meio].nome, nomeBusca);
        if (cmp == 0) {
            printf("\n=== Componente Encontrado ===\n");
            printf("%-20s %-15s %-10s %-10s\n", "Nome", "Tipo", "Qtd", "Prioridade");
            printf("-----------------------------------------------------\n");
            printf("%-20s %-15s %-10d %-10d\n",
                   mochilaVetor[meio].nome,
                   mochilaVetor[meio].tipo,
                   mochilaVetor[meio].quantidade,
                   mochilaVetor[meio].prioridade);
            encontrado = 1;
            break;
        } else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    clock_t fimTempo = clock();
    double tempo = (double)(fimTempo - inicioTempo) / CLOCKS_PER_SEC;
    if (!encontrado) printf("\nComponente nao encontrado.\n");
    printf("Comparacoes: %d | Tempo: %.5f s\n", contadorComparacoes, tempo);
}

int main() {
    int opcao;
    do {
        cabecalho();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: inserirItemVetor(); break;
            case 2: removerItemVetor(); break;
            case 3: listarItens(); break;
            case 4: ordenarMochila(); break;
            case 5: buscaBinaria(); break;
            case 0: printf("\nTorre de Fuga Ativada. Saindo...\n"); break;
            default: printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);
    return 0;
}
