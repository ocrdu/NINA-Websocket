#include <WiFiNINA.h>
#include <WebSocketServer.h>
#include <RTCZero.h>
#include <Base64.h>
#include "wifi_secrets.h" //Put your WiFi SSID and password in this file

#define DEBUG 1

#ifdef DEBUG
  #define Sprint(a) (Serial.print(a))
  #define Sprintln(a) (Serial.println(a))
#else
  #define Sprint(a)
  #define Sprintln(a)
#endif

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

const int IPLastByte = 99; //The last byte of the IP address the Arduino will have in your network
WiFiServer server(80); //Normal HTTP server for webpage
WiFiServer socketServer(8080); //Websocket server on port 8080
WebSocketServer webSocketServer;
WiFiClient client;
WiFiClient socketClient;
RTCZero rtc;
unsigned long epoch;
unsigned long previousSendMillis = 0;
unsigned long previousClockMillis = 0;

//The clock interface web page, gzipped and base64 encoded
char webpage_base64[] = "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";

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  pinMode(LED_BUILTIN, OUTPUT);
  rtc.begin();
  WiFiConnect();
}
 
void loop() {

  //Try to reconnect WiFi if connection was lost
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    Sprintln("\n--Lost WiFi connection");
    WiFi.end();
    WiFiConnect();
  }
  
  //This part serves the html page with the clock to the browser.
  //The webpage was gzipped and then base64-encoded, and will be served as a gz file.
  client = server.available();
  if (client) {
    Sprintln("\n--New client");
    webSocketServer.disconnectStream();
    socketClient.stop();
    Sprintln("--Websocket disconnected");
    String header = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\r') {
          header += c;
        }
        if (header.substring(header.length() - 2) == "\n\n") {
          Sprint(header.substring(0, header.length() - 1));
          if (header.indexOf("GET / HTTP") > -1) {
            client.println("HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Encoding: gzip\n");
            const int webpage_base64_length = sizeof(webpage_base64);
            const int webpage_gz_length = base64_dec_len(webpage_base64, webpage_base64_length);
            char webpage_gz[webpage_gz_length];
            base64_decode(webpage_gz, webpage_base64, webpage_base64_length);
            int packetsize = 1024;
            int done = 0;
            while (webpage_gz_length > done) {
              client.write(webpage_gz + done, packetsize * sizeof(char));
              done = done + packetsize;
              if (webpage_gz_length - done < packetsize) {
                packetsize = webpage_gz_length - done;
              }
            }
            client.stop();
            Sprintln("--Interface webpage sent; client disconnected");
            delay(1000);
          } else if (header.indexOf("GET /author HTTP") > -1) {
            client.println("HTTP/1.1 200 OK\nContent-Type: text/plain; charset=utf-8\n\nOscar den Uijl, oscar@den-uijl.nl\n");
            client.stop();
            Sprintln("\n--Email address sent; client disconnected");
            } else {
            client.println("HTTP/1.1 404 Not Found\nContent-Type: text/plain; charset=utf-8\n\n404 Not Found\n");
            client.stop();
            Sprintln("\n--Page not found; client disconnected");
          }
        }
      }
    }
  }

  //This part connects the websocket when the HTML page asks for it, and send data through the socket
  if (!socketClient.connected()) {
    socketClient = socketServer.available();
    if (webSocketServer.handshake(socketClient)) {
      Sprintln("\n--Websocket connected");
    } else {
      Sprintln("\n--Couldn't connect websocket");
    }
  } else {
    unsigned long currentSendMillis = millis();
    if ((currentSendMillis - previousSendMillis) > 1000) {
      previousSendMillis = currentSendMillis;
      webSocketServer.sendData("time:" + String(rtc.getEpoch()));
    } 
  }
    
  //This part adjusts the RTC every five minutes
  unsigned long currentClockMillis = millis();
  if ((currentClockMillis - previousClockMillis) > 300000) {
    previousClockMillis = currentClockMillis;
    epoch = WiFi.getTime();
    if (epoch > 0) {
      rtc.setEpoch(epoch);
      Sprintln("\n--RTC adjusted");
    }
  }
}

void WiFiConnect() {
  WiFi.setHostname("nano-webserver");
  while (WiFi.status() != WL_CONNECTED) {
    Sprint("Connecting to ");
    Sprint(ssid);
    Sprintln(" ...");
    WiFi.begin(ssid, pass);
    delay(5000);
  }
  IPAddress IP = WiFi.localIP();
  IP[3] = IPLastByte;
  WiFi.config(IP, WiFi.gatewayIP(), WiFi.gatewayIP(), WiFi.subnetMask());
  Sprint("Connected to ");
  Sprintln(ssid);
  int tries = 5;
  do {
    Sprintln("Getting NTP time ...");
    epoch = WiFi.getTime();
    tries--;
    delay(500);
  } while (epoch == 0 && tries > 0);
  if (tries == 0) {
    Sprintln("Failed to get NTP time");
  } else {
    rtc.setEpoch(epoch);
    Sprintln("RTC set to NTP time");
  }
  server.begin();
  socketServer.begin();
  #ifdef DEBUG
  printWifiStatus();
  #endif
  WiFi.lowPowerMode();
  digitalWrite(LED_BUILTIN, HIGH);
}

#ifdef DEBUG
void printWifiStatus() {
  Sprint("SSID: ");
  Sprintln(WiFi.SSID());
  Sprint("Signal strength (RSSI): ");
  Sprint(WiFi.RSSI());
  Sprintln(" dBm");
  Sprint("IP address: ");
  Sprintln(WiFi.localIP());
  Sprint("Gateway: ");
  Sprintln(WiFi.gatewayIP());
  Sprint("Netmask: ");
  Sprintln(WiFi.subnetMask());
  Sprint("Webserver is at http://");
  Sprint(WiFi.localIP());
  Sprintln("/\n");
}
#endif
