## Websocket Client and Server for Arduino Nano 33 IoT

This is a simple library that implements a Websocket client and server running on an Arduino Nano 33 IoT.

Cloned from https://github.com/morrissinger/ESP8266-Websocket and checked to see if it worked with WiFiNINA.h.

### Notes
Inside of the WebSocketServer class there is a compiler directive to turn on support for the older "Hixie76" standard. If you don't need it, leave it off as it greatly increases the memory required.

This library only supports single-frame text frames. It currently does not recognize continuation frames, binary frames, or ping/pong frames.

The handling of failed handshakes has been changed from calling terminateStream() to sending a "400: Bad Request" header, as there is no stream to terminate in such cases.

### Credits
Branden says: "Thank you to github user ejeklint for the excellent starting point for this library. From his original Hixie76-only code I was able to add support for RFC 6455 and create the WebSocket client."

My thanks to Branden for his work on this library.
