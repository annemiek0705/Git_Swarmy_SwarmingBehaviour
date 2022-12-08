#include <config.h>
#include <Swarmy.h>
#include <arduino-timer.h>

Swarmy bot(false);
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
long period = 5000;  //the value is a number of milliseconds
int count = 1;

void setup() {
  // put your setup code here, to run once:
  bot.OLEDprint("", "Driver!", "");
  startMillis = millis();  //initial start time
  bot.setInfraredFrequency(1000);
}

void loop() {
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period){
    if(count % 2){
     // bot.setMotorSpeed(0, -50);
     // bot.setMotorSpeed(1, -50); 
    }else{
     // bot.setMotorSpeed(0, 50);
     // bot.setMotorSpeed(1, 50);
    }
    count++;
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
}
