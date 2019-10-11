#include <WiFiNINA.h>
#include <WebSocketServer.h>
#include <Base64.h>
#include "wifi_secrets.h"

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
const int IPLastByte = 99;
const int socketPort = 8080;
const int maxClients = 5;
WiFiServer socketServer(socketPort);
WiFiClient *clients[maxClients];
WebSocketServer *webSocketServers[maxClients];
const int ledPin = 12;
int ledState = 0;
unsigned long previousMillis = 0;

void setup() {
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  WiFiConnect();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    Sprintln("\n--Lost WiFi connection");
    WiFi.end();
    WiFiConnect();
  }

  WiFiClient newClient = socketServer.available();
  int openSlot = -1;
  if (newClient) {
    Sprintln("\n--New client");
    
    for (int i=0; i<maxClients; i++) {
      if (clients[i] && clients[i]->connected() && clients[i]->remoteIP() == newClient.remoteIP() && clients[i]->remotePort() == newClient.remotePort()) {
        Sprintln("  Already have this client as clients[" + (String)i + "]");
        String message;
        while (newClient.available()) {
          message += newClient.read();
          if (message.length() > 32) {
            break;
          }
        }
        Sprintln("  Received \"" + message + "\"");
        newClient.stop();
        clients[i]->stop();
        delete webSocketServers[i]; 
        Sprintln("--Message ignored; clients[" + (String)i + "] stopped");
        return;
      }
    }
    
    for (int i=0; i<maxClients; i++) {
      if (!clients[i] || !clients[i]->connected()) {
        openSlot = i;
        clients[openSlot] = new WiFiClient(newClient);
        Sprintln("  New client stored in clients[" + (String)openSlot + "]");
        break;
      }
    }
    
    if (openSlot > -1) {
      webSocketServers[openSlot] = new WebSocketServer(clients[openSlot]);
      if (webSocketServers[openSlot]->handshake()) {
        Sprintln("  Websocket handshake by webSocketServers[" + (String)openSlot + "] successfull");
        Sprint("--Websocket connected to new client at "); Sprint(newClient.remoteIP()); Sprint(":"); Sprintln(newClient.remotePort());
      } else {
        Sprintln("--Couldn't connect client");
        newClient.stop();
        clients[openSlot]->stop();
        delete webSocketServers[openSlot];
      }
    } else {
      newClient.println("HTTP/1.1 503 Unavailable\nContent-Type: text/plain; charset=utf-8\n\n503 Unavailable\n");
      newClient.stop();
      Sprintln("--New client rejected: too many clients");
    }
  }

  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) > 1000) {
    boolean oneIsActive = false;
    previousMillis = currentMillis;
    for (int i=0; i<maxClients; i++) {
      if (clients[i] && clients[i]->connected()) {
        webSocketServers[i]->sendData("Hartbeat for client[" + (String)i + "]");
      }
    }
  }

}

void WiFiConnect() {
  WiFi.setHostname("nano-webserver");
  while (WiFi.status() != WL_CONNECTED) {
    Sprintln("Connecting to " + (String)ssid + " ...");
    WiFi.begin(ssid, pass);
    delay(5000);
  }
  IPAddress IP = WiFi.localIP();
  IP[3] = IPLastByte;
  WiFi.config(IP, WiFi.gatewayIP(), WiFi.gatewayIP(), WiFi.subnetMask());
  Sprintln("Connected to " + (String)ssid);
  socketServer.begin();
  #ifdef DEBUG
  printWifiStatus();
  #endif
  WiFi.lowPowerMode();
  digitalWrite(LED_BUILTIN, HIGH);
}

#ifdef DEBUG
void printWifiStatus() {
  Sprint("SSID: "); Sprintln(WiFi.SSID());
  Sprint("Signal strength (RSSI): "); Sprint(WiFi.RSSI()); Sprintln(" dBm");
  Sprint("IP address: "); Sprintln(WiFi.localIP());
  Sprint("Gateway: "); Sprintln(WiFi.gatewayIP());
  Sprint("Netmask: "); Sprintln(WiFi.subnetMask());
  Sprint("Websocket is at http://"); Sprint(WiFi.localIP()); Sprintln(":" + (String)socketPort + "/");
}
#endif
