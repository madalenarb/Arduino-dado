#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <avr/io.h>
#define F_CPU 16000000UL


#define PIN       12
#define NUMPIXELS 12
// define o botão
int button;

/*matriz que indica os pins aos quais estam conectados, 
respetivamente, os segmentos A, B, C, D, E, F, G, */
int leds[][7] = {  2,    3,    6,    5,    4,    7,    8 };

/* matriz que define quais segmentos têem que estar a HIGH de modo a 
imprimir os repetivos números/símbolo*/
int digits[][7] = {
         /*          A     B     C     D     E     F     G  */
         /* 1 */  { !LOW, !HIGH, !HIGH,  !LOW,  !LOW,  !LOW,  !LOW},
         /* 2 */  {!HIGH, !HIGH, !LOW, !HIGH, !HIGH,  !LOW, !HIGH},
         /* 3 */  {!HIGH, !HIGH, !HIGH, !HIGH,  !LOW,  !LOW, !HIGH},
         /* 4 */  { !LOW, !HIGH, !HIGH,  !LOW,  !LOW, !HIGH, !HIGH},
         /* 5 */  {!HIGH,  !LOW, !HIGH, !HIGH,  !LOW, !HIGH, !HIGH},
         /* 6 */  {!HIGH,  !LOW, !HIGH, !HIGH, !HIGH, !HIGH, !HIGH},
         /* - */  {!LOW, !LOW,   !LOW,  !LOW,  !LOW,  !LOW, !HIGH},
};

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 400

// definir variáveis 
int flag = 0;
int number = 0;
int i = 0;
int c = 0;
int ndelay = DELAYVAL;

void setup() {
  Serial.begin(9600);
  pixels.begin();
  //inicializar o pseudo-gerador de códigos
  randomSeed(millis());
  pinMode(A0, INPUT);
  //inicializar o ciclo que vai ativar os segmentos
     for (int segment = 0; segment < 7; segment++) {
        pinMode(leds[0][segment], OUTPUT);
     
  }
  for (int segment = 0; segment < 7; segment++){
        digitalWrite(leds[0][segment], digits[6][segment]);
        }
}

void loop() {
  // Estado 0- HOLD (enquanto não se carrega no botão)
  if(digitalRead(A0) == LOW){
    // Estado 1- RESET
    // Inicializa os seguintes valores depois de carregar no botão
    flag = 0; 
    c = 0;
    i = 0;
    ndelay = DELAYVAL;
    number = 0;
  }
  // Estado 2- LOAD (Carrega um número)
    if(flag == 0){
            pixels.clear();
      for(int n = 0; n <= 5; n++){
        int a = (i - n) >= 0 ? i - n: NUMPIXELS + (i-n);
          pixels.setPixelColor(a , pixels.ColorHSV(c , 255, 255));
      }
      pixels.show();
      i = (i + 1)%NUMPIXELS;
      c = (c + 1500)%65536;
      delay(ndelay);

      if(ndelay > 10){
          ndelay /= 1.05;
      } else {
        for(int n = 0; n < NUMPIXELS; n++){
          pixels.setPixelColor(n , pixels.ColorHSV(c , 255, 255));
          pixels.show();
          //coloca flag a 1 para para o loop até se carregar no botão
          flag = 1;
        }
        // A variável num toma um valor aleatório
        number = (random(1,7));
        Serial.println(number);
        // Ativa os segmentos que iram posteriormente formar o number
        for (int segment = 0; segment < 7; segment++){
        digitalWrite(leds[0][segment], digits[number - 1][segment]);
        }
        delay(200);
      }
    } 
}
