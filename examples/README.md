## Example websocket implementation for the Arduino Nano 33 IoT

This example will show a clock in your web browser when you point it at the Arduino's IP address  http://x.x.x.99/ , where x.x.x are the first three bytes of the IP addresses of your network. If x.x.x.99 is already taken, you need to change the code.

### Needs these libraries to be present:
- WiFiNINA.h
- WebSocketServer.h (this one)
- RTCZero.h
- Base64.h
- wifi_secrets.h
