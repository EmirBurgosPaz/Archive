#include <Arduino.h>
#include <SoftwareSerial.h>
#include<Wire.h>    
#include <RTClib.h>
#include <SPI.h>

#define disk1 0x50
//Address of 24LC256 eeprom chip

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data );
byte readEEPROM(int deviceaddress, unsigned int eeaddress );
void nextionCommand(String nextionName, long value);



SoftwareSerial SerialNextion(2, 3); // RX, TX
String  endChar = String(char(0xff)) + String(char(0xff)) + String(char(0xff));
RTC_DS3231 RTC;

String dfd;
byte lastSec=0;


unsigned int address_red = 0;
unsigned int address_green = 1;
unsigned int address_blue = 2;


long red_value;
long green_value;
long blue_value;
int motor_state = 0;
int motor_state6 = 0;


int ledRojo = 9;
int ledVerde = 10;
int ledAzul = 11;

int hora;
int minuto;

int periodo = 2000;
unsigned long TiempoAhora = 0;

union main
{
  char charByte[4];
  long valLong;
}value;


void setup() {
  SerialNextion.begin(9600);                       //The default baud rate of the Nextion TFT is 9600.         
  Serial.begin(9600);
  Wire.begin();  
  RTC.begin();

  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);

  red_value = int(readEEPROM(disk1, address_red));
  green_value = int(readEEPROM(disk1, address_green));
  blue_value = int(readEEPROM(disk1, address_blue));

  analogWrite(ledRojo,red_value); 
  analogWrite(ledVerde,green_value); 
  analogWrite(ledAzul,blue_value); 
  digitalWrite(LED_BUILTIN, LOW);

  //writeEEPROM(disk1, address_red, 123);
  //writeEEPROM(disk1, address_green, 0);
  //writeEEPROM(disk1, address_blue, 0);

  delay(100);
}

void loop() {

DateTime now = RTC.now();

if(lastSec!=now.second())
  {lastSec=now.second();
    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.println(':');
  }

if ((now.hour()==hora) &  (now.minute()==minuto)){
      if((unsigned long) (millis() - TiempoAhora) > periodo){
        Serial.println("Alarma");
        TiempoAhora = millis();
      }
} 

if(SerialNextion.available()>0)                  //If we receive something...
  {
  dfd += char(SerialNextion.read());
  Serial.println(dfd);
  Serial.println(dfd.length());

  if((dfd.substring(0,5)=="green")& (dfd.length()==9)){

    value.charByte[0]= char(dfd[5]);

    green_value = value.valLong;  
    Serial.println(String(green_value));
    analogWrite(ledVerde,green_value); 

    dfd="";
  }

  if((dfd.substring(0,5)=="alarm")& (dfd.length()==9)){
    value.charByte[0]=char(dfd[5]);
    int alarmid = value.valLong;

    switch (alarmid)
    {
    case 0:
      Serial.println("Alarma1");
      break;
    case 1:
      Serial.println("Alaram2");
      break;
    case 2:
      Serial.println("Alarma3");
      break;
    case 3:
      Serial.println("Alarma4");
    default:
      break;
    }
  
    dfd="";
  }
    

  if((dfd.substring(0,4)=="time") & (dfd.length()==9)){
    
    value.charByte[0]= char(dfd[5]);

    if(dfd.substring(4,5)=="1")  hora = value.valLong;
    if(dfd.substring(4,5)=="2") minuto = value.valLong;

    Serial.println(hora);
    Serial.println(minuto); 
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
    red_value = int(readEEPROM(disk1, address_red));
    green_value = int(readEEPROM(disk1, address_green));
    blue_value = int(readEEPROM(disk1, address_blue));

    analogWrite(ledRojo,red_value); 
    analogWrite(ledVerde,green_value); 
    analogWrite(ledAzul,blue_value); 

    nextionCommand("h0.val", red_value);
    nextionCommand("h1.val", green_value);
    nextionCommand("h2.val", blue_value);
    dfd="";
  }
  
  if((dfd.substring(0,4)=="save")&(dfd.length()==4)){

    writeEEPROM(disk1, address_red, int(red_value));
    writeEEPROM(disk1, address_green, int(green_value));
    writeEEPROM(disk1, address_blue, int(blue_value));
    
    dfd="";
  }

  if((dfd.substring(0,5)=="motor")&(dfd.length()==10)){

    value.charByte[0]= char(dfd[6]);

    if(dfd.substring(4,5)=="1")  motor_state = value.valLong;
    if(dfd.substring(4,5)=="2")  motor_state = value.valLong;
    if(dfd.substring(4,5)=="3")  motor_state = value.valLong;
    if(dfd.substring(4,5)=="4")  motor_state = value.valLong;
    if(dfd.substring(4,5)=="5")  motor_state = value.valLong;
    
    if(motor_state==1) digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    if(motor_state==0) digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    dfd="";
  }

  if((dfd.substring(0,2)=="m6")&(dfd.length()==6)){
    value.charByte[0]= char(dfd[2]);
    motor_state6 = value.valLong;
    Serial.println(motor_state6);

    dfd="";
  }

  }
  
}


void nextionCommand(String nextionName, long value){
  SerialNextion.print(nextionName);
  SerialNextion.print("=");
  SerialNextion.print(String(value));
  SerialNextion.print(endChar);
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
 Wire.beginTransmission(deviceaddress);
 Wire.write((int)(eeaddress >> 8));   // MSB
 Wire.write((int)(eeaddress & 0xFF)); // LSB
 Wire.write(data);
 Wire.endTransmission();

  delay(5);
}
 
byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
 byte rdata = 0xFF;
 
 Wire.beginTransmission(deviceaddress);
 Wire.write((int)(eeaddress >> 8));   // MSB
 Wire.write((int)(eeaddress & 0xFF)); // LSB
 Wire.endTransmission();

 Wire.requestFrom(deviceaddress,1);
 
 if (Wire.available()) rdata = Wire.read();

 return rdata;
}

