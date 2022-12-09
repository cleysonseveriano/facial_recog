#include <WiFi.h> //built-in with arduino
#include <HTTPClient.h> //built-in with arduino
#include <Arduino_JSON.h> //https://github.com/arduino-libraries/Arduino_JSON.git

#define RED_LED 

const char* ssid = "Cleyson";
const char* password = "meuwifi22";

String host = "http://172.20.10.13";
String port = "3000";
String url = host + ":" + port;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

String openTo;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED){
      openTo = httpGETRequest((url + "/openTo").c_str());

      JSONVar model = JSON.parse(openTo);
  
      for (int i = 0; i < model.length(); i++) {
        JSONVar name = model[i];
        Serial.print("Liberando catraca para:");
        Serial.println(name);
        httpPOSTRequest((url + "/register").c_str(), name);
        blinkLed();
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);

  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  return payload;
}

void httpPOSTRequest(const char* serverName, String name) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverName);
  
  http.addHeader("Content-Type", "application/json");

  String body = "{ \"name\": \"" + name + "\"}";

  int httpResponseCode = http.POST(body);
  
  http.end();
}

void blinkLed() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}