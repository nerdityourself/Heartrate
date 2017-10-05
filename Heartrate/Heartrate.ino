
#include "filter.h"
#define ARRAY_SIZE 400

int LED13 = 13;   //  The on-board Arduion LED

int sample;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore. 
int BPM = 0;
unsigned long startMillis =0;
unsigned long lastSend =0;  
bool state= true;
int arraySample[ARRAY_SIZE];
int maximum = 0;
int minimum = 0;
int isteresi = 20;
int beatStatus = 0;

Filter filtroBPM;

// The SetUp Function:

void setup() {
pinMode(LED13,OUTPUT);         // pin that will blink to your heartbeat!Serial.begin(115200);         // Set's up Serial Communication at certain speed. 
pinMode(3,OUTPUT);         // pin that will blink to your heartbeat!Serial.begin(115200);         // Set's up Serial Communication at certain speed. 
filtroBPM.setDepthFilter(10);

Serial.begin(115200);


}



// The Main Loop Function

void loop() {

   sample = analogRead(A5);
  
   
   findThreshold();

   readBPM();

   if(millis() - lastSend > 100){
   Serial.println("Battiti: " + BPM);
   lastSend = millis();
   }
   delay(5);
}


void findThreshold()
{

  maximum = arraySample[0]; 
  minimum = arraySample[0];

  for(int i = 0; i < ARRAY_SIZE ;i++)
  {
    arraySample[i] = arraySample[i+1];
  }

  arraySample[ARRAY_SIZE -1] = sample;

  for(int i = 0; i < ARRAY_SIZE ;i++)
  {
    if(arraySample[i] > maximum) maximum = arraySample[i] ;
  }

   for(int i = 0; i < ARRAY_SIZE ;i++)
  {
     if(arraySample[i] < minimum) minimum = arraySample[i] ;
  }

   Threshold = (maximum + minimum)/2;

}

void readBPM()
{
  
 if(analogRead(A5) > Threshold  + isteresi)
   {                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.  
      
      digitalWrite(LED13,HIGH);  
      beatStatus = 1;
       
      if(!state)
      {
        calcBPM();
        state = true;
      }        
   }
   
   if(analogRead(A5) < Threshold  - isteresi){
      
      digitalWrite(LED13,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
      beatStatus = 0;
      state = false;  
   }

}

void calcBPM()
{
  
  
  double t = (double)(millis() - startMillis)/(double)1000;
  startMillis= millis();
  
  BPM = filtroBPM.filterSamples((double)60/t);

  //Serial.println(bpm);
  
}

