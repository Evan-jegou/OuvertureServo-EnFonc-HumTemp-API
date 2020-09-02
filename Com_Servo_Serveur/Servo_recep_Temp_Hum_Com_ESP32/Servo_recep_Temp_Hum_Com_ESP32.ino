
#include <LiquidCrystal.h>

#include <Servo.h>
Servo myServo;

#include <SoftwareSerial.h>
SoftwareSerial s(10,11);
int data;


int angle = 0;

int Etat = 0;

float previousTemp = 0;
float previousHum = 0;
float temp = 20;
float hum = 20;



LiquidCrystal lcd(13, 12, 5, 4, 3, 2);

int reply = 0;

void setup() 
{  
  pinMode(6, INPUT);
  
  myServo.attach(9); 

  s.begin(9600);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print ("Hello !");
  lcd.setCursor(0,1);
  lcd.print (" ");
  
  delay(1500);

  lcd.setCursor(0,0);
  lcd.print ("-------------");
  lcd.setCursor(0,1);
  lcd.print ("-------------");
}

void loop() 
{
                                        Recup_data_ESP();
                                        Com_servo();  
          
      if (Etat == 1)
      {
        lcd.setCursor(0,0);
        lcd.print ("Temperature :       ");
        lcd.setCursor(0,1);
        lcd.print (temp);        
        lcd.print (" degres C          ");  
      }
      if (Etat == 0)
      {
        lcd.setCursor(0,0);
        lcd.print ("Humidite :     ");
        lcd.setCursor(0,1);
        lcd.print (hum);     
        lcd.print (" pour cent       ");     
      }

                                        verif_Etat ();
}




void Recup_data_ESP()
{
  s.write("s");
  if (s.available()>0)
  {
    temp = s.read();
    
    while(s.read() != 255)
    {
    }
    delay(100);

    float tempDec = s.read();
    tempDec = tempDec/100;
    temp = temp + tempDec;
    Serial.println(temp);
    

        while(s.read() != 255)
        {
        }
        delay(100);
    

    hum = s.read();
    
    while(s.read() != 255)
    {
    }
    delay(100);

    float humDec = s.read();
    humDec = humDec/100;
    hum = hum + humDec;
    Serial.println(hum);
  }
}





int Com_servo()
{   
    if (previousTemp != temp || previousHum != hum)
    {
      int DeltaOpen = 160;
      float OpenPrctHum = 0;
      float OpenPrctTemp = 0;
  
      OpenPrctHum = 1 - ((hum - 15)/(60 - 15));
      OpenPrctTemp = ((temp - 10)/(30 - 10));
  
      float OpenPrct = (OpenPrctHum + OpenPrctTemp) / 2;
      float Opening = (OpenPrct * DeltaOpen);
  
      myServo.write(Opening);  
    }
    
      previousTemp = temp;
      previousHum = hum;
}




void verif_Etat ()
{
   delay(3000);
   Etat = !Etat;
}
