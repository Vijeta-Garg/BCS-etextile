#include <Wire.h>
#include "Adafruit_DRV2605.h"
#include <WiFi.h>
#include <WebServer.h>  // Or use <ESPAsyncWebServer.h> for async


// Replace with your network credentials
const char* ssid = "Idontknowwhattonamethis001";
const char* password = "VG011@gmail.com";

// Create the web server on port 80
WebServer server(80);



#define PCAADDR 0x70
const int regionOne = 18; // A0 maps to GPIO1 on QT Py ESP32-S3   
int regionOnePain = 0;
int regionTwoPain = 0;
int regionThreePain = 0;
int regionFourPain = 0; 
int swelling = 0; 
unsigned long lastUpdate = 0;


const int regionTwo = 9;
const int regionThree = 8;
const int regionFour = 37; 
const int lymphaticVersion = 17; 

 void handleRoot() {
  String html = "<html><body><h1>Seamless Monitoring for Medical Professionals</h1>";
  html += "<p>Region one pain (intercostal muscles):" + String(regionOnePain) + "</p>";
  html += "<p>Region two pain (pectoralis major):" + String(regionTwoPain) + "</p>";
  html += "<p>Region three pain (axilal muscle):" + String(regionThreePain) + "</p>";
  html += "<p>Region four pain (shoulder):" + String(regionFourPain) + "</p>";
    html += "<p>Swelling along left region: " + String(swelling) + "</p>";


  html += "</body></html>";
  server.send(200, "text/html", html);
}


void pcaselect(uint8_t i) {
  if (i > 3) return;
 
  Wire.beginTransmission(PCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


// standard Arduino setup()


Adafruit_DRV2605 drv;

uint8_t oncoEffect = 1;
uint8_t lymphEffect = 1;



void setup() {
  //creates connection with multiplexer channels 
   while (!Serial);
    delay(1000);
    Wire.begin(41,40);

    
    Serial.begin(115200);
    WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Setup web server routes
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");

    Serial.println("\nPCAScanner ready!");

      //setting up pin modes: 
  pinMode(regionOne, INPUT_PULLUP); // Enable internal pull-up
  pinMode(regionTwo, INPUT_PULLUP); // Enable internal pull-up
  pinMode(regionThree, INPUT_PULLUP); // Enable internal pull-up
  pinMode(regionFour, INPUT_PULLUP); // Enable internal pull-up
  pinMode(lymphaticVersion, INPUT_PULLUP); // Enable internal pull-up

}





void oncologicalMassage(){
  Serial.print("Effect #"); Serial.println(oncoEffect);


  if (oncoEffect == 1) {
    Serial.println(F("7 − Soft Bump - 100%"));
    drv.setWaveform(0, 7);  // play effect 
  drv.setWaveform(1, 0);  
  }
  
  if (oncoEffect == 2) {
    Serial.println(F("13 − Soft Fuzz - 60%"));
    drv.setWaveform(0, 13);  // play effect 
  drv.setWaveform(1, 0);       // end waveform
  }

  // play the effect!
  drv.go();


  oncoEffect++;
  if (oncoEffect > 2) oncoEffect = 1;
  delay(1000);

}

void lymphaticMassage(){
   Serial.print("Effect #"); Serial.println(lymphEffect);


  if (lymphEffect == 1) {
    Serial.println(F("13 − Soft fuzz - 100%"));
    drv.setWaveform(0, 13);  // play effect 
  drv.setWaveform(1, 0);  
  }
  
  if (lymphEffect == 2) {
    Serial.println(F("71 − transition ramp - 60%"));
    drv.setWaveform(0, 71);  // play effect 
  drv.setWaveform(1, 0);       // end waveform
  }

  // play the effect!
  drv.go();


  lymphEffect++;
  if (lymphEffect > 2) lymphEffect = 1;
  delay(1000);
}

  void settingupMotorController(uint8_t port) {
  pcaselect(port);
  delay(10); // Let the I2C switch settle
   Serial.print("PCA Port #"); Serial.println(port);

  if (!drv.begin()) {
    Serial.print("Could not find DRV2605 on port ");
    Serial.println(port);
    return; // don’t halt, just skip
  }

  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);
}




void loop() {
  //defining states
  int stateOne = digitalRead(regionOne);
  int stateTwo = digitalRead(regionTwo);
  int stateThree = digitalRead(regionThree);
  int stateFour = digitalRead(regionFour);
  int lymphState = digitalRead(lymphaticVersion); 


if (stateOne == LOW) {
    Serial.println("Switch pressed!");
    // Do your task

    //connection with motor drivers 
    settingupMotorController(0);
 
    oncologicalMassage();

     regionOnePain++;



  } else if(stateTwo == LOW) {
    Serial.println("Switch pressed!");
    // Do your task

    settingupMotorController(1);
    oncologicalMassage();

     regionTwoPain++;

  } else if(stateThree == LOW) {
    Serial.println("Switch pressed!");
    // Do your task

        settingupMotorController(2);

    oncologicalMassage();

     regionThreePain++; 

  } else if(stateFour == LOW){
    Serial.println("Switch pressed!");
    // Do your task
   
        settingupMotorController(3);
         oncologicalMassage();

         regionFourPain++;

      
  } 
  else if(lymphState == LOW){ 
    Serial.println("lymph mode");

  //put into a while loop??
    for(int i = 0; i < 4; i++){
       settingupMotorController(i);
        lymphaticMassage();

        delay(700);
    }
 
    swelling++;
    // Serial.println("Counter: " + String(counter));
  }

    //add a timer 
  server.handleClient(); // Must be called frequently


  }


   // Increment the counter every second
  
    //write in boolean logic to make something "stop" !!!!
    //add in web update functionality 
    //make code prettier perhaps by using a switch command? 
  
