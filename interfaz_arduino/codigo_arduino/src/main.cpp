#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial SerialNextion(2, 3); // RX, TX
String  endChar = String(char(0xff)) + String(char(0xff)) + String(char(0xff));

long red_value = 128;
long green_value = 128;
long blue_value = 128;
int motor_state = 0;

int ledRojo = 9;
int ledVerde = 10;
int ledAzul = 11;

String dfd;

union main
{
  char charByte[4];
  long valLong;
}value;

void nextionCommand(String nextionName, long value);

void setup() {
  SerialNextion.begin(9600);                       //The default baud rate of the Nextion TFT is 9600.         
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);

  analogWrite(ledRojo,red_value); 
  analogWrite(ledVerde,green_value); 
  analogWrite(ledAzul,ledAzul); 
  digitalWrite(LED_BUILTIN, LOW);

}

void loop() {

if(SerialNextion.available()>0)                  //If we receive something...
  {
    dfd += char(SerialNextion.read());
    Serial.print(dfd);
    Serial.println(dfd.length());
    Serial.print(String(value.valLong));
  }

 

if((dfd.substring(0,5)=="green")& (dfd.length()==9)){

    value.charByte[0]= char(dfd[5]);

    green_value = value.valLong;  
    Serial.println(String(green_value));
    analogWrite(ledVerde,green_value); 

    dfd="";
  }

if((dfd.substring(0,4)=="blue")& (dfd.length()==8)){

    value.charByte[0]= char(dfd[4]);

    blue_value = value.valLong;  
    Serial.println(String(blue_value));
    analogWrite(ledAzul,blue_value); 

    dfd="";
  }

if((dfd.substring(0,3)=="red")& (dfd.length()==7)){

    value.charByte[0]= char(dfd[3]);

    red_value = value.valLong; 
    analogWrite(ledRojo,red_value); 
    dfd="";
  }

if ((dfd.substring(0,3)=="set") & (dfd.length()==3))
  {
    nextionCommand("h0.val", red_value);
    delay(100);
    nextionCommand("h1.val", green_value);
    delay(100);
    nextionCommand("h2.val", blue_value);
    dfd="";
  }
  

if((dfd.substring(0,5)=="motor")&(dfd.length()==9)){

    value.charByte[0]= char(dfd[5]);

    long(motor_state) = value.valLong;  
    if(motor_state==1) digitalWrite(LED_BUILTIN, HIGH);
    if(motor_state==0) digitalWrite(LED_BUILTIN, LOW);
    dfd="";
  }
  
}

void nextionCommand(String nextionName, long value){
  SerialNextion.print(nextionName);
  SerialNextion.print("=");
  SerialNextion.print(String(value));
  SerialNextion.print(endChar);
}