#include <config.h>
#include <Swarmy.h>
#include <HttpClient.h>
#include <WiFi.h>;
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char* ssid = "Mikky";
const char* password = "annemiekfokke";
typedef enum{Left, Right, Front, Back, Circle, Stop, Drive}states;
WiFiClient wifiClient;
HttpClient http = HttpClient(wifiClient, "192.168.43.46", 8000);

Swarmy bot(false);
states movement;

void setup() {
  bot.OLEDprint("", "Waking up", "");
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to wifi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(":(");
    delay(500);
    bot.OLEDprint("No WiFi connection", "", "");
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("IP adress: ");
  Serial.println(WiFi.localIP());
  bot.OLEDprint("", "Waiting for request", "");
}



void postRequest(String title){
  if ( WiFi.status() == WL_CONNECTED) {
    String contentType = "application/json";
    DynamicJsonDocument doc(1024);
    doc["title"] = title;
    String jsonString;
    serializeJson(doc, jsonString);
    http.post("/", contentType, jsonString);

    String payload = http.responseBody();
    //Serial.println("\nStatusCode: " + String(httpCode));
    Serial.println(payload);

  } else {
    Serial.println("Connection Lost");
  }

  delay(1000);
}

String getRequest(){
  if ( WiFi.status() == WL_CONNECTED) {
    int httpCode = http.get("/");
    String payload = http.responseBody();
    Serial.println("\nStatusCode: " + String(httpCode));
    Serial.println(payload);
    DynamicJsonDocument doc(1040);
    deserializeJson(doc, payload);
    String Movement = doc["movement"];
    Serial.println("Deserialized string: "+Movement);
    return Movement;
  } else {
    Serial.println("Connection Lost");
  }

  delay(1000);
}
void loop() {
  ProcesData(getRequest());
  delay(500);
}

void ProcesData(String msg){
  states moving;
  if(msg == "L"){moving = Left;}
  if(msg == "R"){moving = Right;}
  if(msg == "C"){moving = Circle;}
  if(msg == "F"){moving = Front;}
  if(msg == "B"){moving = Back;}
  updateVariables(moving);
  
}

void AdjustMovement(int speedA, int speedB, String dir, int waitTime){
  bot.OLEDprint("", dir, "");
  bot.setMotorSpeed(0, speedA);
  bot.setMotorSpeed(1, speedB);
  delay(waitTime);
}

void updateVariables(states movement){
  int speedA, speedB; 
  bool turned = false;
  String dir; 
  int waitTime = 500;
  switch(movement){
    case Left:
      dir = "Turning Left";
      speedA = 50;
      speedB = 100;
      turned  = 1;
       if(turned = 1){
       movement = Front;
       waitTime = 50;
       turned  = 0;
      }
      AdjustMovement(speedA, speedB, dir, waitTime);
    break;
      
    case Right:
      dir = "Turning Left";
      speedA = 100;
      speedB = 50;
      turned  = 1;
       if(turned = 1){
       movement = Front;
       waitTime = 50;
       turned  = 0;
      }
      AdjustMovement(speedA, speedB, dir, waitTime);
    break;
    
    case Front:
      dir = "Driving Forward";
      speedA = 100;
      speedB = 100;
       if(turned = 1){
       movement = Front;
       waitTime = 50;
       turned  = 0;
      }
      AdjustMovement(speedA, speedB, dir, waitTime);
    break;
    
    case Back: 
      dir = "Driving backwards";
      speedA = -100;
      speedB = -100;
       if(turned = 1){
       movement = Front;
       waitTime = 50;
       turned  = 0;
      }
      AdjustMovement(speedA, speedB, dir, waitTime);
    break;
    
    case Circle:
      dir = "Driving in circle";
      speedA = 0;
      speedB = 100;
      movement = Drive;
    break;
    case Stop:
      dir = "Stopped Driving";
      speedA = 0;
      speedB = 0;
       if(turned = 1){
       movement = Front;
       waitTime = 50;
       turned  = 0;
      }
      AdjustMovement(speedA, speedB, dir, waitTime);
    break; 
    case Drive:
    bot.OLEDprint("", "trying to drive", "");
      if(turned = 1){
       movement = Front;
       waitTime = 50;
       turned  = 0;
      }
      AdjustMovement(speedA, speedB, dir, waitTime);
    break;
  } 
}
