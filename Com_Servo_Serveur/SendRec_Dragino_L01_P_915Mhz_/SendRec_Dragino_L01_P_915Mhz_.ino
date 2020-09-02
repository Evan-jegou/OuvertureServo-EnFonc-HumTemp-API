#include <SPI.h>
#include <LoRa.h>
#include <Console.h>
#include <HttpClient.h>

int HEART_LED = A1;

// Set center frequency
uint32_t freq = 915E6;
int SF = 12, Denominator = 5;
long SBW = 125E3;

long old_time = millis();
long new_time;
long status_update_interval = 30000; //update Status every 30 seconds.

int EtatSend = 0;

char DataTempHum[256];


void receivepacket() 
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();

  if (packetSize) 
  {
    // received a packet
    Console.print("Received packet : ");
    // read packet
    int i = 0;
    char message[256] = "0";
    while (LoRa.available()) 
    {
      message[i] = LoRa.read();
      i++;
    }
    String Data = message;
    Console.print("Counter = ");
    Console.println(message);
                              OutPutInPutData(&Data);
    
    delay(10);
                              Accuse_reception ();
    delay(10);
   
    old_time = new_time;
  }
}




 
void OutPutInPutData(String *Data)
{    
  HttpClient client;

  url = "XXX"; // Entrez l'url de la requête. 
  client.get(url); 

  int a = 0;
  while (client.available()) 
  {    
    char c = client.read();
    if (c == 'e')
    {
      c = client.read();
      if (c == 'm')
      {
         c = client.read();
         if (c == 'p')
         {
           c = client.read();

           if (c != '_')
           {
             c = client.read();
             c = client.read();
             while (c != '.')
             {
              DataTempHum[a] = c;
              c = client.read();
              a++;
             }
             
             c = client.read();
             while (c != ',')
             {
              DataTempHum[a] = c;
              c = client.read();
              a++;
             }
           }
         }
      }
    }

    if (c == 'd')
        {
          c = client.read();
          if (c == 'i')
          {
             c = client.read();
             if (c == 't')
             {
               c = client.read();
               if (c == 'y')
                {
                 c = client.read();
                 c = client.read();
                 c = client.read();
                 while (c != '}')
                 {
                  DataTempHum[a] = c;
                  c = client.read();
                  a++;
                 }         
                }               
             }
           }
        }
    }  
}


void Accuse_reception ()
{  
    LoRa.beginPacket();
    Console.print("Send packet : ");
    Console.println(DataTempHum);

    LoRa.print(DataTempHum);  

    LoRa.endPacket();  
}


void show_config()
{
  Console.print("The frequency is "); Console.print(freq); Console.println("Hz");
  Console.print("The spreading factor is "); Console.println(SF);
  Console.print("The coderate is "); Console.println(Denominator);
  Console.print("The single bandwith is "); Console.print(SBW); Console.println("Hz");
}

void setup() 
{
  Bridge.begin(115200);
  Console.begin();
  //  while (!Console);
  pinMode(HEART_LED, OUTPUT);

  Console.println("LoRa Receiver");

  if (!LoRa.begin(freq)) 
  {
    Console.println("Starting LoRa failed!");
    while (1);
  }
  
  LoRa.setSpreadingFactor(SF);
  LoRa.setSignalBandwidth(SBW);
  LoRa.setCodingRate4(Denominator);
  LoRa.setSyncWord(0x34);
  LoRa.receive(0);
                                  show_config();
}

void loop() 
{
                                  receivepacket();
  new_time = millis();
                                  
  if ((new_time - old_time) > status_update_interval) 
  {
    Console.println("");
    Console.println("not receive overtime");
    
    LoRa.beginPacket();
    LoRa.print("Es-tu-là ?");
    Console.println("Es-tu-là ?");
    LoRa.endPacket();    
    old_time = new_time;
  }
}
