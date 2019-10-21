// Strandbeest RC V1.0 -- 2016-01-21
// LED Statusanzeige, Modellumschaltung, Heartbeat
// FlySky Receiver

#include <Servo.h> 
#include <FastLED.h>

#define BAUDRATE 19200
#define Anzahl 3
#define OutputPin 3

CRGB LEDs[Anzahl];
Servo links;
Servo rechts;

#define RechtsFwd 180
#define RechtsNeutral 80
#define RechtsBck 0

#define LinksFwd 0
#define LinksNeutral 80
#define LinksBck 180

#define RCchA 9
#define RCchB 8
#define RCchE 2

long chA;
//int  chAi;
long chB;
//int  chBi;
long chE;

int huecnt=0;
int redcnt=0;
int reddir=0;

void setup()
{
  Serial.begin(BAUDRATE);
  delay(100);
  pinMode(RCchA, INPUT);
  pinMode(RCchB, INPUT);
  pinMode(RCchE, INPUT);
//  Serial.println("Init");
  
  FastLED.addLeds <NEOPIXEL, OutputPin> (LEDs, Anzahl);
  
  rechts.attach(5);
  rechts.write(RechtsNeutral);
  links.attach(6);
  links.write(LinksNeutral);

  LEDs[0]=0x004000;
  LEDs[1]=0x000000;
  LEDs[2]=0x000000;
  FastLED.show();
  delay(500);
  LEDs[1]=0x004000;
  FastLED.show();
  delay(500);
  LEDs[2]=0x004000;
  FastLED.show();
  delay(500);
   
  //ready - blau
  LEDs[0]=0x000040;
  LEDs[1]=0x000040;
  LEDs[2]=0x000040;
  FastLED.show();
    
  //Serial.print("Done...\n");
}

void loop()
{
  chA = pulseIn(RCchA, HIGH, 30000);
  if (chA==0)
  {
    //no rc
    rechts.write(RechtsNeutral);
    links.write(LinksNeutral);
    chA = pulseIn(RCchA, HIGH, 30000);
    
    // HUE CYCLE
    LEDs[0] = CHSV( huecnt, 255, 128);
    LEDs[1] = CHSV( huecnt, 255, 128);
    LEDs[2] = CHSV( huecnt, 255, 128);
    FastLED.show();
    huecnt++;
    if (huecnt==256){huecnt=0;}
  }
  else
  {
    chE = pulseIn(RCchE, HIGH, 30000); //Check for Enable
    if (chE < 1200) //Schalter SWA = 0
    {
      //Kein Enable
      //Alles auf Null
      rechts.write(RechtsNeutral);
      links.write(LinksNeutral);
      
      //Neutral - r/l blau, mitte rot mit heartbeat
      LEDs[0]=0x000040;
      LEDs[1].r=redcnt;
      LEDs[1].g=0;
      LEDs[1].b=0;
      LEDs[2]=0x000040;
      FastLED.show();

      //heartbeat
      if (reddir==0) 
      {
        //count up
        redcnt++;
        if (redcnt==80)
        {
          reddir=1; //Zährichtung umschalten auf abwärts
        }
      }
      else
      {
        //count down
        redcnt--;
        if (redcnt==0)
        {
          reddir=0; //Zährichtung umschalten auf abwärts
        }
      }
    }
    else //Schalter SWA = 1
    {
      chB = pulseIn(RCchB, HIGH, 30000);
      if (chB < 1200) //Links
      {
        if (chA > 1700) //Vor
        {
          //Serial.println("ForwardLeft");
          rechts.write(RechtsFwd);
          links.write(LinksNeutral);
          //rechtes Servo vorwärts
          LEDs[0]=0x000040;
          LEDs[1]=0x804000;
          LEDs[2]=0x004000;
          FastLED.show();
        }
        else if  (chA < 1200) //Rueck
        {
          //Serial.println("BackLeft");
          rechts.write(RechtsBck);
          links.write(LinksNeutral);
          //rechtes Servo rückwärts
          LEDs[0]=0x000040;
          LEDs[1]=0x804000;
          LEDs[2]=0x400000;
          FastLED.show();
        }
        else 
        {
          //Serial.println("Left");
          rechts.write(RechtsFwd);
          links.write(LinksBck);
          //rechtes Servo vorwärts, linkes Servo rückwärts
          LEDs[0]=0x400000;
          LEDs[1]=0x804000;
          LEDs[2]=0x004000;
          FastLED.show();
        }
      }
      else if  (chB > 1700) //rechts
      {
        if (chA > 1700) //Vor
        {
          //Serial.println("ForwardRight");
          rechts.write(RechtsNeutral);
          links.write(LinksFwd);
          //linkes Servo vorwärts
          LEDs[0]=0x004000;
          LEDs[1]=0x804000;
          LEDs[2]=0x000040;
          FastLED.show();
        }
        else if  (chA < 1200) //Rueck
        {
          //Serial.println("BackRight");
          rechts.write(RechtsNeutral);
          links.write(LinksBck);
          //linkes Servo rückwärts
          LEDs[0]=0x400000;
          LEDs[1]=0x804000;
          LEDs[2]=0x000040;
          FastLED.show();
        }
        else //
        {
          //Serial.println("Right");
          rechts.write(RechtsBck);
          links.write(LinksFwd);
          //rechtes Servo rückwärts, linkes Servo vorwärts
          LEDs[0]=0x004000;
          LEDs[1]=0x804000;
          LEDs[2]=0x400000;
          FastLED.show();
        }
      }
      else
      {
        if (chA > 1700) //Vor
        {
          //Serial.println("Forward");
          rechts.write(RechtsFwd);
          links.write(LinksFwd);
          //rechtes Servo vorwärts, linkes Servo vorwärts
          LEDs[0]=0x004000;
          LEDs[1]=0x804000;
          LEDs[2]=0x004000;
          FastLED.show();
        }
        else if  (chA < 1200) //Rueck
        {
          //Serial.println("Back");
          rechts.write(RechtsBck);
          links.write(LinksBck);
          //rechtes Servo rückwärts, linkes Servo rückwärts
          LEDs[0]=0x400000;
          LEDs[1]=0x804000;
          LEDs[2]=0x400000;
          FastLED.show();
        }
        else //
        {
          //Serial.println("Neutral");
          rechts.write(RechtsNeutral);
          links.write(LinksNeutral);
          //Neutral - Alle blau
          LEDs[0]=0x000040;
          LEDs[1]=0x804000;
          LEDs[2]=0x000040;
          FastLED.show();
        }
      }
    }
  }
}
