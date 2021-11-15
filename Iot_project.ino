


int flag=0;
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.


int i;
char msg1[75]; // char array for outgoing topic
char msg2[75];
const char* ssid = "TP-LINK_C78680";
const char* password = "0342235304";
const char* COUNTRY = "country";
const char* CITY = "city";

String Cityname;

String inputMessage1;
String inputParam1;
String inputMessage2;
String inputParam2;
const char* mqtt_server = "test.mosquitto.org";


WiFiClient espClient;
AsyncWebServer server(80);
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;





void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}




//while connected
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outgoingTopic", "Node MCU is now connected to Node-Red");
      // ... and resubscribe
      client.subscribe("incommingTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}





  
  


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //int value=analogRead(A0);
  int value=0;
  flag=0;
  if(Serial.available()){
   value=Serial.parseInt();
   //if(value<threshold)
    //flag=1;
  }
  delay(5000);
}



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="D:/mysem4_ ACA/IoT/IOT_PROJECT/Iot_project/IOT/Iot_project/air.css">
    <title>Air Quality Forcasting System</title>
</head>
<style>
.frm input{
    display: block;
    width: 98%;
    height: 100%;
    padding: 10px;
    border: 1px;
    border-radius: 10px;
}
.frm input[type="submit"]:hover{
    background-color: black;
    color: white;
}
body{
    background-image: url("https://cdn.pixabay.com/photo/2019/07/20/10/45/power-station-4350592_960_720.jpg");
    background-size: 1600px 1600px;
    background-repeat: no-repeat;
    background-attachment: fixed;
    background-position: center;
}
h2{
    font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;
}
h3{
    font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;
}
.frm input[type="submit"] {
    background-color:gray ;
    cursor: pointer;
    width: 99%;
    height: 100%;
    padding: 10px;
    font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;
}
h1{
    font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;
}
p{
    border: white 2px solid;
    border-radius: 10px;
    padding: 10px;
    font-family: 'Lucida Sans', 'Lucida Sans Regular', 'Lucida Grande', 'Lucida Sans Unicode', Geneva, Verdana, sans-serif;
}
</style>
<body>

   <center><h1>Air Quality Forcasting System</h1></center>
   <br><br><br>
   <form class="frm" action="/get">
   <div class="qwerty1">
   <b>
   <h2><label>Enter Your Country</label></h2>
   <input type="text"  name="country" placeholder="Eg:-Sri Lanka" required>
   </div>
   <br>
   <div class="qwerty2">
   <b>
    <h2><label>Enter Your City</label></h2>
    <input type="text"  name="city" placeholder="Eg:-Delgoda" required>
    <h2><input type="submit" value="Submit"></h2>
    </div>
    <br>
    </form>
<br><br>
    
        <hr>
        <h3>For All the Information Please Visit</h3>
        <a href="https://air-pollution-dashboard.mybluemix.net/ui/#!/1?socketid=P_rNhl52mUi8fb-eAAAK"> node-red dash board</a>
        <hr>
        <h3><i>Copyright 2021</i></h3>
</body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void setup() {
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
  WiFi.mode(WIFI_STA);

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputParam1;
    String inputMessage2;
    String inputParam2;
    if (request->hasParam(COUNTRY)&& request->hasParam(CITY)) {
      inputMessage1 = request->getParam(COUNTRY)->value();    
      inputParam1 = COUNTRY;                                
      inputMessage2 = request->getParam(CITY)->value();    
      inputParam2 = CITY;                                   
    }
    else {
      inputMessage1 = "No message sent";
      inputParam1 = "none";
    }
    //Cityname = inputMessage2;
    //Countryname = inputMessage1;    
    Serial.println(inputMessage1);
    Serial.println(inputMessage2);

    if (request->hasParam(COUNTRY)&& request->hasParam(CITY)) {
      inputMessage1 = request->getParam(COUNTRY)->value();
      String Countryname = inputMessage1;
      Countryname.toCharArray(msg1,75);            
      client.publish("outgoingTopic1", msg1);
      

      inputMessage2 = request->getParam(CITY)->value();
      String Cityname = inputMessage2;
      Cityname.toCharArray(msg2,75);            
      client.publish("outgoingTopic2", msg2);
      
      
      
      }
       
    
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam1 + "  " + inputParam2 + ") with value: " + inputMessage1 + " " + inputMessage2 +
                                     "<br><a href=\"/\">Return to Home Page</a>"); 
  });
  server.onNotFound(notFound);
  server.begin();

}  





  
