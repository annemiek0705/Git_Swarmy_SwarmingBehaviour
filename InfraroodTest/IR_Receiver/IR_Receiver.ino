#include <config.h>
#include <Swarmy.h>
#include <HttpClient.h>
#include <WiFi.h>;
#include <WiFiClient.h>
#include <ArduinoJson.h>

const char* ssid = "Mikky";
const char* password = "987612345";
typedef enum{Left, Right, Front, Back, Circle, Stop}states;
WiFiClient wifiClient;
HttpClient http = HttpClient(wifiClient, "192.168.43.46", 8000);

Swarmy bot(false);
int amplitude[8];
int frequency[8];
int i;

void setup() {
  // put your setup code here, to run once:
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
  bot.OLEDprint("", "starting scanning", "");
}

void postRequest(String title){
  if ( WiFi.status() == WL_CONNECTED) {
    String contentType = "application/json";
    DynamicJsonDocument doc(1024);
    doc["amp0"] = String(amplitude[0]);   doc["amp1"] = String(amplitude[1]);   doc["amp2"] = String(amplitude[2]);   doc["amp3"] = String(amplitude[3]); 
    doc["amp4"] = String(amplitude[4]);   doc["amp5"] = String(amplitude[5]);   doc["amp6"] = String(amplitude[6]);   doc["amp7"] = String(amplitude[7]);
    doc["freq0"] = String(frequency[0]);  doc["freq1"] = String(frequency[1]);  doc["freq2"] = String(frequency[2]);  doc["freq3"] = String(frequency[3]); 
    doc["freq4"] = String(frequency[4]);  doc["freq5"] = String(frequency[5]);  doc["freq6"] = String(frequency[6]);  doc["freq7"] = String(frequency[7]);
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
  // put your main code here, to run repeatedly:
  bot.OLEDprint("", "Receive IR", "");
  bot.ReceiveIR();
  bot.OLEDprint("", "get amplitude", "");
  for(i = 0; i < 8; i++){
    amplitude[i] = bot.getAmplitude(i);
    frequency[i] = bot.getFrequency(i);
  }
  String firstStr = String(amplitude[0])+ ":"+String(frequency[0])+ " "+String(amplitude[1])+ ":"+String(frequency[1])+" "+ String(amplitude[2])+":"+String(frequency[2]);
  String secondStr = String(amplitude[3])+":"+String(frequency[3])+" "+String(amplitude[4])+":"+String(frequency[4])+" "+String(amplitude[5])+":"+String(frequency[5]);
  String thirdStr = String(amplitude[6])+":"+String(frequency[6])+ " "+String(amplitude[7])+":"+String(frequency[7]);
  bot.OLEDprint(firstStr, secondStr, thirdStr);
  postRequest("");
  //delay(1000);
}
