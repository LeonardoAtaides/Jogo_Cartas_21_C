# Jogo de Cartas 21 (Blackjack) em C

Este é um jogo de cartas estilo Blackjack (21) implementado em linguagem C, jogável via terminal. O objetivo é somar cartas até chegar o mais próximo possível de 21 pontos, sem ultrapassar esse valor.

## Como jogar

- O jogo suporta de 1 a 4 jogadores humanos, jogando contra o dealer (computador).
- Cada jogador recebe duas cartas e pode escolher entre:
  - **Comprar carta** (tentar se aproximar de 21)
  - **Parar** (manter a pontuação atual)
- Se a pontuação passar de 21, o jogador "estoura" e perde a rodada.
- O dealer compra cartas até atingir pelo menos 17 pontos.
- Vence quem tiver a maior pontuação até 21, sem ultrapassar esse valor.
- O resultado de cada rodada é salvo em um arquivo `placar.txt`.

## Regras básicas

- As cartas de 2 a 10 valem seu valor de face.
- Valete, Dama e Rei valem 10 pontos.
- Ás pode valer 1 ou 11 pontos, dependendo do que for mais vantajoso para o jogador.
- Se empatar com o dealer, o resultado é registrado como empate.

## Instalação e execução

1. **Entre na pasta do código:**
   ```bash
   cd Jogo/
   ```

2. **Compile o programa:**
   ```bash
   gcc Jogo21.c -o Jogo21
   ```

3. **Execute o jogo:**
   ```bash
   ./Jogo21
   ```
   Ou, no Windows:
   ```bash
   Jogo21.exe
   ```

## Funcionalidades

- Interface simples via terminal.
- Suporte a até 4 jogadores.
- Placar persistente em `placar.txt` (criado automaticamente).
- Opção de visualizar o placar e limpar o histórico pelo menu principal.
- Animações simples de embaralhamento e limpeza de placar.

## Observações

- O arquivo `placar.txt` é criado na mesma pasta do executável e armazena o histórico das rodadas.
- Para limpar o placar, basta escolher a opção "Sair" no menu principal, que o arquivo será removido.
- O jogo utiliza sequências ANSI para colorir o texto no terminal. Em alguns terminais do Windows, pode ser necessário ativar o suporte a cores ANSI.

## Exemplo de uso

```
==== BlackJack 21 ====
1. Jogar
2. Ver placar
3. Sair
> 1
Quantos jogadores (1 a 4)? 2
Nome do jogador 1: Alice
Nome do jogador 2: Bob

==== Vez de Alice ====
Sua mao:
[10 de Copas] [As de Ouros] => Total: 21
1. Comprar carta
2. Parar
> 2
...

