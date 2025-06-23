#  Jogo de Cartas 21 (Blackjack)

Um jogo de Blackjack implementado em C com interface de terminal colorida e sistema de placar persistente.

##  Descrição

Este projeto implementa o clássico jogo de cartas Blackjack (21) em linguagem C. O jogo permite que até 4 jogadores compitam contra o dealer, com um sistema de pontuação que salva automaticamente os resultados de cada rodada.

##  Funcionalidades

- **Jogo completo de Blackjack** com regras tradicionais
- **Suporte a múltiplos jogadores** (1 a 4 jogadores)
- **Interface colorida** no terminal para melhor experiência visual
- **Sistema de placar persistente** que salva os resultados em arquivo
- **Animações** durante o embaralhamento das cartas
- **Gerenciamento automático do baralho** com pilha de descarte
- **Cálculo automático de pontuação** considerando as regras do Ás (1 ou 11)

##  Como Jogar

### Regras do Jogo
- O objetivo é obter uma pontuação mais próxima de 21 sem ultrapassá-la
- Cartas numéricas valem seu valor de face (2-10)
- Figuras (Valete, Dama, Rei) valem 10 pontos
- O Ás vale 1 ou 11 pontos (automaticamente ajustado para o melhor valor)
- O dealer deve comprar cartas até atingir pelo menos 17 pontos

### Controles
- **1**: Comprar uma carta
- **2**: Parar (manter a mão atual)
- **1-3**: Navegar pelo menu principal

##  Como Compilar e Executar

### Pré-requisitos
- Compilador C (GCC recomendado)
- Sistema operacional com suporte a ANSI escape codes (Linux, macOS, Windows 10+)

### Compilação
```bash
# Navegue até a pasta do projeto
cd Jogo

# Compile o programa
gcc -o jogo21 Jogo21.c

# Execute o jogo
./jogo21
```

### Para Windows (PowerShell)
```powershell
# Navegue até a pasta do projeto
cd Jogo

# Compile o programa
gcc -o jogo21.exe Jogo21.c

# Execute o jogo
.\jogo21.exe
```

##  Estrutura do Projeto

```
Jogo_Cartas_21_C/
├── Jogo/
│   └── Jogo21.c          # Código fonte principal
├── .gitignore            # Arquivos ignorados pelo Git
└── README.md             # Este arquivo
```

##  Funcionalidades Técnicas

### Estruturas de Dados
- **Carta**: Representa uma carta com naipe e valor
- **NoCarta**: Nó de lista encadeada para cartas
- **Jogador**: Contém nome, mão de cartas e pontuação

### Algoritmos Implementados
- **Embaralhamento**: Algoritmo Fisher-Yates para embaralhar o baralho
- **Cálculo de pontuação**: Lógica inteligente para o valor do Ás
- **Gerenciamento de memória**: Alocação e liberação dinâmica de cartas

### Sistema de Arquivos
- **placar.txt**: Arquivo que armazena o histórico de rodadas e resultados

##  Interface

O jogo utiliza códigos ANSI para colorir a saída:
- 🔴 **Vermelho**: Mensagens de erro e derrotas
- 🟢 **Verde**: Mensagens de sucesso e vitórias  
- 🟡 **Amarelo**: Informações e empates
- 🟠 **Laranja**: Cabeçalhos de rodadas

##  Sistema de Placar

O jogo mantém automaticamente um arquivo `placar.txt` que registra:
- Número da rodada
- Nome de cada jogador
- Resultado (Venceu/Perdeu/Empatou)
- Motivo da derrota (ex: "Estourou")

##  Personalização

Você pode modificar as seguintes constantes no código:
- `MAX_JOGADORES`: Número máximo de jogadores (atualmente 4)
- Cores ANSI: Definições de cores no início do arquivo
- Regras do dealer: Pontuação mínima para o dealer parar (atualmente 17)

##  Solução de Problemas

### Problemas de Compilação
- Certifique-se de ter o GCC instalado
- No Windows, use MinGW ou WSL para melhor compatibilidade

### Problemas de Cores
- Se as cores não aparecerem, seu terminal pode não suportar ANSI escape codes
- No Windows, certifique-se de que o terminal suporta cores ANSI

### Problemas de Arquivo
- O arquivo `placar.txt` é criado automaticamente na primeira execução
- Certifique-se de que o programa tem permissão de escrita no diretório

