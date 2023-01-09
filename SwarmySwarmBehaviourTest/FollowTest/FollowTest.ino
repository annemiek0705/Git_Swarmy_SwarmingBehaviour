#include <neotimer.h>
#include <config.h>
#include <Swarmy.h>
#include <math.h>

#define echoPin 12
#define trigPin 13

Swarmy bot(false);

typedef enum{SEARCHING, FOLLOWING, LEADER, TARGET_LOST} BEHAVIOUR_STATES;
typedef enum{NONE,FORWARD, BACKWARD, LEFT, RIGHT} MOTOR_DIRECTION;

MOTOR_DIRECTION currentDirection = NONE;
MOTOR_DIRECTION previousDirection = NONE;
BEHAVIOUR_STATES state = SEARCHING;

int amplitude[8], frequency[8];

///Debug variables make not global when possible;
int ownFrequency = 2000;
bool flash = false;
int objectDistance = 1000;
int flashTrigger = 0;
  int modulusAmount = 2;
////



//Timers
Neotimer searchTimer = Neotimer(1000);
Neotimer irTimer = Neotimer(300);
Neotimer objectTimer = Neotimer(10);
Neotimer targetLost = Neotimer(2000);
Neotimer buffTimer = Neotimer(10);

void setup() {
  bot.OLEDprint("", "Starting", "");
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  searchTimer.start();
  irTimer.start();
  objectTimer.start();
}

void loop() {
  switch(state){
    case SEARCHING: {
      bot.OLEDprint("", "Searching", "");
      flashIR();
      detectObject();
      if(searchTimer.done()){
        searchPattern();
        searchTimer.start();
      }
      
      int irFreq = receiveIR();
      int irAmp = determineAmplitude();
      
      if(irFreq > 1000 && irFreq < 10000 && !flash){  
        state = FOLLOWING; 
      }
      
      //bot.OLEDprint("Frequency: "+ String(irFreq),"Amplitude: " +String(irAmp),"");
      driving();
    }break;
    
    case FOLLOWING: {
      bot.OLEDprint("", "Following", "");
      //flashIR();
      
      if(!flash){
        adjustDirection(determineDirection());  
        if(determineOwnFreq()){
          state = LEADER;  
        }
      }
      //bot.OLEDprint("Frequency: "+ String(irFreq),"Amplitude: " +String(irAmp),"");
      int irFreq = receiveIR();
      int irAmp = determineAmplitude();
      if(!(irFreq > 1000 && irFreq < 10000)){    
        state = TARGET_LOST;
        targetLost.start();
      }
      driving();
    }break;
    
    case TARGET_LOST: {
      bot.OLEDprint("", "Target Lost", "");
      int irFreq = receiveIR();
      if(targetLost.done()){
        if(irFreq > 1000 && irFreq < 10000 && !flash){  
          state = FOLLOWING;    
        }else{
          state = SEARCHING;  
        }
      }
    }break;    
    
    case LEADER: {
      int irFreq = receiveIR();      
      currentDirection = BACKWARD;
      bot.OLEDprint("Leader","Own frequency: "+ String(ownFrequency),"Other Frequency: "+String(irFreq));
      //flashIR();
      driving();
    }break;
    
  }
}

int receiveIR(){
  int freqFound, ampFound;
  bot.ReceiveIR();
  for(int i = 0; i < 8; i++){
    amplitude[i] = bot.getAmplitude(i);
    frequency[i] = bot.getFrequency(i);
    if(frequency[i]>1000){
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

bool determineOwnFreq(){
  int readFreq = receiveIR();
  if(readFreq > 10000){
    readFreq = 10000;
  }
  if(ownFrequency == readFreq){
    //Change own frequency to 10000
    ownFrequency = 10000;
    return true;
  }else if(ownFrequency < readFreq){
    ownFrequency = readFreq - 10;
  }
}

int determineDirection(){
  int directionIndicator = 0;
  for(int i=0; i<8; i++){
    if((i==3|| i==4 ||i==5) && frequency[i]>1000){//right
      directionIndicator += -1;
     }else if(i == 6 && frequency[i]>1000){//backward
      directionIndicator += 1;
    }else if((i==0 ||i==1 || i == 7) && frequency[i]>1000){ //left
      directionIndicator += 1;
    }else if(i== 2 && frequency[i]>1000){//front
      directionIndicator += 0;
    }
  }
  return directionIndicator;    
}

int adjustDirection(int directionIndicator){
  if(directionIndicator>0){
    //bot.OLEDprint("", "Swarmy to the left", "");
    currentDirection = LEFT;
    return 0;
  }else if(directionIndicator<0){
    //bot.OLEDprint("","Swarmy to the right",""); 
    currentDirection = RIGHT;
    return 1;
  }else if(directionIndicator == 0){
    //bot.OLEDprint("", "Swarmy to the front", "");
    currentDirection = FORWARD;
    return 2;
  }
}

void searchPattern(){
  int randDirection = random(3);
  if(randDirection == 0){ 
    //bot.OLEDprint("", "Forward", "");
    currentDirection = FORWARD;
  }else if(randDirection == 1){
    //bot.OLEDprint("", "Forward Left", "");
    currentDirection = LEFT;
  }else if(randDirection == 3){
    //bot.OLEDprint("", "Right", "");
    currentDirection = RIGHT;
  }    
}

float determineSpeed(float measuredAmp){
  static float desiredAmp = 40;
  const float e = 2.71828;
  if(objectDistance < 10){
    currentDirection = BACKWARD;
    return 0.5; 
  }else if(measuredAmp != desiredAmp){
    float x = desiredAmp - measuredAmp;
    float power = pow(e, 0.04*x);
    float division = 1/(1+ power);
    float equation = 1 - (division * 2);
    return equation;
  }else if(measuredAmp == 0){
    return 1;
  }else {
    return 0;  
  }
}

void flashIR(){
  
  if(irTimer.done()){
    if(flashTrigger % modulusAmount){
      flash = true; 
      bot.setInfraredFrequency(2000);
       
    }else{
      flash = false;
      bot.setInfraredFrequency(0);
      
    }
      
    flashTrigger++;
    irTimer.start();
  }
}

int detectObject(){
  long echoDuration;
  if(objectTimer.done()){
    digitalWrite(trigPin, HIGH);
    objectTimer.start();
  }else{
    digitalWrite(trigPin, LOW);  
    echoDuration = pulseIn(echoPin, HIGH);
    objectDistance = echoDuration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
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
    
    case BACKWARD:
      bot.setMotorSpeed(0, determineSpeed(determineAmplitude())*100);
      bot.setMotorSpeed(1, determineSpeed(determineAmplitude())*100); 
    break;
  }
}
