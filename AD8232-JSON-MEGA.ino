#include <Ethernet.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include "RestClient.h"

#define IP "192.168.0.100"
#define PORT 3003

const int LOplus = 10;
const int LOminus = 11;
const int mv = A1;

String postECG = "";

RestClient client = RestClient(IP, PORT);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 103 };
byte gateway[] = { 192, 168, 0, 1 };
byte subnet[] = { 255, 255, 255, 0 };

void setup()
{
  Serial.begin(9600);

  /*Serial.println("Conectado a la red:");
  client.dhcp();

  Serial.print("IP de Placa Ethernet Shield: ");
  Serial.println(Ethernet.localIP());*/

  Ethernet.begin(mac, ip, gateway, subnet);
  
  pinMode(LOplus, INPUT);
  pinMode(LOminus, INPUT);
}

void loop()
{  
  if ((digitalRead(LOplus) == 1) || (digitalRead(LOminus) == 1))
  {
    Serial.println('!');
  }
  
  else
  {
    Serial.println(mv);

    client.setHeader("Content-Type: application/json");

    StaticJsonBuffer<200> jsonBuffer;
    char json[256];  
    JsonObject& root = jsonBuffer.createObject();
    root["electro"] = mv;
    root.printTo(json, sizeof(json));
    Serial.println("");
    Serial.println(json);
  
    int statusCode = client.post("http://192.168.0.100:3003/api/electrocardiograma", json, &postECG);
  
    Serial.print("Repuesta del servidor: ");
    Serial.println(statusCode);
    Serial.println("");
    Serial.println("Respuesta MongoDB -> " +postECG);
    Serial.println("");
  
    postECG = "";
  }
  
  delay(10);
}
