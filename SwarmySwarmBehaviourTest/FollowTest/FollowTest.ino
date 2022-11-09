#include <config.h>
#include <Swarmy.h>
#include <arduino-timer.h>

Swarmy bot(false);
typedef enum{Searching, Following}states;
states state = Searching;
int amplitude[8], frequency[8] ,foundFreq[8], initAmp[8];
bool swarmyFound = 0;
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds

void setup() {
  // put your setup code here, to run once:
  bot.OLEDprint("", "Starting", "");
  startMillis = millis();  //initial start time
  Serial.begin(115200);
}

void loop() {
  switch(state){
    case Searching:
      bot.OLEDprint("", "Searching", "");
      currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
      if (currentMillis - startMillis >= period){
        driving();
        startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
      }
  
      if(receiveIR()==1000 && swarmyFound == 0){
        swarmyFound = 1;
        state = Following;  
        bot.OLEDprint(String(frequency[0])+" " + String(frequency[1])+" "+ String(frequency[2]), String(frequency[3])+" "+ String(frequency[4])+ " "+String(frequency[5]),String(frequency[6])+" "+String(frequency[7]));  
      }
    break;
    
    case Following:
      adjustDirection(determineDirection());
      bot.OLEDprint(String(amplitude[0])+" " + String(amplitude[1])+" "+ String(amplitude[2]), String(amplitude[3])+" "+ String(amplitude[4])+ " "+String(amplitude[5]),String(amplitude[6])+" "+String(amplitude[7]));  
      
      if(receiveIR()!=1000){
        swarmyFound = 0;
        state = Searching;  
      } 
      break;
  }
}

int receiveIR(){
  int freqFound;
  bot.ReceiveIR();
  for(int i = 0; i < 8; i++){
    amplitude[i] = bot.getAmplitude(i);
    frequency[i] = bot.getFrequency(i);
    if(frequency[i]>500){
      Serial.println("Frequency found");
      freqFound = frequency[i];
    }
  }
  return freqFound;
}
int determineAmp(){
  int highestAmplitude = 0;
  for(int i=0; i<8;i++){
    if(highestAmplitude<amplitude[i]){
      highestAmplitude = amplitude[i];  
    }
  }
  return highestAmplitude;
}

int adjustPosition(int foundAmp){
  if(foundAmp<98){
    //driveforward
    bot.setMotorSpeed(0, -100);
    bot.setMotorSpeed(1, -100); 
  }else if(foundAmp>100){
    //drivebackward
    bot.setMotorSpeed(0, 100);
    bot.setMotorSpeed(1, 100); 
  }
}

int determineDirection(){
  int totalFreq = 0;
  for(int i=0; i<8; i++){
    if((i==3|| i==4 ||i==5) && frequency[i]>500){//right
      totalFreq += -1;
    }else if(i == 6 && frequency[i]>500){//backward
      totalFreq += 10;
    }else if((i==0 ||i==1 || i == 7) && frequency[i]>500){ //left
      totalFreq += 1;
    }else if(i== 2 && frequency[i]>500){//front
      totalFreq += 0;
    }
  }
  return totalFreq;    
}

void adjustDirection(int totalFreq){
  if(totalFreq>0){
    bot.OLEDprint("", "Swarmy to the left", "");
    bot.setMotorSpeed(0, 0);
    bot.setMotorSpeed(1, 100);  
  }else if(totalFreq<0){
    bot.OLEDprint("","Swarmy to the right","");
    bot.setMotorSpeed(0, 100);
    bot.setMotorSpeed(1, 0);  
  }else if(totalFreq == 0){
    bot.OLEDprint("", "Swarmy to the front", "");
   // if(determineAmp()>100){
      bot.setMotorSpeed(0, 0);
      bot.setMotorSpeed(1, 0); 
    //}//adjustPosition(determineAmp());
  }
}
void driving(){
  long randLong = random(4);
  switch(randLong){
    case 0:
      bot.setMotorSpeed(0, -100);
      bot.setMotorSpeed(1, 100);
      delay(400); 
    break;
    case 1:
      bot.setMotorSpeed(0, 100);
      bot.setMotorSpeed(1, -100);
      delay(400);
    break;
    case 2:
      bot.setMotorSpeed(0, 100);
      bot.setMotorSpeed(1, 100); 
    break;
    case 3:
      bot.setMotorSpeed(0, -100);
      bot.setMotorSpeed(1, -100); 
    break;  
  }
}
