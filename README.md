# Labirinto do Rato Cego

O Labirinto do Rato Cego é um jogo que tem como objetivo motivar e tornar a aprendizagem mais significativa para os estudantes de disciplinas iniciais de programação, através da teoria de Aprendizagem Significativa e da Aprendizagem Baseada em Problemas.

Nele, o jogador deve criar um código que guie um rato cego por um labirinto até a saída. O jogo é composto por três componentes: *Criador de Labirintos*, *Gerador de Movimentos* e *Visualizador de Labirintos*.
___
O *Visualizador de Labirintos* proporciona aos jogadores uma interface gráfica para selecionar configurações pré-jogo, como o labirinto, a quantidade de jogadores e o arquivo de movimento de cada rato. Por meio dessa interface, os jogadores também acompanham a movimentação de todos os ratos no labirinto e conferem o ranking final com a colocação de cada participante.

___
O componente *Gerador de Movimentos* cria uma comunicação entre o arquivo de mapa e o código criado pelo jogador. A partir dessa comunicação é criado o arquivo de movimento do rato, que é utilizado pelo *Visualizador de Labirintos* para simular a movimentação do rato no labirinto.

___
Através do componente *Criador de Labirintos* os usuários podem elaborar e editar seus próprios mapas. É possível selecionar a altura e largura do mapa e desenhar o labirinto, definindo pontos de decisão, caminhos, pontos de início e fim, e caminhos proibidos. Após o mapa ser criado, ele pode ser salvo em formato json e posteriormente editado.

O *Criador de Labirintos* é um software independente dos outros dois componentes e pode ser encontrado no repositório [RatoCego-Criador-de-Labirinto](https://github.com/Gabriel-Azevedo-Batalha/RatoCego-Criador-de-Labirinto)  


## Instalação
Passos para compilar e instalar o projeto em sistemas Linux baseados em Debian.

### Instalação das Dependências 

- [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.0):
```bash
sudo apt install libsdl2-2.0-0 libsdl2-dev
``` 

- [SDL2_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.8.2):
```bash
sudo apt install libsdl2-image-2.0-0 libsdl2-image-dev
``` 

- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.22.0)
```bash
sudo apt install libsdl2-ttf-2.0-0 libsdl2-ttf-dev
```

- [libSSL](https://github.com/openssl/openssl/releases/tag/openssl-3.0.13)
```bash
sudo apt install libssl-dev
```

### Visualizador de Labirintos
Execute este comando na pasta raiz para compilar o Visualizador de Labirintos.
```bash
make
```

Após compilar o projeto com sucesso, ainda no diretório raiz execute o programa com o comando:
```bash
./RatoCegoBin
```

### Gerador de Movimentos
Passos para a execução do componente Rato Cego:
- 1: Acesse o diretório **/movesGenerator** do repositório
- 2: Compile o arquivo **movesGenerator.cpp** com o comando:
```bash
g++ movesGenerator.cpp -o mg -lcrypto
```
- 3: Compile, se necessário, o código criado pelo jogador
- 4: Crie um arquivo de movimento usando `generateMoves.sh`, informando o programa criado pelo jogador e o mapa, exemplo:
```bash
./generateMoves.sh ./prog ../assets/maps/simple.json
```

### Windows

Passos para a execução do componente Visualizador de Labirintos em um sistema Windows:
- Passo 1: abra o projeto **LabirintoRatoCego.sln**, localizado na pasta raiz do repositório, utilizando o Visual Studio 2022.
- Passo 2: certifique-se de que a configuração de compilação está definida para x64.
- Passo 3: use o Depurador Local do Windows (Local Windows Debugger) para construir e executar o jogo.