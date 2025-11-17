#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTDE_TERRITORIOS 5

typedef struct {
    char nome[50];
    char cor[20];
    int tropas;
} Territorio;


/* ---------- PROTÓTIPOS ---------- */
void cadastrarTerritorios(Territorio *vet);
void exibirMapa(const Territorio *vet);
void atacar(Territorio *vet);
int simularBatalha();
int verificarMissoes(const Territorio *vet);


/* ---------- FUNÇÃO PRINCIPAL ---------- */
int main() {
    srand(time(NULL));

    Territorio *mapa = (Territorio*) calloc(QTDE_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    printf("=== SISTEMA DE TERRITORIOS ===\n\n");

    cadastrarTerritorios(mapa);

    int opcao;

    do {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missoes\n");
        printf("3 - Exibir Mapa\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                atacar(mapa);
                break;
            case 2:
                if (verificarMissoes(mapa)) {
                    printf("\n*** MISSAO COMPLETA! ***\n");
                } else {
                    printf("\nNenhuma missao concluida ainda.\n");
                }
                break;
            case 3:
                exibirMapa(mapa);
                break;
            case 0:
                printf("\nEncerrando...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    free(mapa);
    return 0;
}


/* ---------- CADASTRO INICIAL ---------- */
void cadastrarTerritorios(Territorio *vet) {
    printf("Cadastro dos Territorios:\n\n");

    for (int i = 0; i < QTDE_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);

        printf("Nome: ");
        scanf(" %49[^\n]", vet[i].nome);

        printf("Cor do Exercito: ");
        scanf(" %19[^\n]", vet[i].cor);

        printf("Numero de tropas: ");
        scanf("%d", &vet[i].tropas);

        printf("\n");
    }
}


/* ---------- IMPRESSÃO DO MAPA ---------- */
void exibirMapa(const Territorio *vet) {
    printf("\n===== ESTADO DO MAPA =====\n");

    for (int i = 0; i < QTDE_TERRITORIOS; i++) {
        printf("%d) %-12s | Exército: %-10s | Tropas: %d\n",
               i + 1, vet[i].nome, vet[i].cor, vet[i].tropas);
    }
}


/* ---------- LÓGICA DO ATAQUE ---------- */
void atacar(Territorio *vet) {
    int atq, def;

    exibirMapa(vet);

    printf("\nEscolha o territorio atacante (1 a 5): ");
    scanf("%d", &atq);
    printf("Escolha o territorio defensor (1 a 5): ");
    scanf("%d", &def);

    if (atq < 1 || atq > QTDE_TERRITORIOS ||
        def < 1 || def > QTDE_TERRITORIOS ||
        atq == def) {

        printf("Escolha invalida.\n");
        return;
    }

    atq--; def--;

    if (vet[atq].tropas <= 1) {
        printf("\nO territorio %s nao possui tropas suficientes para atacar.\n", vet[atq].nome);
        return;
    }

    printf("\nSimulando batalha...\n");
    int resultado = simularBatalha();

    if (resultado > 0) {
        vet[def].tropas--;

        printf("Ataque venceu! %s perdeu 1 tropa.\n", vet[def].nome);

        if (vet[def].tropas <= 0) {
            printf("Territorio conquistado!\n");
            strcpy(vet[def].cor, vet[atq].cor);
            vet[def].tropas = 1;
        }
    } 
    else if (resultado < 0) {
        vet[atq].tropas--;
        printf("Defensor resistiu! %s perdeu 1 tropa.\n", vet[atq].nome);
    } 
    else {
        printf("Empate! Nenhuma tropa perdida.\n");
    }
}


/* ---------- DADOS DE ATAQUE ---------- */
int simularBatalha() {
    int ataque = (rand() % 6) + 1;
    int defesa = (rand() % 6) + 1;

    printf("\nDado ataque:  %d\n", ataque);
    printf("Dado defesa:  %d\n\n", defesa);

    if (ataque > defesa) return 1;
    if (ataque < defesa) return -1;
    return 0;
}


/* ---------- MISSÕES ---------- */
int verificarMissoes(const Territorio *vet) {
    int conquistados = 0;
    int verdeDestruido = 1;

    for (int i = 0; i < QTDE_TERRITORIOS; i++) {
        if (strcmp(vet[i].cor, "Verde") == 0 && vet[i].tropas > 0) {
            verdeDestruido = 0;
        }

        if (vet[i].tropas > 0 && strcmp(vet[i].cor, "Verde") != 0) {
            conquistados++;
        }
    }

    if (verdeDestruido || conquistados >= 3)
        return 1;

    return 0;
}