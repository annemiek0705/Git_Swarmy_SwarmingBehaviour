#include <config.h>
#include <Swarmy.h>

Swarmy bot(false);
typedef enum{Searching, Following}states;
states state = Searching;
int amplitude[8], frequency[8] ,initFreq[8], initAmp[8];
bool arrayCopied = 0;
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
        if(arrayCopied == 0){
          copyArray();
          arrayCopied = 1;
          bot.OLEDprint(String(initFreq[0])+" " + String(initFreq[1])+" "+ String(initFreq[2]), String(initFreq[3])+" "+ String(initFreq[4])+ " "+String(initFreq[5]),String(initFreq[6])+" "+String(initFreq[7]));
          state = Following;  
        }
      }
    break;
    
    case Following:
      bot.OLEDprint("", "Following", "");
      if(receiveIR()!=1000){
        arrayCopied = 0;
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
void copyArray(){
  for(int i; i<7; i++){
    initFreq[i] = frequency[i];
    initAmp[i] = amplitude[i];
  }
}

void determineDirection(){
    
}
