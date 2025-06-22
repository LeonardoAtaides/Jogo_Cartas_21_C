<<<<<<< HEAD

# 🃏 Jogo de Cartas - BlackJack 21 (em C)

Este é um projeto de terminal em C que simula o famoso jogo **BlackJack (21)**, com suporte para até 4 jogadores e um dealer automático.

---

## 📁 Estrutura de Pastas

```
JOGO_CARTAS_21_C/
│
├── Jogo/
│   ├── Jogo21.c        # Código-fonte principal do jogo
│
├── README.md           # Este arquivo
├── .gitignore          # Arquivo para ignorar binários, etc.
```

---

## 🚀 Como Compilar e Executar

### 🔧 Requisitos
- Compilador C (GCC recomendado)
- Terminal (Linux, macOS ou Git Bash no Windows)

### 💻 Compilação (dentro da pasta `Jogo/`)
```bash
cd Jogo
gcc Jogo21.c -o Jogo21
=======
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

### ▶️ Execução
```bash
./Jogo21
```
Ou, no Windows:
```bash
Jogo21.exe
```

---

## 🎮 Funcionalidades

- Jogadores e Dealer com turnos individuais
- Sistema de pontuação com AS valendo 1 ou 11
- Exibição das cartas e somatório no terminal
- Registro automático de cada rodada em `placar.txt`
- Exibição do histórico de partidas

---

## 🗂️ Arquivo de Placar

O jogo salva o resultado de cada rodada no arquivo `placar.txt`, com informações como:

- Número da rodada
- Nome dos jogadores
- Resultado: `Venceu`, `Perdeu`, `Empatou`
