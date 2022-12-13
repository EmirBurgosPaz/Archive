#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialNextion(2, 3); // RX, TX
String  endChar = String(char(0xff)) + String(char(0xff)) + String(char(0xff));

long valor = 128;
String dfd;

union main
{
  char charByte[4];
  long valLong;
}value;


void setup() {
  SerialNextion.begin(9600);                       //The default baud rate of the Nextion TFT is 9600.         
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);  
}

void loop() {
  if(SerialNextion.available()>0)                  //If we receive something...
  {
    dfd += char(SerialNextion.read());
    Serial.print(dfd);
    Serial.println(dfd.length());
  }

  if ((dfd.substring(0,3)=="set") & (dfd.length()==3))
  {
    SerialNextion.print("h2.val=");
    SerialNextion.print(String(valor));
    SerialNextion.print(endChar);
    dfd="";
  }
  

  if((dfd.substring(0,3)=="red")& (dfd.length()==7)){

    value.charByte[0]= char(dfd[3]);

    valor = value.valLong;

    Serial.println(String(value.valLong));

    SerialNextion.print("h1.val=");
    SerialNextion.print(String(value.valLong));
    SerialNextion.print(endChar);
    
    dfd="";
  }
  
}