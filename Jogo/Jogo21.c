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

typedef enum {
    AS = 1, DOIS, TRES, QUATRO, CINCO,
    SEIS, SETE, OITO, NOVE, DEZ,
    VALETE = 10, DAMA = 10, REI = 10
} ValorCarta;

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

void Embaralhar_Cartas_animacao() {
    printf("\nEmbaralhando Cartas");
    for (int i = 0; i < 3; i++) {
        fflush(stdout);
        sleep(1);
        printf(".");
    }
    printf("\n");
}

void Sair() {
    printf("Saindo");
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
        if (sscanf(linha, "======  Rodada %d ======", &rodada) == 1) {
            if (rodada > maior) maior = rodada;
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

void jogar() {
    srand(time(NULL));
    int n;
    printf("Quantos jogadores (1 a %d)? ", MAX_JOGADORES);
    scanf("%d", &n);
    while (getchar() != '\n');

    if (n < 1 || n > MAX_JOGADORES) {
        printf(RED "Numero invalido de jogadores.\n" RESET);
        return;
    }

    Jogador jogadores[MAX_JOGADORES];
    for (int i = 0; i < n; i++) {
        printf("Nome do jogador %d: ", i + 1);
        scanf("%s", jogadores[i].nome);
        while (getchar() != '\n');
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
            printf("\n==== Vez de %s ====\n", jogadores[i].nome);
            mostrar_mao_horizontal("Sua mao", jogadores[i].mao);

            if (jogadores[i].pontuacao > 21) {
                printf( RED "\nVoce estourou!\n" RESET);
                break;
            }

            printf("1. Comprar carta\n2. Parar\n> ");
            scanf("%d", &opcao);
            while (getchar() != '\n');
            if (opcao == 1) {
                adicionar_carta(&jogadores[i].mao, comprar_carta(&baralho));
                printf(GREEN"\nComprou uma carta\n" RESET);
            }
        } while (opcao != 2);
    }

    printf("\n==== Vez de Dealer ====\n");
    Embaralhar_Cartas_animacao();
    sleep(1);
    adicionar_carta(&dealer.mao, comprar_carta(&baralho));
    adicionar_carta(&dealer.mao, comprar_carta(&baralho));
    int opcao;
    do {
        dealer.pontuacao = calcular_pontuacao(dealer.mao);
        mostrar_mao_horizontal("Dealer", dealer.mao);

        if (dealer.pontuacao >= 17) break;
        sleep(1);
        opcao = 1;
        if (opcao == 1) {
            adicionar_carta(&dealer.mao, comprar_carta(&baralho));
            printf(GREEN "\nDealer comprou uma carta\n" RESET);
        }
    } while (opcao != 2);

    printf("\n==== RESULTADOS ====\n");
    for (int i = 0; i < n; i++) {
        if (jogadores[i].pontuacao > 21) {
            printf(RED "%s estourou!\n" GREEN "Dealer venceu.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Perdeu (Estourou)");
        } else if (dealer.pontuacao > 21 || jogadores[i].pontuacao > dealer.pontuacao) {
            printf( GREEN "%s venceu o dealer!\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Venceu");
        } else if (jogadores[i].pontuacao == dealer.pontuacao) {
            printf(YELLOW "%s empatou com o dealer.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Empatou");
        } else {
            printf(GREEN"Dealer venceu %s.\n" RESET, jogadores[i].nome);
            salvar_resultado(f, rodada, jogadores[i].nome, "Perdeu");
        }
    }
    fprintf(f, "-------------------------\n\n");
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
    char buffer[10];
    int escolha = 0;
    do {
        printf("\n==== BlackJack 21 ====\n");
        printf("1. Jogar\n2. Ver placar\n3. Sair\n> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        
        if (buffer[0] == '\n') {
            printf(RED"\nEntrada invalida! Digite um numero.\n" RESET);
            continue;
        }
        
        if (sscanf(buffer, "%d", &escolha) != 1) {
            printf(RED"\nEntrada invalida! Digite um numero.\n" RESET);
            continue;
        }
        
        switch (escolha) {
            case 1: jogar(); break;
            case 2: mostrar_placar(); break;
            case 3:
                if (remove("placar.txt") == 0) {
                    printf("Placar Limpo!\n");
                } 
                Sair();
                break;
            default: printf(RED "Opcao invalida.\n" RESET); break;
        }
    } while (escolha != 3);
    return 0;
}
