#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define ORANGE "\x1b[38;5;208m"  
#define RESET "\x1b[0m"

#define MAX_JOGADORES 4

typedef enum {
    COPAS, OUROS, ESPADAS, PAUS
} Naipe;

typedef struct {
    Naipe naipe;
    int valor;
} Carta;

typedef struct NoCarta {
    Carta carta;
    struct NoCarta *prox;
} NoCarta;

typedef struct {
    char nome[50];
    NoCarta *mao;
    int pontuacao;
} Jogador;

const char *nome_naipe(Naipe n) {
    switch(n) {
        case COPAS: return "Copas";
        case OUROS: return "Ouros";
        case ESPADAS: return "Espadas";
        case PAUS: return "Paus";
        default: return "";
    }
}

const char *nome_valor(int valor) {
    switch(valor) {
        case 1: return "As";
        case 11: return "Valete";
        case 12: return "Dama";
        case 13: return "Rei";
        default: {
            static char str[3];
            sprintf(str, "%d", valor);
            return str;
        }
    }
}


void adicionar_carta(NoCarta **mao, Carta c) {
    NoCarta *nova = malloc(sizeof(NoCarta));
    nova->carta = c;
    nova->prox = *mao;
    *mao = nova;
}

int calcular_pontuacao(NoCarta *mao) {
    int total = 0, ases = 0;
    while (mao) {
        int val = mao->carta.valor;
        if (val == 1) {
            total += 11;
            ases++;
        } else {
            total += val;
        }
        mao = mao->prox;
    }
    while (total > 21 && ases > 0) {
        total -= 10;
        ases--;
    }
    return total;
}

void mostrar_mao_horizontal(const char *nome, NoCarta *mao) {
    printf("\n%s:\n", nome);
    NoCarta *temp = mao;
    while (temp) {
        printf("[%s de %s] ", nome_valor(temp->carta.valor), nome_naipe(temp->carta.naipe));
        temp = temp->prox;
    }
    printf("=> Total: %d\n", calcular_pontuacao(mao));
}

void liberar_mao(NoCarta *mao) {
    while (mao) {
        NoCarta *tmp = mao;
        mao = mao->prox;
        free(tmp);
    }
}

NoCarta *criar_baralho() {
    NoCarta *baralho = NULL;
    for (int naipe = 0; naipe < 4; naipe++) {
        for (int valor = 1; valor <= 13; valor++) {
            int valor_real = valor > 10 ? 10 : valor;
            Carta c = {naipe, valor_real};
            adicionar_carta(&baralho, c);
        }
    }
    return baralho;
}

void Embaralhar_Cartas_animacao() {
    printf("\nEmbaralhando Cartas");
    for (int i = 0; i < 3; i++) {
        fflush(stdout);
        sleep(1);
        printf(".");
    }
    printf("\n");
}

void embaralhar(NoCarta **baralho) {
    Embaralhar_Cartas_animacao();
    NoCarta *array[52];
    int i = 0;
    while (*baralho && i < 52) {
        array[i++] = *baralho;
        *baralho = (*baralho)->prox;
    }
    for (int j = i - 1; j > 0; j--) {
        int r = rand() % (j + 1);
        NoCarta *tmp = array[j];
        array[j] = array[r];
        array[r] = tmp;
    }
    for (int j = 0; j < i - 1; j++) array[j]->prox = array[j + 1];
    array[i - 1]->prox = NULL;
    *baralho = array[0];
}

void mover_para_descarte(NoCarta **mao, NoCarta **descarte) {
    if (!*mao) return;
    NoCarta *atual = *mao;
    while (atual->prox) atual = atual->prox;
    atual->prox = *descarte;
    *descarte = *mao;
    *mao = NULL;
}

Carta comprar_carta(NoCarta **baralho, NoCarta **descarte) {
    if (!*baralho) {
        *baralho = *descarte;
        *descarte = NULL;
        embaralhar(baralho);
    }
    if (!*baralho) {
        printf(RED "Baralho vazio!\n" RESET);
        return (Carta){0};
    }
    Carta c = (*baralho)->carta;
    NoCarta *tmp = *baralho;
    *baralho = (*baralho)->prox;
    free(tmp);
    sleep(1);
    return c;
}

void mostrar_placar() {
    FILE *f = fopen("placar.txt", "r");
    if (f) {
        char linha[256];
        printf("\n%s==== PLACAR ====%s\n", YELLOW, RESET);
        while (fgets(linha, sizeof(linha), f)) {
            if (strstr(linha, "Rodada")) printf("\n%s%s%s", ORANGE, linha, RESET);
            else if (strstr(linha, "Venceu")) printf("%s%s%s", GREEN, linha, RESET);
            else if (strstr(linha, "Perdeu")) printf("%s%s%s", RED, linha, RESET);
            else if (strstr(linha, "Empatou")) printf("%s%s%s", YELLOW, linha, RESET);
            else printf("%s", linha);
        }
        fclose(f);
    } else {
        printf(YELLOW "Nenhum placar salvo ainda.\n" RESET);
    }
}

int ler_numero_rodada() {
    FILE *f = fopen("placar.txt", "r");
    if (!f) return 1;
    int maior = 0, rodada;
    char linha[256];
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "====== Rodada %d ======", &rodada) == 1 && rodada > maior) {
            maior = rodada;
        }
    }
    fclose(f);
    return maior + 1;
}

void salvar_cabecalho_rodada(FILE *f, int rodada) {
    fprintf(f, "====== Rodada %d ======\n", rodada);
}

void salvar_resultado(FILE *f, int rodada, const char *nome, const char *resultado) {
    fprintf(f, "%s: %s\n", nome, resultado);
}

void Limpar_Placar() {
    printf("Limpando Placar");
    for (int i = 0; i < 3; i++) { fflush(stdout); sleep(1); printf("."); }
    printf("\n");
}

void Sair() {
    printf("Saindo");
    for (int i = 0; i < 3; i++) { fflush(stdout); sleep(1); printf("."); }
    printf("\n");
}

void jogar() {
    srand(time(NULL));
    int n;
    printf("Quantos jogadores (1 a %d)? ", MAX_JOGADORES);
    scanf("%d", &n); while (getchar() != '\n');
    if (n < 1 || n > MAX_JOGADORES) { printf(RED "Numero invalido.\n" RESET); return; }

    Jogador jogadores[MAX_JOGADORES];
    for (int i = 0; i < n; i++) {
        printf("Nome do jogador %d: ", i + 1);
        scanf("%s", jogadores[i].nome); while (getchar() != '\n');
        jogadores[i].mao = NULL;
        jogadores[i].pontuacao = 0;
    }

    Jogador dealer = {"Dealer", NULL, 0};
    NoCarta *baralho = criar_baralho();
    NoCarta *descarte = NULL;
    embaralhar(&baralho);

    int rodada = ler_numero_rodada();
    printf("\n==== RODADA %d ====\n", rodada);
    FILE *f = fopen("placar.txt", "a");
    if (!f) { printf(RED "Erro ao abrir placar.txt\n" RESET); return; }
    salvar_cabecalho_rodada(f, rodada);

    for (int i = 0; i < n; i++) {
        adicionar_carta(&jogadores[i].mao, comprar_carta(&baralho, &descarte));
        adicionar_carta(&jogadores[i].mao, comprar_carta(&baralho, &descarte));
        printf("\n==== Vez de %s ====\n", jogadores[i].nome);
        mostrar_mao_horizontal("Sua mao", jogadores[i].mao);

        int opcao = 0;
        do {
            jogadores[i].pontuacao = calcular_pontuacao(jogadores[i].mao);
            if (jogadores[i].pontuacao > 21) { printf(RED "\nEstourou!\n" RESET); break; }
            printf("1. Comprar carta\n2. Parar\n> ");
            scanf("%d", &opcao); while (getchar() != '\n');
            if (opcao == 1) {
                adicionar_carta(&jogadores[i].mao, comprar_carta(&baralho, &descarte));
                printf(GREEN "\n+ 1 carta\n" RESET);
                mostrar_mao_horizontal("Sua mao", jogadores[i].mao);
            }
        } while (opcao != 2);
    }

    printf("\n==== Vez de Dealer ====\n");
    Embaralhar_Cartas_animacao();
    adicionar_carta(&dealer.mao, comprar_carta(&baralho, &descarte));
    adicionar_carta(&dealer.mao, comprar_carta(&baralho, &descarte));
    while ((dealer.pontuacao = calcular_pontuacao(dealer.mao)) < 17) {
        mostrar_mao_horizontal("Dealer", dealer.mao);
        adicionar_carta(&dealer.mao, comprar_carta(&baralho, &descarte));
        printf(GREEN "\n+ 1 Carta\n" RESET);
    }
    mostrar_mao_horizontal("Dealer", dealer.mao);

    printf("\n==== RESULTADOS ====\n");
    for (int i = 0; i < n; i++) {
        jogadores[i].pontuacao = calcular_pontuacao(jogadores[i].mao);
        if (jogadores[i].pontuacao > 21) {
            printf(RED "%s estourou! Dealer venceu.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Perdeu (Estourou)");
        } else if (dealer.pontuacao > 21 || jogadores[i].pontuacao > dealer.pontuacao) {
            printf(GREEN "%s venceu o dealer!\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Venceu");
        } else if (jogadores[i].pontuacao == dealer.pontuacao) {
            printf(YELLOW "%s empatou com o dealer.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Empatou");
        } else {
            printf(RED "Dealer venceu %s.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Perdeu");
        }
        mover_para_descarte(&jogadores[i].mao, &descarte);
    }

    mover_para_descarte(&dealer.mao, &descarte);
    liberar_mao(baralho);
    liberar_mao(descarte);
    fclose(f);
}

int main() {
    int escolha;
    char buffer[10];
    do {
        printf("\n==== BlackJack 21 ====\n1. Jogar\n2. Ver placar\n3. Sair\n> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        if (sscanf(buffer, "%d", &escolha) != 1) {
            printf(RED "\nEntrada invalida! Digite um numero.\n" RESET);
            continue;
        }
        switch (escolha) {
            case 1: jogar(); break;
            case 2: mostrar_placar(); break;
            case 3:
                if (remove("placar.txt") == 0) Limpar_Placar();
                Sair();
                break;
            default:
                printf(RED "Opcao invalida.\n" RESET);
        }
    } while (escolha != 3);
    return 0;
}
