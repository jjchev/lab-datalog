#include <OneWire.h>
#include <DallasTemperature.h>
#include <SD.h>

//Datalogger con arduino y microSD. Sensores LM35 (analog) y DS18B20 (digital). v0.5.0 /jjchev

// Declaracion de variables globales
const int pinDatosDQ = 2;
const int pinLM35 = A0;
const int pinLED = 5;
const int pinBOTON = 3;
const int cspin = 4;

int estadoBoton = 0;
unsigned long TiempoInicial = 0;
unsigned long Tiempo = 0;
int i = 0;
float voltajeLM = 0.0;
float Temp = 0;

File dataFile;


// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);


void setup() {
  Serial.begin(9600); // puerto serial 9600
  Serial.print(F("Iniciando SD ..."));
  if (!SD.begin(cspin))
  {
    Serial.println(F("Error al iniciar"));
    return;
  }
  Serial.println(F("Iniciado correctamente"));
  sensorDS18B20.begin();
  pinMode(pinLED , OUTPUT);
  pinMode(pinBOTON , INPUT);
  analogReference(INTERNAL);
  attachInterrupt(digitalPinToInterrupt(pinBOTON), reiniciar, CHANGE); //interrupcion de codigo para reiniciar el archivo SD


}



void loop() {


  Temp = 0;
  for (int j = 0; j <= 20; j++) {
    voltajeLM = analogRead(pinLM35);
    Temp = Temp + (voltajeLM * 1.1 * 100 / 1024.0);
    delay(50);
  }
  Temp = Temp / 20;


  Tiempo = millis();

  digitalWrite(pinLED , LOW);
  sensorDS18B20.requestTemperatures();
  Serial.print("Temperatura sensor Digital: ");
  Serial.println(sensorDS18B20.getTempCByIndex(0));
  Serial.print("Temperatura sensor Analogico: ");
  Serial.println(Temp); Serial.println("");
  Serial.println ((Tiempo - TiempoInicial) / 1000);



  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print((Tiempo - TiempoInicial) / 1000 / 60);
    dataFile.print(",Digital,");
    dataFile.println(sensorDS18B20.getTempCByIndex(0));
    dataFile.print((Tiempo - TiempoInicial) / 1000 / 60);
    dataFile.print(",Analogico,");
    dataFile.println(Temp);
    dataFile.close();

  }
  else {
    Serial.println("Error al abrir el archivo");
  }


  delay(60000); // Medir cada x (en milisegundos)


}

// En interrupciones no funciona delay
void reiniciar() {

  for (i = 0; i <= 100; i++) {
    digitalWrite(pinLED , HIGH);
    digitalWrite(pinLED , LOW);
  }
  TiempoInicial = millis();
  SD.remove("datalog.txt");
}
