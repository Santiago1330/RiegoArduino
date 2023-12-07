//libreria time
#include <TimeLib.h>
//libreria DHT
#include <DHT.h>
//definicion de pin y tipo DHT22
#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE); //instancia

//sensor de tierra
int sensorPinCC = A0; // Pin analógico al que está conectado a la cama Kanche
int sensorPinCG = A1; // Pin analógico al que está conectado a la cama del suelo
int humidityCE = A2; // Pin analógico al que está conectado a la cama testigo 1
int humidityCF = A3; // Pin analógico al que está conectado a la cama testigo 2

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
  pinMode(humidityCE, INPUT);
  pinMode(humidityCF, INPUT);
  dht.begin();
  Serial.begin(9600);
  //Fijamos la fecha y hora actuales
  setTime(17, 15, 0, 30, 11, 2023);// Hora,Min,Seg - Dia,Mes,Año
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
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperatura del aire: ");
    Serial.print(temperature);
    Serial.print(" °C, Humedad del aire: ");
    Serial.print(humidity);
    Serial.println(" %");
  }else{
    Serial.println("Error al obtener informacion del sensor de aire");
  }

//lectura de los sensores de tierra
  int sensorValueCC = analogRead(sensorPinCC); // Lee el valor del sensor Cama kanche
  int sensorValueCG = analogRead(sensorPinCG); // Lee el valor del sensor Cama en el suelo
  int sensorValueCE = analogRead(humidityCE); // Lee el valor del sensor Cama testigo 1
  int sensorValueCF = analogRead(humidityCF); // Lee el valor del sensor Cama testigo 2

  //Validacion de la lectura de la cama kanche
  if (sensorValueCC >= minValue && sensorValueCC <= maxValue) {
    int porcentajeCC = map(sensorValueCC, minValue, maxValue, minPorcentaje, maxPorcentaje);
    Serial.print("Humedad del suelo de la cama Kanche: ");
    Serial.print(porcentajeCC);
    Serial.println("%");
  } else {
    Serial.println("Error al leer el sensor de tierra Cama kanche");
  }
  //Validacion de la lectura de la cama en el suelo
  if (sensorValueCG >= minValue && sensorValueCG <= maxValue) {
    int porcentajeCG = map(sensorValueCG, minValue, maxValue, minPorcentaje, maxPorcentaje);
    Serial.print("Humedad de la tierra de la cama en el suelo: ");
    Serial.print(porcentajeCG);
    Serial.println("%");
  } else {
    Serial.println("Error al leer el sensor de tierra de la cama en en suelo");
  }
  //Validacion de la lectura de la cama testigo 1
  if (sensorValueCE >= minValue && sensorValueCE <= maxValue) {
    int porcentajeCE = map(sensorValueCE, minValue, maxValue, minPorcentaje, maxPorcentaje);
    Serial.print("Humedad de la tierra de la cama TESTIGO 1: ");
    Serial.print(porcentajeCE);
    Serial.println("%");
  } else {
    Serial.println("Error al leer el sensor de tierra de la cama TESTIGO 1");
  }
  //Validacion de la lectura de la cama testigo 2
  if (sensorValueCF >= minValue && sensorValueCF <= maxValue) {
    int porcentajeCF = map(sensorValueCF, minValue, maxValue, minPorcentaje, maxPorcentaje);
    Serial.print("Humedad de la tierra de la cama TESTIGO 2: ");
    Serial.print(porcentajeCF);
    Serial.println("%");
  } else {
    Serial.println("Error al leer el sensor de tierra de la cama TESTIGO 2");
  }

  //Variables para las condiciones
  int hora = hour();

  //inicio de condiciones
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