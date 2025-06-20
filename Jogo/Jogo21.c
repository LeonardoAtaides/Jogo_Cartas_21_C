#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


// DEFINIR CORES DOS PRINTS

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define ORANGE "\x1b[38;5;208m"  
#define RESET "\x1b[0m"

#define MAX_JOGADORES 4

typedef enum {
    COPAS, OUROS, ESPADAS, PAUS
} Naipe;

typedef enum {
    AS = 1, DOIS, TRES, QUATRO, CINCO,
    SEIS, SETE, OITO, NOVE, DEZ,
    VALETE = 10, DAMA = 10, REI = 10
} ValorCarta;

// STRUCTS
typedef struct {
    Naipe naipe;
    ValorCarta valor;
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
        case 10: return "10";
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
    int total = 0;
    int ases = 0;
    while (mao) {
        int val = mao->carta.valor;
        if (val == 1) ases++;
        total += val;
        mao = mao->prox;
    }
    while (ases > 0 && total + 10 <= 21) {
        total += 10;
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
    int total = calcular_pontuacao(mao);
    printf("=> Total: %d\n", total);
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
            Carta c = {naipe, valor > 10 ? 10 : valor};
            adicionar_carta(&baralho, c);
        }
    }
    return baralho;
}

void animar_embaralhamento() {
    printf("Embaralhando");
    for (int i = 0; i < 3; i++) {
        fflush(stdout);
        sleep(1);
        printf(".");
    }
    printf("\n");
}

void embaralhar(NoCarta **baralho) {
    animar_embaralhamento();

    NoCarta *array[52];
    int i = 0;
    while (*baralho) {
        array[i++] = *baralho;
        *baralho = (*baralho)->prox;
    }
    for (int j = 0; j < 52; j++) {
        int r = rand() % 52;
        NoCarta *tmp = array[j];
        array[j] = array[r];
        array[r] = tmp;
    }
    for (int j = 0; j < 51; j++) array[j]->prox = array[j+1];
    array[51]->prox = NULL;
    *baralho = array[0];
}

Carta comprar_carta(NoCarta **baralho) {
    Carta c = (*baralho)->carta;
    NoCarta *tmp = *baralho;
    *baralho = (*baralho)->prox;
    free(tmp);
    return c;
}

int ler_numero_rodada() {
    FILE *f = fopen("placar.txt", "r");
    if (!f) return 1; 

    int maior = 0, rodada;
    char linha[256];
    while (fgets(linha, sizeof(linha), f)) {
        if (sscanf(linha, "==========Rodada %d===========", &rodada) == 1) {
            if (rodada > maior) maior = rodada;
        }
    }
    fclose(f);
    return maior + 1;
}

void salvar_cabecalho_rodada(FILE *f, int rodada) {
    fprintf(f, "==========Rodada %d===========\n", rodada);
}

void salvar_resultado(FILE *f, int rodada, const char *nome, const char *resultado) {
    fprintf(f, "%s: %s\n", nome, resultado);
}

void jogar() {
    srand(time(NULL));
    int n;
    printf("Quantos jogadores (1 a %d)? ", MAX_JOGADORES);
    scanf("%d", &n);
    if (n < 1 || n > MAX_JOGADORES) {
        printf("Numero invalido de jogadores.\n");
        return;
    }

    Jogador jogadores[MAX_JOGADORES];
    for (int i = 0; i < n; i++) {
        printf("Nome do jogador %d: ", i + 1);
        scanf("%s", jogadores[i].nome);
        jogadores[i].mao = NULL;
        jogadores[i].pontuacao = 0;
    }

    Jogador dealer = {"Dealer", NULL, 0};
    NoCarta *baralho = criar_baralho();
    embaralhar(&baralho);

    int rodada = ler_numero_rodada();
    printf("\n==== RODADA %d ====\n", rodada);

    FILE *f = fopen("placar.txt", "a");
    if (!f) {
        printf( RED "Erro ao abrir placar.txt\n" RESET);
        return;
    }
    salvar_cabecalho_rodada(f, rodada);

    for (int i = 0; i < n; i++) {
        adicionar_carta(&jogadores[i].mao, comprar_carta(&baralho));
        adicionar_carta(&jogadores[i].mao, comprar_carta(&baralho));

        int opcao;
        do {
            jogadores[i].pontuacao = calcular_pontuacao(jogadores[i].mao);
            printf("\n==== VEZ DE %s ====\n", jogadores[i].nome);
            mostrar_mao_horizontal("Sua mão", jogadores[i].mao);

            if (jogadores[i].pontuacao > 21) {
                printf( RED "Você estourou!\n" RESET);
                break;
            }

            printf("1. Comprar carta\n2. Parar\n> ");
            scanf("%d", &opcao);
            if (opcao == 1)
                adicionar_carta(&jogadores[i].mao, comprar_carta(&baralho));
        } while (opcao != 2);
    }

    printf("\n==== DEALER JOGANDO... ====\n");
    sleep(1);
    adicionar_carta(&dealer.mao, comprar_carta(&baralho));
    adicionar_carta(&dealer.mao, comprar_carta(&baralho));
    while ((dealer.pontuacao = calcular_pontuacao(dealer.mao)) < 17) {
        printf("Dealer compra uma carta...\n");
        sleep(1);
        adicionar_carta(&dealer.mao, comprar_carta(&baralho));
    }
    mostrar_mao_horizontal("Dealer", dealer.mao);

    // Resultados
    printf("\n==== RESULTADOS ====\n");
    for (int i = 0; i < n; i++) {
        if (jogadores[i].pontuacao > 21) {
            printf(ORANGE "%s estourou! Dealer venceu.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Perdeu (Estourou)");
        } else if (dealer.pontuacao > 21 || jogadores[i].pontuacao > dealer.pontuacao) {
            printf( GREEN "%s venceu o dealer!\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Venceu");
        } else if (jogadores[i].pontuacao == dealer.pontuacao) {
            printf(YELLOW "%s empatou com o dealer.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Empatou");
        } else {
            printf("Dealer venceu %s.\n", jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Perdeu");
        }
    }
    fprintf(f, "-----------------------------\n\n");
    fclose(f);

    for (int i = 0; i < n; i++) liberar_mao(jogadores[i].mao);
    liberar_mao(dealer.mao);
    liberar_mao(baralho);
}

void mostrar_placar() {
    FILE *f = fopen("placar.txt", "r");
    if (f) {
        char linha[256];
        printf("\n==== PLACAR ====\n");
        while (fgets(linha, sizeof(linha), f)) {
            printf("%s", linha);
        }
        fclose(f);
    } else {
        printf(YELLOW "Nenhum placar salvo ainda.\n" RESET);
    }
}

int main() {
    int escolha;
    do {
        printf("\n==== JOGO 21 ====\n");
        printf("1. Jogar\n2. Ver placar\n3. Sair\n> ");
        scanf("%d", &escolha);
        switch (escolha) {
            case 1: jogar(); break;
            case 2: mostrar_placar(); break;
            case 3: printf("Saindo...\n"); break;
            default: printf(RED "Opcao invalida.\n" RESET); break;
        }
    } while (escolha != 3);
    return 0;
}
