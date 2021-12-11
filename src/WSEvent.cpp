#include "WSEvent.h"
#include "Bluetooth.h"

uint16_t WSEvent::counter = 1;

WebSocketsServer WSEvent::webSocket = WebSocketsServer(webSocketPort);

IPAddressFail WSEvent::IPAddressFailures[254];

/**
 * Populate the list of struct array with IP addresses
 * for a /24 subnet this will need to be modified if you are on something else
 */
void WSEvent::initIPAddressFailures() {
    for (int i=0; i<253; i++) {
        IPAddressFailures[i].FailedAttempts = 0;
    }
}

void WSEvent::webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("WEBSOCKET: [%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("WEBSOCKET: [%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        /* A simple system to block IP Addresses that fail connecting 5 times for a 5 minute period */
        // int IPAddressPosition = ip[3]-1;
        // if ((IPAddressPosition>=0) && (IPAddressPosition<253)) {
        //   int FailedAttempts = IPAddressFailures[IPAddressPosition].FailedAttempts;
        //   if (FailedAttempts<5) {
        //     if (strcmp((const char *)payload, "/jsonrpc") != 0) {
        //       Serial.println("WEBSOCKET: Client has not connected to the correct path, disconnecting...");
        //       Serial.printf("IP Address %d.%d.%d.%d has failed connecting\n",ip[0], ip[1], ip[2], ip[3]);
  
        //       IPAddressFailures[IPAddressPosition].lastFailure = time(nullptr);
        //       IPAddressFailures[IPAddressPosition].FailedAttempts=FailedAttempts+1;
        //       if ((FailedAttempts+1)==5) {
        //         Serial.printf("IP Address %d.%d.%d.%d has failed connecting 5 times, it will now be blocked for 300 seconds\n",ip[0], ip[1], ip[2], ip[3]);
        //       } 
        //       webSocket.disconnect(num);
        //     }        
        //   } else {
        //     Serial.println("Disconnecting client due to being banned");
        //     if ((IPAddressFailures[IPAddressPosition].lastFailure+300)<time(nullptr)) {
        //       Serial.println("It has now been over five minutes, disabling client block...");
        //       IPAddressFailures[IPAddressPosition].FailedAttempts = 0;
        //     }
        //     webSocket.disconnect(num);
        //   }
        // }
      }
      break;
    case WStype_TEXT:
      {
        unsigned long startTime;
        startTime = millis();

        Serial.printf("WEBSOCKET: [%u] Payload: %s\n", num, payload);
        Serial.printf("Counter: %d\n", counter);
        
        counter = counter + 1;
        /*  I want to avoid filling up stack space, and this is only needed to test latency */
        if (counter==10000) {
          counter=1;
        }
        JSONVar jsonBody = JSON.parse((const char *)payload);

        if (strcmp(jsonBody["method"], "Player.GetActivePlayers") == 0) {
          webSocket.sendTXT(num, "{\"id\": 1, \"jsonrpc\": \"2.0\", \"result\": [ { \"playerid\": 1, \"type\": \"video\" } ]}");
          return;
        } else {
          webSocket.sendTXT(num, "{\"id\":1,\"jsonrpc\":\"2.0\",\"result\":\"OK\"}");
        }

        if (JSON.typeof(jsonBody) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        if (Bluetooth::BLEconnected == false) {
          Serial.println("Bluetooth not connected");
          return;
        }
        if (!jsonBody.hasOwnProperty("method")) {
          Serial.println("JSON parse cannot find method");
          return;
        }
        Serial.println("Correctly parsed JSON");
        bluetooth.send(jsonBody);
        Serial.printf("Function time was %d\n", (int)(millis() - startTime));
        break;
      }
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING:
    case WStype_PONG:
      break;
  }

}
