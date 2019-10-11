## Websocket client and server for the Arduino Nano 33 IoT

This is a simple library that implements a Websocket client and server running on an Arduino Nano 33 IoT.

Forked from from https://github.com/morrissinger/ESP8266-Websocket and checked to see if it worked with WiFiNINA.h. Main edit so far: see third item under "Notes".

### Notes
Inside of the WebSocketServer class there is a compiler directive to turn on support for the older "Hixie76" standard. If you don't need it, leave it off as it greatly increases the memory required.

This library only supports single-frame text frames. It currently does not recognize continuation frames, binary frames, or ping/pong frames.

### Main changes in this fork

- The handling of failed handshakes on the server side has been changed from calling terminateStream() to sending a "400: Bad Request" header, as there is no stream to terminate in such cases.

- A constructor that takes a Client as a parameter has been added to the WebSocketServer class, and also a handshake function without parameters, so you can easily "spawn" multiple websocket servers for multiple websocket clients. Another option for this would be to provide all functions in the class with an extra Client parameter, but I didn't feel like doing that.

The library should still function in the original way also, and not break existing code.

### Credits
Branden says: "Thank you to github user ejeklint for the excellent starting point for this library. From his original Hixie76-only code I was able to add support for RFC 6455 and create the WebSocket client."

My thanks to Branden for his work on this library.
