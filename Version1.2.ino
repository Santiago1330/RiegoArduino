//libreria time
#include <TimeLib.h>
//libreria DHT
#include <DHT.h>
//definicion de pin y tipo DHT22
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE); //instancia

//sensor de tierra
int sensorPinCC = A0; // Pin analógico al que está conectado el sensor
int sensorPinCG = A2;
int minValue = 1023;     // Valor mínimo del sensor
int maxValue = 0;  // Valor máximo del sensor
int minPorcentaje = 0;  // Porcentaje mínimo
int maxPorcentaje = 100;  // Porcentaje máximo

//relay
int relayPinCC = 7;
int relayPinCG = 4;


void setup() {
  pinMode(sensorPinCC, INPUT);
  pinMode(sensorPinCG, INPUT);
  dht.begin();
  Serial.begin(9600);
  //Fijamos la fecha y hora actuales
  setTime(15, 17, 0, 5, 07, 2023);// Hora,Min,Seg - Dia,Mes,Año
  pinMode(relayPinCC, OUTPUT);
  pinMode(relayPinCG, OUTPUT);
}

void loop() {
  //Impresion de la fecha y hora.
  Serial.print("Fecha: ");
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(year());
  Serial.print("   Hora: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.println(minute());

  // sensor
  float temperature = dht.readTemperature(); // Lee la temperatura en grados Celsius
  float humidity = dht.readHumidity(); // Lee la humedad relativa en porcentaje

  Serial.print("Temperatura del aire: ");
  Serial.print(temperature);
  Serial.print(" °C, Humedad del aire: ");
  Serial.print(humidity);
  Serial.println(" %");

  //sensores de tierra
  int sensorValueCC = analogRead(sensorPinCC); // Lee el valor del sensor Cama chica
  int sensorValueCG = analogRead(sensorPinCG); // Lee el valor del sensor Cama Grande
  //convertimos el valor del sensor en procentajes
  int porcentajeCC = map(sensorValueCC, minValue, maxValue, minPorcentaje, maxPorcentaje);
  int porcentajeCG = map(sensorValueCG, minValue, maxValue, minPorcentaje, maxPorcentaje);
  //Imprimimos los valores del sensor
  Serial.print("Humedad del suelo de la cama chica: ");
  Serial.print(porcentajeCC);
  Serial.println("%");
  Serial.print("Humedad del suelo de la cama grande: ");
  Serial.print(porcentajeCG);
  Serial.println("%");
  //Variables para las condiciones
  int hora = hour();

  //inicio de condiciones
  //el sistema debe estar activo de 6 pm a 8 am
  if ((hora >= 18 && hora <=24) ||(hora >= 1 && hora <=8) ){
    //Cama chica
    if (porcentajeCC <=10 ){
      digitalWrite(relayPinCC, HIGH); //encendemos el relay y por ende la electrioValvula
      Serial.println("El riego esta activo 1");
      //delay(30000);
      delay(60000); //se enciende durante un minuto.
      digitalWrite(relayPinCC, LOW);
    }else{
      Serial.println("La siembra sigue con humedad mayor al 20%");
      delay(59000);
    }
    //Cama grande
    if (porcentajeCG <=20 ){
      digitalWrite(relayPinCG, HIGH); //encendemos el relay y por ende la electrioValvula
      Serial.println("El riego esta activo 2");
      //delay(30000);
      delay(300000); //se enciende durante un cinco minutos.
      digitalWrite(relayPinCG, LOW);
      delay(40000);
    }else{
      Serial.println("La siembra sigue con humedad mayor al 20%");
      delay(59000);
    }
  }else{
    Serial.println("NO es momento de regar");
    Serial.print("Humedad del suelo cama chica: ");
    Serial.print(porcentajeCC);
    Serial.println("%");
    Serial.print("Humedad del suelo cama grande: ");
    Serial.print(porcentajeCG);
    Serial.println("%");
    delay(59000); //se espera un minuto
  }
}