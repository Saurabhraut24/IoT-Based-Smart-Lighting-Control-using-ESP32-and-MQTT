# IoT-Based Smart Lighting Control using ESP32, MQTT and Node-RED
 
A simulation-based Internet of Things (IoT) mini-project that remotely controls **26 WS2812 addressable LEDs** (13 strips × 2 pixels) from a web dashboard. The dashboard is built in **Node-RED (Dashboard 2.0)**, commands travel over **MQTT** through the **HiveMQ public broker**, and an **ESP32 simulated in Wokwi** receives them and drives the LEDs with fixed colours, an automatic colour-change mode, a running-light effect and OFF.
 
> **Project status:** Simulation tested in Wokwi. The `RED`, `GREEN`, `BLUE` and `OFF` commands have been tested through the dashboard. Other commands are implemented in the firmware but are not listed as tested (see [Test Results](#12-test-results)).
---
##wokvi Link
https://wokwi.com/projects/477145959232628737
---
---
 
## Table of Contents
 
1. [Project Overview](#1-project-overview)
2. [Problem Statement](#2-problem-statement)
3. [Objectives](#3-objectives)
4. [Features](#4-features)
5. [System Architecture](#5-system-architecture)
6. [Working Principle](#6-working-principle)
7. [Technologies and Tools](#7-technologies-and-tools)
8. [Circuit Design and Connections](#8-circuit-design-and-connections)
9. [MQTT Configuration and Commands](#9-mqtt-configuration-and-commands)
10. [Firmware Logic](#10-firmware-logic)
11. [Setup and Execution](#11-setup-and-execution)
12. [Test Results](#12-test-results)
13. [Screenshots and Demonstration](#13-screenshots-and-demonstration)
14. [Project Folder Structure](#14-project-folder-structure)
15. [Limitations](#15-limitations)
16. [Future Enhancements](#16-future-enhancements)
17. [Safety and Security Notes](#17-safety-and-security-notes)
18. [Author](#18-author)
---
 
## 1. Project Overview
 
This project demonstrates how a lighting system can be controlled remotely using IoT technologies. A user presses a button on the Node-RED dashboard. Node-RED publishes the command text to the MQTT topic `smartlight/control`. The HiveMQ broker forwards the message to the ESP32, which is subscribed to the same topic. The ESP32 interprets the command and updates the WS2812 LED strip.
 
The system is implemented as a **simulation** in Wokwi and combines an embedded controller, Wi-Fi connectivity, the MQTT publish/subscribe protocol and a browser-based user interface.
 
## 2. Problem Statement
 
Conventional lighting is switched manually and offers no remote control, colour selection or automated effects. This project designs a simple IoT-based approach in which lighting colours and effects are selected from a dashboard, sent over MQTT and applied by an ESP32.
 
## 3. Objectives
 
- Build and demonstrate an IoT smart-lighting prototype in Wokwi.
- Connect the ESP32 simulation to Wi-Fi and an MQTT broker.
- Publish lighting commands from a Node-RED dashboard.
- Subscribe to an MQTT topic on the ESP32 and interpret incoming commands.
- Control 26 WS2812/NeoPixel LEDs using fixed colours, an automatic colour-change mode, a running-light effect and an OFF command.
- Validate the dashboard → MQTT → ESP32 → LED path using LED output and Serial Monitor logs.
## 4. Features
 
- Browser dashboard with **Fixed Colors**, **Lighting Effects** and **Power Control** sections.
- Fixed colours: `RED`, `GREEN`, `BLUE`, `YELLOW`, `PURPLE`, `CYAN`, `PINK` (and `WHITE` in firmware).
- **AUTO** mode – cycles through 7 colours every 1 second.
- **RUN** mode – a red running light with a 2-pixel fading trail that wraps from LED 26 back to LED 1.
- **OFF** – turns all LEDs off.
- Non-blocking effects using `millis()`, so MQTT stays responsive.
- Commands can also be typed in the Serial Monitor for local testing.
- Serial Monitor logs for Wi-Fi, MQTT status, received commands and active mode.
- Automatic MQTT reconnection with a unique client ID.
## 5. System Architecture
 
```text
                    USER
                     |
                     v
        +---------------------------+
        | Web Browser               |
        | Node-RED Dashboard 2.0    |
        +-------------+-------------+
                      |
                      v
        +---------------------------+
        | Node-RED                  |
        | Button Groups -> MQTT Out |
        +-------------+-------------+
                      |  MQTT PUBLISH
                      |  Topic: smartlight/control
                      v
        +---------------------------+
        | HiveMQ Public MQTT Broker |
        | broker.hivemq.com:1883    |
        +-------------+-------------+
                      |  MQTT SUBSCRIBE (over Wi-Fi)
                      v
        +---------------------------+
        | ESP32 (Wokwi Simulation)  |
        | PubSubClient + Logic      |
        +-------------+-------------+
                      |  GPIO 5 (data)
                      v
        +---------------------------+
        | 13 x WS2812 strips        |
        | 2 pixels each = 26 LEDs   |
        +---------------------------+
```
 
| Layer | Component | Responsibility |
|---|---|---|
| User interface | Node-RED Dashboard 2.0 | Lets the user choose a colour, effect or OFF |
| Application logic | Node-RED flow | Publishes the selected command to MQTT |
| Communication | MQTT over Wi-Fi | Publish/subscribe message transfer |
| Broker | HiveMQ public broker | Routes messages to subscribers |
| Embedded processing | ESP32 (Wokwi) | Receives, parses and executes commands |
| Actuator | 26 × WS2812 LEDs | Displays colours and effects |
 
## 6. Working Principle
 
1. The user opens the Node-RED dashboard in a browser.
2. The user presses a button, for example `RED`, `AUTO`, `RUN` or `OFF`.
3. The dashboard outputs the command string to the MQTT Out node.
4. Node-RED publishes it to the topic `smartlight/control`.
5. HiveMQ forwards the message to all subscribers of that topic.
6. The ESP32 `mqttCallback()` builds a string from the payload.
7. `processCommand()` trims the text, converts it to uppercase and matches it against the supported commands.
8. The ESP32 updates the 26 LEDs (static colour, AUTO cycle or RUN effect).
9. The Serial Monitor prints the received command and the active mode.
## 7. Technologies and Tools
 
| Technology / Tool | Use in the project |
|---|---|
| Wokwi | Online simulation of the ESP32 and the LED strips |
| ESP32 DevKit-C V4 | Simulated microcontroller with built-in Wi-Fi |
| WS2812 LED strips | Addressable LED actuators (13 strips × 2 pixels) |
| Arduino C/C++ | ESP32 firmware |
| Adafruit NeoPixel | Library to drive the WS2812 LEDs |
| PubSubClient | MQTT client library for the ESP32 |
| Wi-Fi (`Wokwi-GUEST`) | Network connectivity inside Wokwi |
| MQTT | Messaging protocol |
| HiveMQ Public Broker | MQTT broker |
| Node-RED + Dashboard 2.0 | Flow-based integration and web dashboard |
| Git and GitHub | Version control and project hosting |
 
## 8. Circuit Design and Connections
 
### Components in the simulation
 
- ESP32 DevKit-C V4
- 13 × WS2812 LED strips, 2 pixels each (26 LEDs in total)
- Jumper-wire connections
### Connection summary
 
| Connection | Details |
|---|---|
| LED data input | ESP32 **GPIO 5** → `DIN` of strip 1 |
| Strip-to-strip data | `DOUT` of each strip → `DIN` of the next strip (daisy chain) |
| Power / ground | Supply and ground lines are chained from strip to strip (see `diagram.json`) |
| LED order | Strip 1 contains LEDs 1–2, strip 2 contains LEDs 3–4, … strip 13 contains LEDs 25–26 |
 
Firmware settings: `NUM_LEDS = 26`, `LED_PIN = 5`, `NEO_GRB + NEO_KHZ800`, brightness `80`.
 
> These connections are for the Wokwi simulation. Operation on physical hardware has not been tested.
 
## 9. MQTT Configuration and Commands
 
| Parameter | Value |
|---|---|
| Broker hostname | `broker.hivemq.com` |
| Port | `1883` (no TLS) |
| Topic | `smartlight/control` |
| Client ID | `ESP32-SmartLight-<random 4 digits>` |
| Wokwi Wi-Fi SSID | `Wokwi-GUEST` |
| Wokwi Wi-Fi password | Empty string |
 
The topic must be identical in the ESP32 firmware and the Node-RED MQTT Out node.
 
### Supported commands
 
Commands are not case-sensitive.
 
| Payload | Action | On dashboard |
|---|---|---|
| `RED` | All LEDs red | Yes |
| `GREEN` | All LEDs green | Yes |
| `BLUE` | All LEDs blue | Yes |
| `YELLOW` | All LEDs yellow | Yes |
| `PURPLE` | All LEDs purple | Yes |
| `CYAN` | All LEDs cyan | Yes |
| `PINK` | All LEDs pink | Yes |
| `WHITE` | All LEDs white | Inject node in flow |
| `AUTO` | Cycles through 7 colours every 1000 ms | Yes |
| `RUN` / `RUNNING` / `RUNNING LIGHT` | Red running light with trail, 70 ms per step | Yes (`RUN`) |
| `OFF` | All LEDs off | Yes |
 
Any other text prints `UNKNOWN COMMAND` and the list of valid commands on the Serial Monitor.
 
## 10. Firmware Logic
 
### Responsibilities
 
- Initialise the Serial Monitor and the NeoPixel strip, with all LEDs off.
- Connect to Wi-Fi and the MQTT broker, then subscribe to `smartlight/control`.
- Parse incoming commands and select the light mode (`OFF_MODE`, `STATIC_MODE`, `AUTO_MODE`, `RUN_MODE`).
- Run AUTO and RUN effects without `delay()`, using `millis()`.
- Reconnect to MQTT automatically if the connection is lost.
- Accept commands from the Serial Monitor as well as MQTT.
### Simplified algorithm
 
```text
START
  Initialise Serial, NeoPixel strip (26 LEDs, brightness 80), all LEDs OFF
  Connect to Wi-Fi
  Set MQTT broker and callback; connect; subscribe "smartlight/control"
 
LOOP:
  If MQTT disconnected -> reconnect and subscribe
  mqttClient.loop()                      // receive messages
  Read Serial Monitor command (if any)
 
  On command (MQTT or Serial):
      trim + convert to UPPERCASE
      colour name  -> mode = STATIC, set all 26 LEDs to that colour
      AUTO         -> mode = AUTO
      RUN/RUNNING  -> mode = RUN, start from LED 1
      OFF          -> mode = OFF, clear all LEDs
      otherwise    -> print UNKNOWN COMMAND
 
  If mode == AUTO: every 1000 ms show next of 7 colours
  If mode == RUN : every 70 ms move red LED one step (with 2-pixel trail),
                   after LED 26 return immediately to LED 1
END LOOP
```
 
## 11. Setup and Execution
 
### A. Start the Wokwi simulation
 
1. Open the Wokwi project containing `sketch.ino`, `diagram.json` and `libraries.txt`.
2. Make sure `libraries.txt` contains:
```text
   Adafruit NeoPixel
   PubSubClient
```
 
3. Start the simulation and open the Serial Monitor.
4. Confirm that Wi-Fi connects, MQTT connects and the device subscribes to `smartlight/control`.
### B. Start Node-RED
 
```bash
node-red
```
 
Open the editor at `http://127.0.0.1:1880`. The dashboard is normally available at `http://127.0.0.1:1880/dashboard/page1` (the path may differ with your configuration).
 
### C. Configure Node-RED
 
1. Install **Dashboard 2.0** (`@flowfuse/node-red-dashboard`) if it is not installed.
2. Import the flow from `json file/flows.json` (Menu → Import), or create button groups as below.
3. Button groups and their values:
   | Group | Options (label = value) |
   |---|---|
   | Fixed Colors | `RED`, `GREEN`, `BLUE`, `YELLOW`, `PURPLE`, `CYAN`, `PINK` |
   | Lighting Effects | `AUTO`, `RUN` |
   | Power Control | `OFF` |
4. Connect all button outputs to one **MQTT Out** node:
   - Server `broker.hivemq.com`, port `1883`, MQTT 3.1.1, TLS disabled
   - Topic `smartlight/control`
5. Deploy the flow and open the dashboard.
### D. Verify the result
 
For each button, check that:
 
- the LEDs in Wokwi show the expected colour or effect, and
- the Serial Monitor shows `Command Received: <COMMAND>` and the mode message.
## 12. Test Results
 
Tested through the Node-RED dashboard in the Wokwi simulation:
 
| Test case | Input command | Expected output | Result |
|---|---|---|---|
| 1 | `RED` | All 26 LEDs turn red | Pass |
| 2 | `GREEN` | All 26 LEDs turn green | Pass |
| 3 | `BLUE` | All 26 LEDs turn blue | Pass |
| 4 | `OFF` | All LEDs turn off | Pass |
 
Example Serial Monitor output:
 
```text
MQTT MESSAGE RECEIVED
Topic: smartlight/control
Command: RED
Command Received: RED
RED MODE
```
 
`YELLOW`, `PURPLE`, `CYAN`, `PINK`, `WHITE`, `AUTO` and `RUN` are implemented in the firmware. Add them to the table above only after testing each one.
 
## 13. Screenshots and Demonstration
 
Add your own screenshots in `docs/images/` and link them here:
 
| # | Screenshot | What it shows |
|---|---|---|
| 1 | Wokwi circuit | ESP32 with 13 chained LED strips |
| 2 | Node-RED flow | Buttons connected to MQTT Out, status "connected" |
| 3 | Dashboard + simulation | Dashboard button pressed, LEDs and Serial Monitor output |
| 4 | Wokwi code view | `sketch.ino` with Wi-Fi, MQTT and NeoPixel code |
 
Example embed once the files are added:
 
```markdown
![Dashboard and simulation](docs/images/dashboard-output.png)
```
 
- **Wokwi project:** _add your shareable Wokwi project URL_
- **Demonstration video:** _add your video URL_
## 14. Project Folder Structure
 
```text
Smart-Lighting-Control/
├── Wokwi files/
│   └── light/
│       ├── diagram.json
│       ├── libraries.txt
│       ├── sketch.ino
│       └── wokwi-project.txt
├── json file/
│   └── flows.json
├── docs/
│   └── images/
├── Node-RED_Flow 2.html
├── Node-RED_Flow 2_files/
└── README.md
```
 
`Node-RED_Flow 2.html` and its `_files` folder are an exported web page of the editor and are not a portable flow. Use `json file/flows.json` (exported with Menu → Export → JSON) to reproduce the project.
 
## 15. Limitations
 
- Demonstrated in simulation only; physical hardware has not been verified.
- Uses a public MQTT broker, so availability and network conditions can affect operation.
- MQTT runs on port 1883 without TLS or authentication.
- All LEDs show the same colour in static mode; there is no per-LED or brightness control from the dashboard.
- Only the commands listed under [Test Results](#12-test-results) have been verified.
## 16. Future Enhancements
 
- Brightness slider and RGB colour picker on the dashboard.
- More lighting effects and adjustable speed.
- Device and connection status indicator on the dashboard.
- Private MQTT broker with authentication and TLS.
- Testing on a physical ESP32 with real WS2812 strips and a suitable 5 V supply.
## 17. Safety and Security Notes
 
- The public broker is meant for demonstrations, not for sensitive or production data.
- The topic `smartlight/control` is predictable and other public-broker users could publish to it. Use a unique topic for your own testing.
- Do not publish credentials, personal data or confidential messages.
- For real hardware, power the LED strips from a suitable supply and share ground with the ESP32.
## 18. Author
 
**Saurabh Raut**
B.Tech – Electronics and Computer Engineering
Walchand Institute of Technology, Solapur, Maharashtra, India
 









Claude finished the response
