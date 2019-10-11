## Example websocket implementations for the Arduino Nano 33 IoT

- The websocketclock.ino example will show a clock in your web browser when you point it at the Arduino's IP address  http://x.x.x.99/ , where x.x.x are the first three bytes of the IP addresses of your network. If x.x.x.99 is already taken, you need to change the code.

- The multisocket.ino example shows how to "spawn" multiple websocket servers to handle multiple websocket clients. You can use wsp.html to connect to a websocket server; if you open that page again in another tab, it will connect also. **Note:** The websocket server's address is hardcoded in the wsp.html page; change it to your Arduino's address.

Both examples will try to set the Arduino's IP address to http://x.x.x.99/ , where x.x.x are the first three bytes of the IP addresses of your network. If x.x.x.99 is already taken, you need to change the code.

### The examples use these libraries:

- https://github.com/ocrdu/WiFiNINA for WiFi connection and servers (**Don't** use https://github.com/arduino-libraries/WiFiNINA 1.4.0)
- https://github.com/ocrdu/NINA-Websocket as the websocket library (Forked and changed from https://github.com/morrissinger/ESP8266-Websocket);
- https://github.com/adamvr/arduino-base64 for base64 decoding;
- https://github.com/arduino-libraries/RTCZero as the Real Time Clock library.
