/*
請上傳此程式碼至esp8266
*/

#include<SoftwareSerial.h>
#include<ESP8266WiFi.h>
SoftwareSerial d1mini(D2,D3);

const char* ssid     = "fan";
const char* password = "3psdvkj28bdb";

WiFiServer server(80);
String header;

IPAddress local_IP(172,20,10,2);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

void setup(){
  Serial.begin(9600);
  d1mini.begin(4800);
  pinMode(D2,INPUT);
  pinMode(D3,OUTPUT);

  pinMode(LED_BUILTIN,OUTPUT);

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  IPAddress IP = WiFi.localIP();
  Serial.println("IP address: ");
  Serial.println(IP);
  server.begin();
}
void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /off") >= 0) {
              Serial.println("digitalWrite(LED_BUILTIN, HIGH)");
              digitalWrite(LED_BUILTIN, HIGH);
            } else if (header.indexOf("GET /on") >= 0) {
              Serial.println("digitalWrite(LED_BUILTIN, LOW)");
              digitalWrite(LED_BUILTIN, LOW);
            } else if (header.indexOf("GET /one") >= 0) {
              d1mini.print("1");
              d1mini.println("\n");
              delay(30);
            } else if (header.indexOf("GET /two") >= 0) {
              d1mini.print("2");
              d1mini.println("\n");
              delay(30);
            } else if (header.indexOf("GET /three") >= 0) {
              d1mini.print("3");
              d1mini.println("\n");
              delay(30);
            } 
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

}