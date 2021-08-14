
//Include Files
#include "secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
#endif
#include <ESPAsyncWebServer.h>
#include <Wire.h>
//Include Files

//Variables
float Cell1MinVoltage = 3.2;
float Cell2MinVoltage = 3.2;
float HighVoltageMaxCurrent =2.0;
float cell1voltage = 3.2;
float cell2voltage = 3.2;
float batterycurrent = 2.0;
float highvoltagecurrent= 2.0;
//Variables

//Functionproto
float inputtovalues(String inputParam,String inputMessage,float &Cell1MinVoltage,float &Cell2MinVoltage,float &HighVoltageMaxCurrent);
//Functionproto

// Placeholder to map cell values for HTML input
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
const char* PARAM_INPUT_4 = "input4";
const char* PARAM_INPUT_5 = "input5";
// Placeholder to map cell values for HTML input


//HTML with Script
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.15.1/css/all.css" integrity="998e14e9de" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>SINGABOAT's Power Delivery System</h2>
  <p>
    <i class="fas fa-car-battery" style="color:#059e8a;"></i><span class="labels">Cell 1 Voltage:</span>&nbsp;&nbsp;<span id="cell1volt">%CELL1VOLT%</span>&nbsp;&nbsp;&nbsp;&nbsp;<i class="fas fa-car-battery" style="color:#059e8a;"></i><span class="labels">Cell 2 Voltage:</span>&nbsp;&nbsp;<span id="cell2volt">%CELL2VOLT%</span>
  </p>
  <p>
    <i class="fas fa-tachometer-alt" style="color:#059e8a;"></i><span class="labels">Battery Current: </span>&nbsp;&nbsp;<span id="bcd">%BCD%</span>&nbsp;&nbsp;&nbsp;&nbsp;<i class="fas fa-tachometer-alt" style="color:#059e8a;"></i><span class="labels">High Voltage Current: </span>&nbsp;&nbsp;<span id="hcb">%HCB%</span>
  </p>
  <p>
  
    <i class="fas fa-bolt"style="color:#F7FF00;"></i><span class="labels">Cell 1 Min Voltage: </span><span id="c1mv">%C1MV%</span>
    <form action="/get">
    <input type="text" name="input1"><input type="submit" value="Submit">
    </form><br>
  </p>  
  <p>  
    <i class="fas fa-bolt"style="color:#F7FF00;"></i><span class="labels">Cell 2 Min Voltage: </span><span id="c2mv">%C2MV%</span>
    <form action="/get">
    <input type="text" name="input2"><input type="submit" value="Submit">
    </form><br>
  </p>
  <p>
    <i class="fas fa-bolt"style="color:#F7FF00;"/></i> <span class="labels">High Voltage Max Current: </span><span id="hvmc">%HVMC%</span>
    <form action="/get">
    <input type="text" name="input3"><input type="submit" value="Submit">
    </form><br>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("cell1volt").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/cell1volt", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("cell2volt").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/cell2volt", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("bcd").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/bcd", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("hcb").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/hcb", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("c1mv").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/c1mv", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("c2mv").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/c2mv", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("hvmc").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/hvmc", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)rawliteral";
//HTML with Script

// Replaces placeholder with values
String processor(const String& var){
  //Serial.println(var);
  if(var == "CELL1VOLT"){
    return String(cell1voltage);
  }
  else if(var == "CELL2VOLT"){
    return String(cell2voltage);
  }
  else if(var == "BCD"){
    return String(batterycurrent);
  }
  else if(var == "HCB"){
    return String(highvoltagecurrent);
  }
  else if(var == "C1MV"){
    return String(Cell1MinVoltage);
  }
  else if(var == "C2MV"){
    return String(Cell2MinVoltage);
  }
  else if(var == "HVMC"){
    return String(HighVoltageMaxCurrent);
  }
  return String();
}
// Replaces placeholder with values


AsyncWebServer server(80);


void setup() 
{
  Serial.begin(115200);
  Wire.begin(31, 32);
  //Wi-Fi Setup
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println(".");
  }
  Serial.println(WiFi.localIP());
  //Wi-Fi Setup





  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/cell1volt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(cell1voltage).c_str());
  });
  server.on("/cell2volt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(cell2voltage).c_str());
  });
  server.on("/bcd", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(batterycurrent).c_str());
  });
  server.on("/hcb", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(highvoltagecurrent).c_str());
  });
  server.on("/c1mv", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(Cell1MinVoltage).c_str());
  });
  server.on("/c2mv", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(Cell2MinVoltage).c_str());
  });
  server.on("/hvmc", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(HighVoltageMaxCurrent).c_str());
  });

  // Route for root / web page

  // Send a GET request to <ESP_IP>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    // Send a GET request to <ESP_IP>

    //Storing Input Values
    inputtovalues(inputParam,inputMessage,Cell1MinVoltage,Cell2MinVoltage,HighVoltageMaxCurrent);
    //Storing Input Values
    
    request->send_P(200, "text/html", index_html, processor);
    });

  // Start server
  server.begin();
}

void loop() 
{
  


}



float inputtovalues(String inputParam,String inputMessage,float &Cell1MinVoltage,float &Cell2MinVoltage,float &HighVoltageMaxCurrent)
{
    if (inputParam == PARAM_INPUT_1)
    {
    Cell1MinVoltage=inputMessage.toFloat();
    if(Cell1MinVoltage>50.0)
    {
      Cell1MinVoltage=50.0;
    }
    else if (Cell1MinVoltage<2.5)
    {
      Cell1MinVoltage=2.5;
    }
    Serial.println("PARAM_INPUT_1");
    Serial.println(Cell1MinVoltage);
    }

    
    else if(inputParam == PARAM_INPUT_2)
    {
    Cell2MinVoltage=inputMessage.toFloat();
    if(Cell2MinVoltage>50.0)
    {
      Cell2MinVoltage=50.0;
    }
    else if (Cell1MinVoltage<2.5)
    {
      Cell2MinVoltage=2.5;
    }
    Serial.println("PARAM_INPUT_2");
    Serial.println(Cell2MinVoltage);
    }


    
    else if(inputParam == PARAM_INPUT_3)
    {
    HighVoltageMaxCurrent=inputMessage.toFloat();
    if(HighVoltageMaxCurrent>6.0)
    {
      HighVoltageMaxCurrent=6.0;
    }
    else if (HighVoltageMaxCurrent<0.0)
    {
      HighVoltageMaxCurrent=0.0;
    }
    Serial.println("PARAM_INPUT_3");
    Serial.println(HighVoltageMaxCurrent);
    }
}
