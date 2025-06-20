#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void mostrar_mao(const char *nome, NoCarta *mao) {
    printf("\nMao de %s:\n", nome);
    while (mao) {
        printf("- %s de %s\n", nome_valor(mao->carta.valor), nome_naipe(mao->carta.naipe));
        mao = mao->prox;
    }
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

void embaralhar(NoCarta **baralho) {
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

void salvar_placar(const char *nome, int pontos) {
    FILE *f = fopen("placar.txt", "a");
    if (f) {
        fprintf(f, "%s: %d\n", nome, pontos);
        fclose(f);
    }
}

void mostrar_placar() {
    FILE *f = fopen("placar.txt", "r");
    if (f) {
        char linha[100];
        printf("\n==== PLACAR ====\n");
        while (fgets(linha, sizeof(linha), f)) {
            printf("%s", linha);
        }
        fclose(f);
    } else {
        printf("Nenhum placar salvo ainda.\n");
    }
}

void jogar() {
    srand(time(NULL));
    NoCarta *baralho = criar_baralho();
    embaralhar(&baralho);

    Jogador jogador = {"Jogador", NULL, 0};
    Jogador dealer = {"Dealer", NULL, 0};

    adicionar_carta(&jogador.mao, comprar_carta(&baralho));
    adicionar_carta(&jogador.mao, comprar_carta(&baralho));
    adicionar_carta(&dealer.mao, comprar_carta(&baralho));

    int opcao;
    do {
        jogador.pontuacao = calcular_pontuacao(jogador.mao);
        mostrar_mao(jogador.nome, jogador.mao);
        printf("Total: %d\n", jogador.pontuacao);

        if (jogador.pontuacao > 21) {
            printf("Voce estourou!\n");
            break;
        }

        printf("1. Comprar carta\n2. Parar\n> ");
        scanf("%d", &opcao);

        if (opcao == 1) adicionar_carta(&jogador.mao, comprar_carta(&baralho));
    } while (opcao != 2);

    if (jogador.pontuacao <= 21) {
        while ((dealer.pontuacao = calcular_pontuacao(dealer.mao)) < 17) {
            adicionar_carta(&dealer.mao, comprar_carta(&baralho));
        }
        mostrar_mao(dealer.nome, dealer.mao);
        printf("Dealer total: %d\n", dealer.pontuacao);

        if (dealer.pontuacao > 21 || jogador.pontuacao > dealer.pontuacao) {
            printf("Voce venceu!\n");
            salvar_placar(jogador.nome, 1);
        } else if (dealer.pontuacao == jogador.pontuacao) {
            printf("Empate!\n");
            salvar_placar(jogador.nome, 0);
        } else {
            printf("Dealer venceu!\n");
            salvar_placar(jogador.nome, 0);
        }
    } else {
        salvar_placar(jogador.nome, 0);
    }

    liberar_mao(jogador.mao);
    liberar_mao(dealer.mao);
    liberar_mao(baralho);
}

int main() {
    int escolha;
    do {
        printf("\n==== JOGO 21 ====");
        printf("\n1. Jogar\n2. Ver placar\n3. Sair\n> ");
        scanf("%d", &escolha);
        switch (escolha) {
            case 1: jogar(); break;
            case 2: mostrar_placar(); break;
            case 3: printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (escolha != 3);
    return 0;
}