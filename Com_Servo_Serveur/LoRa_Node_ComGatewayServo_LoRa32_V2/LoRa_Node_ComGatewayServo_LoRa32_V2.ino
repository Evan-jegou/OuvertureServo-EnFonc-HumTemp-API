#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  

#include <SoftwareSerial.h>
SoftwareSerial s(12,13);

int SF = 12, Denominator = 5;
long SBW = 125E3;

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     23   // GPIO23 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  915E6


unsigned int counter = 0;

String rssi = "RSSI --";
String packSize = "--";
String packet ;


float temp = 0;
float hum = 0;


void setup() 
{
  pinMode(16,OUTPUT);

  s.begin(9600);
  Serial.begin(9600);
  
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(915E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(SBW);
  LoRa.setCodingRate4(Denominator);
  LoRa.setSyncWord(0x34); 
 
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.println("init ok");   
  delay(1500);
}


void loop() 
{  

                                          ComArdui();


  
  // send packet
  Serial.print ("Message envoyé : ");
  Serial.println(String(counter));
  LoRa.beginPacket();
  LoRa.print(counter);
  LoRa.endPacket();

  long old_time = millis();
  long new_time = 0;
  int continuer = 0;

  while (((new_time - old_time) < 5000) && continuer == 0)
  {
    new_time = millis();
                                      receivepacket(&continuer);
    if (continuer == 1)
    {
       counter++;   
    }
  }        
}





void ComArdui()
{
  int tempInt = temp;
  int tempDec = (temp - tempInt)*100;
  
  int humInt = hum;
  int humDec = (hum - humInt)*100;
  
  if(s.available()>0)
  {
   int data = temp;
   s.write(data);
   delay(50);
   
   data = 255;
   s.write(data);
   delay(50);
   
   data = tempDec;
   s.write(data);

       data = 255;
       s.write(data);
       delay(50);

   data = hum;
   s.write(data);
   delay(50);
   
   data = 255;
   s.write(data);
   delay(50);
   
   data = humDec;
   s.write(data);
  }
}




void receivepacket(int *continuer) 
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) 
  {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    int i = 0;
    char message[256];
    while (LoRa.available()) 
    {
      message[i] = LoRa.read();
      i++;
    }
    int Nb = atoi(message);
    Serial.println(message);

    int A = Nb/100;
    temp = A/100.0;
    temp = temp-273.15;
    Serial.print("Température en °C : ");
    Serial.println(temp);
    
    hum = Nb - (A*100.0);
    Serial.print("Humidité en % : ");
    Serial.println(hum);
    
    *continuer = 1;
  }
}
