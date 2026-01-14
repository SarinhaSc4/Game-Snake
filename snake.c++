#include <iostream>
using namespace std;
int main()
 cout << "Olá, Mundo!" << endl;
// ======================
// INCLUSÃO DE BIBLIOTECAS
// ======================

// Inclui a biblioteca gráfica SFML, responsável por:
// - criação de janelas
// - carregamento de imagens (texturas)
// - sprites
// - captura de eventos (teclado, mouse)
// - renderização na tela
#include <SFML/Graphics.hpp>

// Inclui a biblioteca de tempo padrão do C/C++
// Aqui ela é usada principalmente para:
// - inicializar o gerador de números aleatórios (srand / rand)
#include <time.h>

// Permite usar as classes da SFML diretamente
// Exemplo: RenderWindow em vez de sf::RenderWindow
using namespace sf;

// ======================
// CONFIGURAÇÕES DO JOGO
// ======================

// Quantidade de blocos horizontais do tabuleiro
int N = 30;

// Quantidade de blocos verticais do tabuleiro
int M = 20;

// Tamanho de cada bloco (tile) em pixels
int size = 16;

// Largura real da janela (em pixels)
// Calculada multiplicando o tamanho do bloco pela quantidade horizontal
int w = size * N;

// Altura real da janela (em pixels)
// Calculada multiplicando o tamanho do bloco pela quantidade vertical
int h = size * M;

// Variável que controla a direção atual da cobra
// s = baixo | a = esquerda | d = direita | w = cima
int dir;

// Quantidade inicial de segmentos da cobra
int num = 3;

// ======================
// ESTRUTURA DA COBRA
// ======================

// Estrutura que representa UM segmento da cobra
struct Snake
{
    int x; // posição horizontal (coluna do grid)
    int y; // posição vertical (linha do grid)
};

// Vetor que armazena até 100 segmentos da cobra
// s[0] = cabeça
// s[1..] = corpo
Snake s[100];

// ======================
// ESTRUTURA DA FRUTA
// ======================

// Estrutura que representa a fruta
struct Fruit
{
    int x; // posição horizontal da fruta
    int y; // posição vertical da fruta
};

// Instância única da fruta
Fruit f;

// ======================
// FUNÇÃO DE ATUALIZAÇÃO DO JOGO
// ======================

// Função responsável por atualizar a lógica do jogo:
// - movimento da cobra
// - crescimento
// - colisões
// - verificação de fruta
void Tick()
{
    // Move o corpo da cobra:
    // Começa do último segmento e vai até o segundo
    // Cada segmento copia a posição do segmento anterior
    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    // Atualiza a posição da cabeça da cobra conforme a direção
    if (dir == 0) s[0].y += 1; // move para baixo
    if (dir == 1) s[0].x -= 1; // move para a esquerda
    if (dir == 2) s[0].x += 1; // move para a direita
    if (dir == 3) s[0].y -= 1; // move para cima

    // Verifica se a cabeça da cobra encostou na fruta
    if (s[0].x == f.x && s[0].y == f.y)
    {
        num++;               // aumenta o tamanho da cobra
        f.x = rand() % N;    // gera nova posição aleatória em X
        f.y = rand() % M;    // gera nova posição aleatória em Y
    }

    // Tratamento das bordas da tela
    // Quando a cobra sai de um lado, ela reaparece do outro (teletransporte)
    if (s[0].x > N) s[0].x = 0;
    if (s[0].x < 0) s[0].x = N;
    if (s[0].y > M) s[0].y = 0;
    if (s[0].y < 0) s[0].y = M;

    // Verifica colisão da cabeça com o próprio corpo
    for (int i = 1; i < num; i++)
    {
        if (s[0].x == s[i].x && s[0].y == s[i].y)
        {
            // Se colidir, a cobra "encolhe" até o ponto da colisão
            num = i;
        }
    }
}

// ======================
// FUNÇÃO PRINCIPAL
// ======================

int main()
{
    // Inicializa o gerador de números aleatórios
    // Necessário para posições aleatórias da fruta
    srand(time(0));

    // Cria a janela do jogo com largura w e altura h
    // Título exibido na barra da janela
    RenderWindow window(VideoMode(w, h), "Snake Game!");

    // Declaração das texturas
    Texture t1, t2, t3;

    // Carrega as imagens do disco para as texturas
    t1.loadFromFile("C:/projetoSFML/x64/Debug/images/preto.jpg"); // fundo
    t2.loadFromFile("C:/projetoSFML/x64/Debug/images/roxo.png");  // cobra
    t3.loadFromFile("C:/projetoSFML/x64/Debug/images/red.png");   // fruta

    // Criação dos sprites a partir das texturas
    Sprite sprite1(t1); // fundo
    Sprite sprite2(t2); // cobra
    Sprite sprite3(t3); // fruta

    // Relógio para controle de tempo (FPS lógico)
    Clock clock;

    // Variável acumuladora de tempo
    float timer = 0;

    // Delay define a velocidade da cobra
    // Quanto menor, mais rápida
    float delay = 0.1f;

    // Posição inicial da fruta no grid
    f.x = 10;
    f.y = 10;

    // ======================
    // LOOP PRINCIPAL DO JOGO
    // ======================

    while (window.isOpen())
    {
        // Obtém o tempo decorrido desde o último frame
        float time = clock.getElapsedTime().asSeconds();

        // Reinicia o relógio
        clock.restart();

        // Soma o tempo acumulado
        timer += time;

        // Processamento de eventos (fechar janela, etc.)
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        // ======================
        // CONTROLE DA COBRA (W A S D)
        // ======================

        if (Keyboard::isKeyPressed(Keyboard::A)) dir = 1; // esquerda
        if (Keyboard::isKeyPressed(Keyboard::D)) dir = 2; // direita
        if (Keyboard::isKeyPressed(Keyboard::W)) dir = 3; // cima
        if (Keyboard::isKeyPressed(Keyboard::S)) dir = 0; // baixo

        // Atualiza a lógica do jogo apenas quando o tempo passa do delay
        if (timer > delay)
        {
            timer = 0;
            Tick();
        }

        // ======================
        // DESENHO NA TELA
        // ======================

        // Limpa a tela antes de desenhar
        window.clear();

        // Desenha o fundo (tabuleiro inteiro)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                sprite1.setPosition(i * size, j * size);
                window.draw(sprite1);
            }
        }

        // Desenha cada segmento da cobra
        for (int i = 0; i < num; i++)
        {
            sprite2.setPosition(s[i].x * size, s[i].y * size);
            window.draw(sprite2);
        }

        // Desenha a fruta
        sprite2.setPosition(f.x * size, f.y * size);
        window.draw(sprite2);

        sprite3.setPosition(f.x * size, f.y * size);
        window.draw(sprite3);

        // Exibe tudo que foi desenhado na janela
        window.display();
    }

    // Encerra o programa corretamente
    return 0;
}
