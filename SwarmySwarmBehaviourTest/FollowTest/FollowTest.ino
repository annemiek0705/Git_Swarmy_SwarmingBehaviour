#include <config.h>
#include <Swarmy.h>

Swarmy bot(false);
typedef enum{Searching, Following}states;
states state = Searching;
int amplitude[8], frequency[8] ,foundFreq[8], initAmp[8];
bool swarmyFound = 0;
void setup() {
  // put your setup code here, to run once:
  bot.OLEDprint("", "Starting", "");
  Serial.begin(115200);
}

void loop() {
  switch(state){
    case Searching:
     bot.OLEDprint("", "Searching", "");
      if(receiveIR()==1000){
        if(swarmyFound == 0){
          swarmyFound = 1;
          state = Following;  
          bot.OLEDprint(String(frequency[0])+" " + String(frequency[1])+" "+ String(frequency[2]), String(frequency[3])+" "+ String(frequency[4])+ " "+String(frequency[5]),String(frequency[6])+" "+String(frequency[7]));
    
        }
      }
    break;
    
    case Following:
     // bot.OLEDprint("", "Following", "");
      adjustDirection(determineDirection());
      if(receiveIR()!=1000){
        swarmyFound = 0;
        state = Searching;  
      }
      //bot.OLEDprint(String(foundFreq[0])+" " + String(foundFreq[1])+" "+ String(foundFreq[2]), String(foundFreq[3])+" "+ String(foundFreq[4])+ " "+String(foundFreq[5]),String(foundFreq[6])+" "+String(foundFreq[7]));
          
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


int determineDirection(){
  int totalFreq = 0;
  for(int i = 0; i < 8;i++){
    Serial.println(foundFreq[i]);
    if(i > 0 && i < 4 && frequency[i]>500){//right
      totalFreq += -1;
    }else if(i == 4 && frequency[i]>500){//backward
      totalFreq += 10;
    }else if(i>4 && i<8 && frequency[i]>500){ //left
      totalFreq += 1;
    }else if(i ==0 && frequency[i]>500){//front
      totalFreq += 0;
    }
    
    Serial.println(totalFreq);
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
    bot.setMotorSpeed(0, 0);
    bot.setMotorSpeed(1, 0); 
  }
}
