/**
 * Author Miguel Covarrubias
 * SJSU - CMPE-286 Project
 * 
 * Uses Arduino with WiFi Module
 * */


#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>

char ssid[] = "xxxx";    // your network SSID (name)
char pass[] = "xxxx";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
//char server[] = "192.168.1.36:80";   // use to test in local network 

//WiFiSSLClient client;
WiFiClient client;
//IPAddress server(192,168,1,36); // use to test in local network 
IPAddress server(18,234,165,159);  // AWS web server

// hardcoded values below to represent out model
long availble_parking_spots = 0;

// Ultrasonic Sensor 1
const int s1_pingPin = 1; 
const int s1_echoPin = 2;
const int s1_redPin = 3;
const int s1_greenPin = 4;
long s1_available = 0; // 0 if not available, 1 if available

// Ultrasonic Sensor 2
const int s2_pingPin = 5; 
const int s2_echoPin = 6;
const int s2_redPin = 7;
const int s2_greenPin = 8;
long s2_available = 0; // 0 if not available, 1 if available

// Ultrasonic Sensor 3
const int s3_pingPin = 9; 
const int s3_echoPin = 10;
const int s3_redPin = 11;
const int s3_greenPin = 12;
long s3_available = 0; // 0 if not available, 1 if available

long s1_counter = 0;
long s2_counter = 0;
long s3_counter = 0;

void setup() {
  
   Serial.begin(9600); // Starting Serial Terminal
     while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  
   pinMode(s1_redPin, OUTPUT);
   pinMode(s1_greenPin, OUTPUT);
   pinMode(s2_redPin, OUTPUT);
   pinMode(s2_greenPin, OUTPUT);
   pinMode(s3_redPin, OUTPUT);
   pinMode(s3_greenPin, OUTPUT);
}

void printUltrasonicSensorInfo(char sensorName[], int parkingSpot, int pingPin, int echoPin, int redPin, int greenPin, long counter, long& isAvailableAsInt) {
   
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   
   if(cm < 7) {
    counter = counter + 1;
   } else {
    counter = 0;
   }

   Serial.print("COUNTER: ");
   Serial.println(s1_available);

   if(counter >= 5) {
     isAvailableAsInt = 0;
     availble_parking_spots = s1_available + s2_available + s3_available; //getAvailableParkingSpots();
     sendStatusChange("Garage1", parkingSpot, false, availble_parking_spots);
     digitalWrite(redPin, HIGH); 
     digitalWrite(greenPin, LOW);   
   } else {
     isAvailableAsInt = 1;
     availble_parking_spots = s1_available + s2_available + s3_available; //getAvailableParkingSpots();
     sendStatusChange("Garage1", parkingSpot, true, availble_parking_spots);
     digitalWrite(redPin, LOW); 
     digitalWrite(greenPin, HIGH);  
   }
   
   Serial.print(sensorName);
   Serial.print(" -> ");
   Serial.print(inches);
   Serial.print("in, ");
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
}

void loop() {
  printUltrasonicSensorInfo("ULTRASONIC_1", 1, s1_pingPin, s1_echoPin, s1_redPin, s1_greenPin, &s1_counter, s1_available);
  printUltrasonicSensorInfo("ULTRASONIC_2", 2, s2_pingPin, s2_echoPin, s2_redPin, s2_greenPin, &s2_counter, s2_available);
  printUltrasonicSensorInfo("ULTRASONIC_3", 3, s3_pingPin, s3_echoPin, s3_redPin, s3_greenPin, &s3_counter, s3_available);
  delay(2000); // 
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

// send status change about the parking spots to the web server
void sendStatusChange(String parkingId, long spotId, boolean isAvailable, long availableSpots) {
    // Reserve memory space for your JSON data
  StaticJsonDocument<200> doc;
  doc["parkingId"] = parkingId;
  doc["spotId"] = spotId;
  doc["isAvailable"] = isAvailable;
  doc["totalAvailableSpots"] = availableSpots;

  // Generate the minified JSON and send it to the Serial port.
  Serial.println("Payload content: ");
  serializeJsonPretty(doc, Serial);
  Serial.println();
  Serial.print("Payload size: ");
  Serial.println(measureJsonPretty(doc));
  Serial.println();

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    // Make a HTTP request:
    Serial.println("connected to server");
    client.println("POST /api/updatespot HTTP/1.1");
    client.println("Host: 192.168.1.36:80"); // AWS web server
    //client.println("Host: 192.168.1.36:80"); // use to test in local network 
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(measureJsonPretty(doc));
    client.println();
    serializeJsonPretty(doc, client);

  } else {
      Serial.println("NOT connected to server");
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
