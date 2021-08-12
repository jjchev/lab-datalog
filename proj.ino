
// Declaracion de variables globales
// Sensores LM35
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

#define ANCHO 128
#define ALTO 64
#define OLED_RESET 4
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);


float tempC; // valor obtenido del sensor (0 a 1023)
float tempMedia; // valor medio de las 50 lecturas
int pinLM35 = 0; // Sensor en puerto analogico 0

void setup() {
  // Configuramos el puerto serial a 9600 bps
  Serial.begin(9600); // puerto serial 9600
  Wire.begin();

}

void loop() {

  tempMedia = 0; // inicializar antes de cada loop for

  for (int i = 0; i <= 50; i++) {
    tempC = analogRead(pinLM35);
    tempC = (tempC * 5.0 * 100) / 1024;
    tempMedia = tempMedia + tempC;
    delay(10); // delay de 100ms entre cada lectura, for de 50 ciclos = temperatura media en 5 segundos
  }

  tempMedia = tempMedia / 50;
  Serial.print(tempMedia);
  Serial.print("\n");
  delay(0); // delay de grabacion de datos a consola o archivo (en milisegundos, restar los 5 segundos del for)



}
