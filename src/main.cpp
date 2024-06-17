#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "Password";

#define DHTPIN 4     // Digital pin connected to the DHT sensor

//DHT
#define DHTTYPE    DHT11     

DHT dht(DHTPIN, DHTTYPE);
//relay
int ison=0, rpin=33;
//servo
Servo elservo;
int pinservo=14;
int forcedopen;
float maxtemp=30.0;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}
String rman(){
  if (ison==1){
    ison=0;
    return "off";
  }
  else if (ison==0)
  {
    ison=1;
    return "on";
  }
  return "unk";
}
String servomanage(int type){
  //type 1 cuando es para forzar el activado/desactivado
  if (type==1){
    if (forcedopen==1){
      forcedopen=0;
    }
    else if(forcedopen==0){
      forcedopen=1;
    }
  }
  //type 0 para escribir en el servo
  else if(type==0){
    if (forcedopen==1){
      elservo.write(135);
    }
    else if(forcedopen==0){
      if(dht.readTemperature()>=maxtemp&& dht.readTemperature()!=0){
        elservo.write(135);
      }
      else elservo.write(0);
    }
  }
  return String(forcedopen);
}
String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
} 
String inicio_sesion(String uname, String pwd){
  String defus="admin";
  String defcon="admin"; 
  Serial.println(uname);
  Serial.println(pwd);
  if (uname==defus){
    if(pwd==defcon){
      Serial.println("yes");
      return "yes";
    }
  }
  else{
    return "no";
  }
  return "no";
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(rpin, OUTPUT);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    //change "host" whit the ip of the computer hosting the dashboard
    request->redirect("http://host/?ip="+ WiFi.localIP().toString());
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/inicio", HTTP_POST, [](AsyncWebServerRequest *request){
    AsyncWebParameter* uname= request->getParam(0);
    AsyncWebParameter* pass= request->getParam(1);
    String E=inicio_sesion(uname->value().c_str(), pass->value().c_str()).c_str();

    request->send_P(200, "text/plain",E.c_str());
  });
  server.on("/int",HTTP_POST, [](AsyncWebServerRequest *request){  request->send_P(200,"text/plain", rman().c_str());});
  server.on("/ven",HTTP_POST, [](AsyncWebServerRequest *request){  request->send_P(200,"text/plain", servomanage(1).c_str());});
  server.on("/mtemp",HTTP_POST, [](AsyncWebServerRequest *request){
    AsyncWebParameter* p1= request->getParam(0);
    maxtemp=static_cast<float>(atoi(p1->value().c_str()));
    Serial.printf("\nmaxtemp= %f\n", maxtemp);
    request->send_P(200, "text/plain", "ok");
  });
  // Start server
  server.begin();
  elservo.attach(pinservo,500,2500);
}
 
void loop(){
        if (ison==1){
          digitalWrite(rpin, LOW);
        }
        else if (ison==0)
        {
          digitalWrite(rpin, HIGH);
        }
        servomanage(0);
}