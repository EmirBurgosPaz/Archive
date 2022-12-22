#include <Arduino.h>
#include <SoftwareSerial.h>
#include<Wire.h>    

#define disk1 0x50
//Address of 24LC256 eeprom chip

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data );
byte readEEPROM(int deviceaddress, unsigned int eeaddress );

unsigned int address_red = 0;
unsigned int address_green = 1;
unsigned int address_blue = 2;


SoftwareSerial SerialNextion(2, 3); // RX, TX
String  endChar = String(char(0xff)) + String(char(0xff)) + String(char(0xff));

long red_value;
long green_value;
long blue_value;
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
   Wire.begin();  

  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);

  analogWrite(ledRojo,int(readEEPROM(disk1, address_red))); 
  analogWrite(ledVerde,int(readEEPROM(disk1, address_green))); 
  analogWrite(ledAzul,int(readEEPROM(disk1, address_blue))); 
  digitalWrite(LED_BUILTIN, LOW);

  red_value = int(readEEPROM(disk1, address_red));
  green_value = int(readEEPROM(disk1, address_green));
  blue_value = int(readEEPROM(disk1, address_blue));

  //writeEEPROM(disk1, address_red, 123);
  //writeEEPROM(disk1, address_green, 0);
  //writeEEPROM(disk1, address_blue, 0);

}

void loop() {

if(SerialNextion.available()>0)                  //If we receive something...
  {
  dfd += char(SerialNextion.read());
  Serial.println(dfd);
  Serial.println(dfd.length());
  Serial.println(String(value.valLong));

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
    analogWrite(ledRojo,int(readEEPROM(disk1, address_red))); 
    analogWrite(ledVerde,int(readEEPROM(disk1, address_green))); 
    analogWrite(ledAzul,int(readEEPROM(disk1, address_blue))); 

    nextionCommand("h0.val", int(readEEPROM(disk1, address_red)));
    Serial.print(int(readEEPROM(disk1, address_red)));
    nextionCommand("h1.val", int(readEEPROM(disk1, address_green)));
    Serial.print(int(readEEPROM(disk1, address_blue)));
    nextionCommand("h2.val", int(readEEPROM(disk1, address_blue)));
    Serial.print(int(readEEPROM(disk1, address_green)));
    delay(100);
    dfd="";
  }
  
  if((dfd.substring(0,4)=="save")&(dfd.length()==4)){

    Serial.print("Works");
    writeEEPROM(disk1, address_red, int(red_value));
    writeEEPROM(disk1, address_green, int(green_value));
    writeEEPROM(disk1, address_blue, int(blue_value));
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