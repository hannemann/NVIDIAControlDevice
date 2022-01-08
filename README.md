# Bluetooth LE Remote Companion

Remotely control devices via Bluetooth from your Browser, Homeassistant via NodeRed or any IR Remote 

This software runs on an ESP32 making it into a virtual keyboard running a websocket server, this allows clients to connect via websocket and send named keycodes.

Keycodes sent are also broadcasted to all connected websocket clients making it possible to react 
e.g. using Homeassistant via NodeRed and start your favourite streaming app via adb or toggle lights or...

A simple keyboard is also available in the web frontend. Point your browser to the esp's IP and check out.

The web frontend offers an easy way to configure your IR remote.

## Supported Hardware
* ESP32

### Recommended Peripherals
* TSOP4838 on PIN 23 (pin can be configured in prod_env.ini see example)

## Installation
### PlatformIO
* Checkout Project
* Connect ESP32
* Upload

## Configuration
### WIFI
Connect to the WIFI 'BLERC' with the password 0987654321 and enter your WIFI Credentials. Device reboots and than connects to your WIFI. Look up the IP in your Router and than point your Browser to it.

### Bluetooth
Bind the device you want to control to BLE Remote Companion.  
It happend to me sometinmes that the Mousepad did not work instantly. A rebind helped in that cases.

### Other
You find a configuration page in the menu of the frontend.
|Config|Description|
|------|-----------|
|Room  |Give it a name (needed for HA Websocket API)|


## Websocket Server
Remote Companion exposes a websocket server (default port: 81).
```
ws://1.2.3.4:81/jsonrpc
```
Supported commands:
* keydown
  * sends keydown
* keyup
  * sends keyup
* keypress
  * sends a keypress with optional longpress
* learn
  * learn keycode
* clear
  * clear IR configuration
* reboot
### keypress/keyup/keydown
#### Parameters
* {String} method <i>keypress|keyup|keydown</i>
* {Object} params
* {String} params.type <i>KEYBOARD|CONSUMER|APP_LAUNCHER|APP_CONTROL</i>
* {String} params.code <i>one of the keycodes as shown in [tables below](#keycode-tables)</i>
* {Number} (params.longpress) <i>1|0 optional longpress</i>

Example:
```json
{
    "method": "keypress",
    "params": {
        "type": "KEYBOARD",
        "code": "KEYCODE_ENTER",
        "longpress": 1
    } 
}
```
## Node-RED
You can use the Node-RED websocket nodes to send and receive messages to/from your Remote Companion.  
This enables you to send all keycodes defined in keys.h and also receive the keycodes sent by Remote Companion if you use your remote or the web frontend.  
I use it to have a remote in my Homeassistant and also start Apps via ADB by pressing the Red/Green/Yellow/Blu Buttons on my IR Remote.
### Configuration
* add Websocket out and configure accordingly
  * activate Send Heartbeat checkbox for automatic reconnect
* add Websocket in to receive messages if a key is pressed

### Home Assistant script example:
Observe `android_tv_control` with Node-RED `events:all` node to send messages via websocket to your Remote Companion
```yaml
android_tv_enter:
  sequence:
    - event: android_tv_control
      event_data:
        type: KEYBOARD
        code: KEYCODE_ENTER

android_tv_enter_longpress:
  sequence:
    - event: android_tv_control
      event_data:
        type: KEYBOARD
        code: KEYCODE_ENTER
        longpress: 1
```
### [Websocket out example flow](/doc/node-red/websocket-out-flow.json)
### [Websocket in (with longpress) example flow](/doc/node-red/websocket-in-flow.json)

## Keycode Tables
* [Keyboard Codes 0x07 (Type: KEYBOARD / 1)](/doc/keycodes/keyboard.md)
* [Keyboard Codes 0x0c (Type: CONSUMER / 2)](/doc/keycodes/consumer.md)
* [Keyboard Codes 0x0c (Type: APP_LAUNCHER / 3)](/doc/keycodes/app-launcher.md)

* [Keyboard Codes 0x0c (Type: APP_CONTROL / 4)](/doc/keycodes/app-control.md)
* [Named and Decimal Keycodes from Android Open Source Project](/doc/keycodes/AOS-Project.md)
## 
* change Wifi MAC also changes BT MAC? Test... this could solve issues if device is connected to an unknown bt client
* documentation
* Mouse Mode -> needs testing
  * https://github.com/T-vK/ESP32-BLE-Mouse
  * https://github.com/olegos76/nimble_kbdhid_example/blob/main/main/gatt_vars.c)
* swipe
* handle ir errors when learning
* layout onfiguration
* configurable ws timeout
* case for Node MCU style with TSOP (reset button)
* start BLE Advertising via tactile button longpress
* Basic Auth
* configurable device name/room visible in frontend/title
* deeplinks to layouts for iframes