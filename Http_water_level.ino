#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
//#include <WebServer.h>

#define RELAY_PIN    33 // ESP32 pin GIOP17 that connects to relay
#define MOISTURE_PIN 34 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor
int trigger_pin = 13;
int echo_pin   = 12;



// Replace with your network credentials
//const char* ssid = "Urbankisaan";
//const char* password = "kukatpally";
const char* ssid = "FD17 2_4";
const char* password = "chetan1702";

//WebServer server(80);

// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "https://plant-api-iot.herokuapp.com/addData";
// Service API Key
// String apiKey = "REPLACE_WITH_YOUR_API_KEY";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 seconds (10000)
unsigned long timerDelay = 10000;

//String page = "";
int distance_cm;
float tankPer;

void setup() {
  Serial.begin(115200);
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//  server.on("/", []() {
//    page = "<head><meta http-equiv=\"refresh\" content=\"3\"></head><center><h1>Ultasonic Water Level Monitor</h1><h3>Current water level:</h3> <h4>" + String(distance_cm) + "cm</h4></center>";
//    //page = "<foot><meta http-equiv=\"refresh\" content=\"4\"></head><center><h5>soil moiture Monitor</h5><h6>Moiture level:</h6> <h7>" + String(distance_cm) + "</h7></center>";
//    server.send(200, "text/html", page);
// 
//  
//  });
//  server.begin();
//  Serial.println("Web server started!");
}

void loop() {
  int value = analogRead(MOISTURE_PIN); // read the analog value from sensor

  if (value < THRESHOLD) {
    Serial.print("The soil is WET => turn pump OFF");
    digitalWrite(RELAY_PIN, LOW);
  } else {
    Serial.print("The soil is DRY => turn pump ON");
    digitalWrite(RELAY_PIN, HIGH);
  }

  

  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  long duration = pulseIn(echo_pin, HIGH);
  distance_cm = (duration / 2) / 29.09;
  Serial.println(distance_cm);

   if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);
      
      // Specify content-type header
      // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      //String httpRequestData = "api_key=" + apiKey + "&field1=" + String(random(40));           
      // Send HTTP POST request
      //int httpResponseCode = http.POST(httpRequestData);

      // If you need an HTTP request with a content type: application/json, use the following:
     // http.addHeader("Content-Type", "application/json");
     // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");
     //int httpResponseCode = http.POST("{\"api_key\":\Water_level\,\"MOISTURE_Value\":\"value\",\"TANK_Level\":\"tankPer\"}");

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");

      Serial.print(" MOISTURE_Value: ");
      String str_MoistureVal=String(value);
      Serial.println(str_MoistureVal);

      Serial.print("TANK_Level  %  : ");
      tankPer=distance_cm*3.3;
      String str_TankPer=String(distance_cm);
      Serial.println(str_TankPer);

      http.addHeader("Content-Type", "application/json");
      String moistureHeader = "{\"moisture\":\"";
      String moistureData = moistureHeader + str_MoistureVal;
      String levelHeader = "\",\"waterLevel\":\"";
      String levelData = levelHeader + str_TankPer;
      String end = "\"}";
      String levelDataEnd = levelData + end;
      String data =  moistureData + levelDataEnd ;
      Serial.println(data);
      int httpResponseCode = http.POST(data);

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
   }
//  server.handleClient();
  delay(2000);
}