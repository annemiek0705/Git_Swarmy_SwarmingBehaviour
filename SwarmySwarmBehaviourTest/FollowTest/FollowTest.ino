#include <config.h>
#include <Swarmy.h>
#include <math.h>
#include <arduino-timer.h>

Swarmy bot(false);

typedef enum{SEARCHING, FOLLOWING} BEHAVIOUR_STATES;
typedef enum{NONE, FORWARD, BACKWARD, LEFT, RIGHT, FORWARD_LEFT, FORWARD_RIGHT} MOTOR_DIRECTION;

MOTOR_DIRECTION currentDirection = NONE;
BEHAVIOUR_STATES state = SEARCHING;

int amplitude[8], frequency[8];

int ownFrequency = 2000;
int targetFrequency = 1000;
int threshold = 25;

unsigned long currentMillis;
unsigned long period = 1000;  //the value is a number of milliseconds
unsigned long startMillis;  //some global variables available anywhere in the program
static float desiredAmp = 150;
const float e = 2.71828;

void setup() {
  // put your setup code here, to run once:
  bot.OLEDprint("", "Starting", "");
  startMillis = millis();  //initial start time
  Serial.begin(115200);
  bot.setInfraredFrequency(ownFrequency);
}

void loop() {
  switch(state){
    case SEARCHING: {
      //bot.OLEDprint("", "Searching", "");
      currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
      
      if (currentMillis - startMillis >= period){  
        searchPattern();
        startMillis = currentMillis; 
      }

      int irFreq = receiveIR();
      int irAmp = determineAmplitude();
      if(irFreq > (targetFrequency - threshold) && irFreq < (targetFrequency + threshold)){
        state = FOLLOWING;    
      }
      bot.OLEDprint("Frequency: "+ String(irFreq),"Amplitude: " +String(irAmp),"");
      driving();
    }
    break;
    case FOLLOWING: {
      int irFreq = receiveIR();
      if(!(irFreq > (targetFrequency - threshold) && irFreq < (targetFrequency + threshold))){  
        state = SEARCHING;    
      }

      adjustDirection(determineDirection());  
      driving();
      
    }
    break;
  }
}

int receiveIR(){
  int freqFound, ampFound;
  bot.ReceiveIR();
  for(int i = 0; i < 8; i++){
    amplitude[i] = bot.getAmplitude(i);
    frequency[i] = bot.getFrequency(i);
    if(frequency[i]>500){
      freqFound = frequency[i];
      ampFound = amplitude[i];
    }
  }
  return freqFound;
}

int determineAmplitude(){
  int highestAmp = 0;
  for(int i=0; i<8; i++){
    if(amplitude[i]>highestAmp){
      highestAmp = amplitude[i];  
    }
  } 
  return highestAmp; 
}

int determineDirection(){
  int directionIndicator = 0;
  for(int i=0; i<8; i++){
    if((i==3|| i==4 ||i==5) && frequency[i]>500){//right
      directionIndicator += -1;
     }else if(i == 6 && frequency[i]>500){//backward
      directionIndicator += 1;
    }else if((i==0 ||i==1 || i == 7) && frequency[i]>500){ //left
      directionIndicator += 1;
    }else if(i== 2 && frequency[i]>500){//front
      directionIndicator += 0;
    }
  }
  return directionIndicator;    
}

int adjustDirection(int directionIndicator){
  if(directionIndicator>0){
    bot.OLEDprint("", "Swarmy to the left", "");
    currentDirection = LEFT;
    return 0;
  }else if(directionIndicator<0){
    bot.OLEDprint("","Swarmy to the right",""); 
    currentDirection = RIGHT;
    return 1;
  }else if(directionIndicator == 0){
    bot.OLEDprint("", "Swarmy to the front", "");
    currentDirection = FORWARD;
    return 2;
  }
}

void searchPattern(){
  int randDirection = random(4);
  
  if(randDirection == 0){ 
    bot.OLEDprint("", "Forward", "");
    currentDirection = FORWARD;
 }else if(randDirection == 1){
    bot.OLEDprint("", "Forward Left", "");
    currentDirection = FORWARD_LEFT;
  }else if(randDirection == 2){
    bot.OLEDprint("", "Backward", "");
    currentDirection = BACKWARD;
  }else if(randDirection == 3){
    bot.OLEDprint("", "Forward Right", "");
    currentDirection = FORWARD_RIGHT;
  }     
}

float determineSpeed(float measuredAmp){
  if(measuredAmp != desiredAmp){
    float x = desiredAmp - measuredAmp;
    float power = pow(e, 0.08*x);
    float division = 1/(1+ power);
    float equation = 1 - (division * 2);
    return equation;
  }else if(measuredAmp == 0){
    return 1;
  }else{
    return 0;  
  }
 
}

void driving(){
  switch(currentDirection){
    case LEFT:
      bot.setMotorSpeed(0, -determineSpeed(determineAmplitude())*100);
      bot.setMotorSpeed(1, determineSpeed(determineAmplitude())*25);
    break;
    
    case RIGHT:
      bot.setMotorSpeed(0, determineSpeed(determineAmplitude())*25);
      bot.setMotorSpeed(1, -determineSpeed(determineAmplitude())*100);
    break;
    
    case FORWARD:
      bot.setMotorSpeed(0, -determineSpeed(determineAmplitude())*100);
      bot.setMotorSpeed(1, -determineSpeed(determineAmplitude())*100); 
    break; 

    case FORWARD_LEFT:
      bot.setMotorSpeed(0, -determineSpeed(determineAmplitude())*100);
      bot.setMotorSpeed(1, -determineSpeed(determineAmplitude())*50);
    break;

    case FORWARD_RIGHT:
      bot.setMotorSpeed(0, -determineSpeed(determineAmplitude())*50);
      bot.setMotorSpeed(1, -determineSpeed(determineAmplitude())*100);
    break;
    
    case BACKWARD:
      bot.setMotorSpeed(0, determineSpeed(determineAmplitude())*100);
      bot.setMotorSpeed(1, determineSpeed(determineAmplitude())*100); 
    break;
    
    case NONE:
      bot.setMotorSpeed(0, 0);
      bot.setMotorSpeed(1, 0); 
    break;
  }
}
