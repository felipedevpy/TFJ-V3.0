#include <MCUFRIEND_kbv.h>  // Inclui a biblioteca para controlar a tela LCD TFT
#include <TouchScreen.h>     // Inclui a biblioteca para ler os dados da tela sensível ao toque
#include <Adafruit_GFX.h>    // Inclui a biblioteca gráfica da Adafruit, necessária para desenhar na tela
#include "Nanoshield_Termopar.h" // Biblioteca para controlar os termopares (sensores de temperatura)
#include <SPI.h>             // Inclui a biblioteca SPI (protocolo de comunicação para o display)
#include "Nanoshield_LCD.h"  // Inclui a biblioteca para o controle do LCD específico

Nanoshield_Termopar termopar(43, TC_TYPE_K, TC_AVG_OFF);  // Cria o objeto para o primeiro termopar, no pino 43
Nanoshield_Termopar termopar2(44, TC_TYPE_K, TC_AVG_OFF); // Cria o objeto para o segundo termopar, no pino 44

Nanoshield_LCD lcd;  // Cria o objeto do LCD (para funcionalidades adicionais)

#define XM A2     // Define os pinos para o controle da tela sensível ao toque
#define YP A3
#define YM 9
#define XP 8

#define TS_MINX 90  // Define os limites mínimos e máximos do touchscreen (valores de calibração)
#define TS_MINY 92
#define TS_MAXX 906
#define TS_MAXY 951

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  // Cria o objeto touchscreen

#define LCD_RESET A4  // Define o pino de reset do LCD
#define LCD_CS A3     // Define o pino de Chip Select (CS) do LCD
#define LCD_CD A2     // Define o pino de comando de dados (CD) do LCD
#define LCD_WR A1     // Define o pino de escrita (WR) do LCD
#define LCD_RD A0     // Define o pino de leitura (RD) do LCD

MCUFRIEND_kbv tft;  // Cria o objeto para controlar o LCD TFT

#define MINPRESSURE 10  // Define o limite mínimo de pressão para um toque ser considerado válido
#define MAXPRESSURE 1000 // Define o limite máximo de pressão

#define PRETO   0x0000  // Define a cor preta em formato hexadecimal
#define BRANCO  0xFFFF  // Define a cor branca
#define AMARELO 0xFFE0  // Define a cor amarela

bool error = false;  // Variável de controle para erros
bool valor_botao1 = false;  // Variável de controle para o botão 1
bool valor_botao2 = false;  // Variável de controle para o botão 2

int px = 150;  // Posição X do botão de interação na tela (botão de temperatura)
int py = 160;  // Posição Y do botão de interação na tela
int largura = 150; // Largura do botão
int altura = 225;  // Altura do botão
int tolerancia = 20;  // Tolerância para detectar toques na área do botão

unsigned long ultimoToque = 0;  // Armazena o timestamp do último toque
const unsigned long intervaloToque = 500;  // Intervalo mínimo de tempo entre toques (500ms)

void telaInicial();   // Declaração da função para a tela inicial
void telaBoasVindas(); // Declaração da função para a tela de boas-vindas

void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial
  termopar.begin();    // Inicializa o primeiro termopar
  termopar2.begin();   // Inicializa o segundo termopar
  uint16_t ID = tft.readID();  // Lê o ID do display TFT
  tft.begin(ID);       // Inicializa o display TFT com o ID lido
  tft.setRotation(3);  // Define a rotação da tela para 3 (orientação da tela)

  telaBoasVindas();    // Exibe a tela de boas-vindas
  telaInicial();       // Exibe a tela inicial
}

void loop() {
  unsigned long tempoAtual = millis();  // Armazena o tempo atual (milissegundos desde o início)

  TSPoint p = ts.getPoint();  // Obtém as coordenadas do toque na tela
  pinMode(XM, OUTPUT);  // Configura os pinos de controle do touchscreen
  digitalWrite(XM, LOW);  // Configura a leitura no pino XM
  pinMode(YP, OUTPUT);  
  digitalWrite(YP, HIGH); // Configura o pino YP para leitura
  pinMode(YM, OUTPUT);  
  digitalWrite(YM, LOW);  // Configura o pino YM para leitura
  pinMode(XP, OUTPUT);  
  digitalWrite(XP, HIGH); // Configura o pino XP para leitura

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE && (tempoAtual - ultimoToque > intervaloToque)) {  // Verifica se o toque é válido e se o tempo desde o último toque é suficiente
      ultimoToque = tempoAtual;  // Atualiza o timestamp do último toque
      p.x = tft.width() - (map(p.x, TS_MINX, TS_MAXX, tft.width(), 0));  // Mapeia a coordenada X do toque
      p.y = tft.height() - (map(p.y, TS_MINY, TS_MAXY, tft.height(), 0)); // Mapeia a coordenada Y do toque

      if (p.x > px - tolerancia && p.x < px + largura + tolerancia &&  // Verifica se o toque ocorreu dentro da área do botão
          p.y > py - tolerancia && p.y < py + altura + tolerancia) {

          tft.fillRect(186, 30, 85, 30, PRETO);  // Limpa a área de temperatura 1
          tft.fillRect(186, 170, 85, 30, PRETO); // Limpa a área de temperatura 2

          termopar.read();   // Lê o valor do primeiro termopar
          termopar2.read();  // Lê o valor do segundo termopar

          tft.setTextColor(BRANCO);  // Define a cor do texto como branco
          tft.setCursor(200, 179);  // Define a posição do cursor na tela para o primeiro valor de temperatura
          tft.print(termopar.getExternal() - 1.25);  // Exibe a primeira temperatura

          tft.setCursor(200, 40);   // Define a posição do cursor para o segundo valor de temperatura
          tft.print(termopar2.getExternal());  // Exibe a segunda temperatura
      }  
  }
  
}

void telaBoasVindas() {
  tft.setRotation(3);  // Define a rotação da tela para 3
  tft.fillScreen(BRANCO);  // Preenche a tela com a cor branca
  tft.setTextColor(PRETO);  // Define a cor do texto como preto
  tft.setTextSize(3);  // Define o tamanho do texto
  tft.setCursor(85, 100);  // Define a posição do texto na tela
  tft.print("TFJ V3.0");  // Exibe "TFJ V3.0" na tela
  delay(5000);  // Aguarda 5 segundos (5000 milissegundos)
  tft.fillScreen(PRETO);  // Preenche a tela com a cor preta
  if (termopar.hasError()) {  // Verifica se o primeiro termopar está com erro
    tft.setTextColor(AMARELO);  // Define a cor do texto como amarelo
    tft.setTextSize(1);  // Define o tamanho do texto como 1
    tft.setCursor(85, 80);  // Define a posição do texto
    tft.print("Termopar 1 com erro");  // Exibe mensagem de erro do termopar
  }
}

void telaInicial() {
  tft.setRotation(3);  // Define a rotação da tela para 3
  criarBotao(186, 145 , 90, 85, "", PRETO);  // Cria um botão de tamanho específico na tela
  criarBotao(186, 10, 90, 85, "", PRETO);   // Cria outro botão
  criarBotao(20, 100, 120, 50, "", PRETO);  // Cria um terceiro botão

  escreveEstado(92, 42, "Temp_1", AMARELO);  // Exibe o texto "Temp_1" na posição indicada
  escreveEstado(92, 176, "Temp_2", AMARELO); // Exibe o texto "Temp_2"
  escreveEstado(42, 115, "REFRESH", AMARELO); // Exibe o texto "REFRESH"
}

void escreveEstado(int posx, int posy, String texto, int cor) {
  tft.setCursor(posx, posy);  // Define a posição do texto
  tft.setTextColor(cor);      // Define a cor do texto
  tft.setTextSize(2);         // Define o tamanho do texto
  tft.print(texto);           // Exibe o texto
}

void criarBotao(int posx, int posy, int largura, int altura, String texto, int cor) {
  tft.fillRect(posx, posy, largura, altura, cor);  // Cria um botão retangular com a cor especificada
  tft.drawRect(posx, posy, largura, altura, PRETO); // Desenha a borda do botão
  tft.setCursor(posx + 12, posy + 1000);  // Define a posição do texto dentro do botão
  tft.setTextColor(BRANCO);  // Define a cor do texto como branco
  tft.setTextSize(3);        // Define o tamanho do texto
  tft.print(texto);          // Exibe o texto no botão
}
