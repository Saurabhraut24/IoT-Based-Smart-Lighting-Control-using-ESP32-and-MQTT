# Smart Lighting Control using ESP32, MQTT and Node-RED

A simulation-based Internet of Things (IoT) mini-project for remotely controlling an RGB LED through a web dashboard. The dashboard is built with Node-RED and FlowFuse Dashboard, messages are exchanged using MQTT through the HiveMQ public broker, and an ESP32 simulated in Wokwi receives the commands and controls the RGB LED.

> **Project status:** Simulation tested in Wokwi. The `RED`, `GREEN`, `BLUE`, and `OFF` commands have been tested successfully.

## Table of Contents

- [1. Project Overview](#1-project-overview)
- [2. Problem Statement](#2-problem-statement)
- [3. Objectives](#3-objectives)
- [4. Features](#4-features)
- [5. System Architecture](#5-system-architecture)
- [6. Working Principle](#6-working-principle)
- [7. Technologies and Tools](#7-technologies-and-tools)
- [8. Circuit Design and Pin Connections](#8-circuit-design-and-pin-connections)
- [9. MQTT Configuration](#9-mqtt-configuration)
- [10. Software and Firmware Logic](#10-software-and-firmware-logic)
- [11. Project Folder Structure](#11-project-folder-structure)
- [12. Setup and Execution](#12-setup-and-execution)
- [13. Test Results](#13-test-results)
- [14. Screenshots and Demonstration](#14-screenshots-and-demonstration)
- [15. Limitations](#15-limitations)
- [16. Future Enhancements](#16-future-enhancements)
- [17. Safety and Security Notes](#17-safety-and-security-notes)
- [18. Author](#18-author)
- [19. License](#19-license)

## 1. Project Overview

The **Smart Lighting Control using ESP32, MQTT and Node-RED** project demonstrates how an IoT device can be controlled remotely through a dashboard. A user selects a colour on the dashboard. Node-RED publishes the selected command to an MQTT topic. The HiveMQ broker forwards the message to the ESP32, which is subscribed to that topic. The ESP32 interprets the command and updates the RGB LED outputs.

The project is implemented as a **simulation**, using Wokwi for the ESP32 and RGB LED circuit. It demonstrates the integration of an embedded controller, wireless connectivity, messaging protocol, and web-based user interface.

## 2. Problem Statement

Controlling lighting locally can be inconvenient when a user wants to change the light state from a dashboard or remote interface. This project demonstrates a simple IoT-based approach in which lighting commands are sent over MQTT and applied by a simulated ESP32.

## 3. Objectives

- Build and demonstrate an IoT smart-lighting prototype in Wokwi.
- Connect an ESP32 simulation to Wi-Fi and an MQTT broker.
- Publish lighting commands from a Node-RED dashboard.
- Subscribe to an MQTT topic on the ESP32 and interpret incoming commands.
- Control WS2812/NeoPixel LEDs using fixed colours, automatic colour changes, a running-light effect, and an OFF command.
- Validate the dashboard-to-ESP32 communication path through simulation output and serial logs.

## 4. Features

- Browser-based light-control dashboard.
- Button-group controls for `RED`, `GREEN`, `BLUE`, and `OFF`.
- MQTT communication through the HiveMQ public broker.
- ESP32 Wi-Fi connection in Wokwi.
- MQTT command reception and processing.
- Serial Monitor messages showing received commands and light states.

The firmware also contains an `ON` command that sets all three RGB channels HIGH (white). This command should be listed as tested functionality only after it has been tested separately.

## 5. System Architecture

```text
             USER
              |
              v
+------------------------------+
| Web Browser                  |
| FlowFuse Dashboard           |
| RED | GREEN | BLUE | OFF     |
+---------------+--------------+
                |
                v
+------------------------------+
| Node-RED                     |
| Dashboard Button Group       |
| MQTT Output Node             |
+---------------+--------------+
                |
                | MQTT PUBLISH
                | Topic: smartlight/control
                v
+------------------------------+
| HiveMQ Public MQTT Broker    |
+---------------+--------------+
                ^
                | MQTT SUBSCRIBE
                |
+---------------+--------------+
| ESP32 (Wokwi Simulation)     |
| Wi-Fi + PubSubClient         |
| Command Callback / Logic     |
+---------------+--------------+
                |
                | GPIO 25, 26, 27
                v
+------------------------------+
| RGB LED                      |
| Red / Green / Blue / Off     |
+------------------------------+
```

### Architecture components

| Layer | Component | Responsibility |
|---|---|---|
| User interface | FlowFuse Dashboard | Lets the user select a lighting command |
| Flow / application logic | Node-RED | Sends the selected command to MQTT |
| Communication | MQTT | Transfers messages using a publish/subscribe model |
| Broker | HiveMQ Public Broker | Routes published messages to subscribers |
| Embedded processing | ESP32 in Wokwi | Receives and interprets MQTT commands |
| Output / actuator | RGB LED | Shows the selected colour or turns off |

## 6. Working Principle

1. The user opens the Node-RED dashboard in a browser.
2. The user selects `RED`, `GREEN`, `BLUE`, or `OFF`.
3. The dashboard button group outputs the selected string.
4. The Node-RED MQTT Out node publishes the string to `smartlight/control`.
5. The HiveMQ broker receives the message and forwards it to connected subscribers of that topic.
6. The ESP32 MQTT client receives the message through its callback.
7. The firmware converts the command to uppercase and checks which command was received.
8. The ESP32 sets GPIO 25, GPIO 26, and GPIO 27 accordingly.
9. The RGB LED changes colour or switches off, and the Serial Monitor displays the command and state.

## 7. Technologies and Tools

| Technology / Tool | Use in the project |
|---|---|
| Wokwi | Online simulation of the ESP32 and RGB LED |
| ESP32 DevKit C V4 | Simulated microcontroller |
| Arduino C++ | ESP32 firmware language |
| Wi-Fi | Network connectivity in Wokwi |
| MQTT | Messaging protocol |
| HiveMQ Public Broker | MQTT broker |
| PubSubClient | MQTT client library for ESP32 |
| Node-RED | Flow-based integration and MQTT publishing |
| FlowFuse Dashboard 2.0 | Browser dashboard controls |
| Git and GitHub | Source-code version control and project hosting |

## 8. Circuit Design and Pin Connections

### Components in the simulation

- ESP32 DevKit C V4
- One common-cathode RGB LED
- Three 220 Ω resistors
- Jumper-wire connections

### Pin connection table

| RGB LED pin/channel | Connection |
|---|---|
| Red channel (R) | ESP32 GPIO 25 through a 220 Ω resistor |
| Green channel (G) | ESP32 GPIO 26 through a 220 Ω resistor |
| Blue channel (B) | ESP32 GPIO 27 through a 220 Ω resistor |
| Common cathode (COM) | ESP32 GND |

**Note:** These are the connections used in the Wokwi simulation. Physical hardware has not been claimed as tested.

## 9. MQTT Configuration

| Parameter | Value |
|---|---|
| Broker hostname | `broker.hivemq.com` |
| Port | `1883` |
| MQTT version | MQTT 3.1.1 |
| Topic | `smartlight/control` |
| QoS | `0` |
| Retain | `false` |
| Wokwi Wi-Fi SSID | `Wokwi-GUEST` |
| Wokwi Wi-Fi password | Empty string |

### Commands

| Payload | Firmware action |
|---|---|
| `RED` | Turns on the red channel |
| `GREEN` | Turns on the green channel |
| `BLUE` | Turns on the blue channel |
| `OFF` | Turns off all RGB channels |
| `ON` | Sets all three RGB channels HIGH (white); test separately before claiming as verified |

The topic must match in both the ESP32 firmware and the Node-RED MQTT Out node.

## 10. Software and Firmware Logic

### ESP32 firmware responsibilities

- Connect to the Wokwi Wi-Fi network.
- Connect to the MQTT broker.
- Subscribe to `smartlight/control`.
- Receive a message and build a command string from the payload.
- Match the command against supported values.
- Set the RGB LED GPIO pins.
- Print the received command and resulting light state to the Serial Monitor.
- Reconnect to MQTT if the connection is lost.

### Simplified algorithm

```text
START
  |
Initialize Serial Monitor and RGB GPIO pins
  |
Connect to Wi-Fi
  |
Configure MQTT broker and callback
  |
Connect to MQTT broker
  |
Subscribe to "smartlight/control"
  |
LOOP:
  If MQTT is disconnected:
      Reconnect and subscribe again
  Process incoming MQTT messages
  When a command arrives:
      Read payload
      Convert command to uppercase
      If RED: turn on red channel
      Else if GREEN: turn on green channel
      Else if BLUE: turn on blue channel
      Else if OFF: turn all channels off
      Else if ON: turn all channels on
      Print command and state
END LOOP
```

### Important files

- `sketch.ino` — ESP32 firmware.
- `diagram.json` — Wokwi circuit and wiring configuration.
- `libraries.txt` — Wokwi library list; includes `PubSubClient`.

## 11. Project Folder Structure

The repository currently includes the Wokwi simulation, a Node-RED-related folder, and a JSON flow export. A simplified view of the visible repository structure is:

```text
Smart-Lighting-Control/
├── Node-RED_Flow 2_files/
│   └── (Node-RED editor assets and related files)
├── Wokwi files/
│   └── light/
│       ├── diagram.json
│       ├── libraries.txt
│       ├── sketch.ino
│       └── wokwi-project.txt
├── json file/
│   └── flows.json
├── Node-RED_Flow 2.html
└── README.md
```

Folder names above reflect the current repository layout shown during project preparation. If you rename folders, update this section accordingly.

**Repository cleanup recommendation:** The `Node-RED_Flow 2_files/` folder appears to contain exported webpage/editor assets. If `Node-RED_Flow 2.html` is only an exported Node-RED editor page, it may not be a portable Node-RED flow. To make the project easy to reproduce, export the flow from Node-RED using **Menu → Export → JSON**, save it as a clearly named file such as `node-red-flow.json`, and document that file here. Avoid committing unnecessary downloaded dependencies or private data.

## 12. Setup and Execution

### A. Start the Wokwi simulation

1. Open the Wokwi project containing `sketch.ino`, `diagram.json`, and `libraries.txt`.
2. Confirm the RGB LED uses a common-cathode configuration.
3. Confirm the RGB connections match GPIO 25, 26, and 27, with COM connected to GND.
4. Confirm `libraries.txt` contains:

   ```text
   PubSubClient
   ```

5. Start the simulation and open the Serial Monitor.
6. Check that Wi-Fi connects, MQTT connects, and the device subscribes to `smartlight/control`.

### B. Start Node-RED

Install Node.js and Node-RED if needed, then run:

```bash
node-red
```

Open the Node-RED editor:

```text
http://127.0.0.1:1880
```

The dashboard URL used in this setup is:

```text
http://127.0.0.1:1880/dashboard/page1
```

The URL can differ if your Node-RED configuration or dashboard path changes.

### C. Configure Node-RED

1. Install **FlowFuse Dashboard 2.0** if it is not already installed.
2. Add a dashboard button-group node and assign it to a dashboard group.
3. Add the options below:

   | Label | Value |
   |---|---|
   | RED | `RED` |
   | GREEN | `GREEN` |
   | BLUE | `BLUE` |
   | OFF | `OFF` |

4. Connect the button-group output to an MQTT Out node.
5. Configure the MQTT broker:
   - Server: `broker.hivemq.com`
   - Port: `1883`
   - MQTT version: `3.1.1`
   - TLS: disabled for this demonstration
6. Set the MQTT Out topic to `smartlight/control`.
7. Deploy the flow and open the dashboard.
8. Click each button and observe the LED and Serial Monitor.

### D. Verify the end-to-end result

For each button, verify both:
- The expected LED colour/state in Wokwi.
- The corresponding command and light status in the Serial Monitor.

## 13. Test Results

The following commands were tested successfully through the Node-RED dashboard in the Wokwi simulation.

| Test case | Input command | Expected output | Result |
|---|---|---|---|
| 1 | `RED` | Red LED channel turns on | Pass |
| 2 | `GREEN` | Green LED channel turns on | Pass |
| 3 | `BLUE` | Blue LED channel turns on | Pass |
| 4 | `OFF` | All LED channels turn off | Pass |

Example Serial Monitor output:

```text
MQTT Message Received: RED
Light: RED
```

`ON` is implemented in the code but is not included in the passed test cases until it is tested separately.

## 14. Screenshots and Demonstration

For a complete GitHub project page or academic submission, add your own screenshots in a folder such as `docs/images/`:

1. **Wokwi circuit:** ESP32, RGB LED, resistors, and wiring.
2. **Node-RED flow:** button group connected to MQTT Out and broker status.
3. **Dashboard:** RED/GREEN/BLUE/OFF controls.
4. **Test evidence:** Wokwi LED and Serial Monitor showing a successful command.

If you record a demonstration video, add its public link here:

- **Wokwi project:** _Add your shareable Wokwi project URL_
- **Demonstration video:** _Add your video URL, if available_

Do not leave placeholder links in the final version; replace them with the actual shareable URLs.

## 15. Limitations

- The project has been demonstrated in simulation; physical hardware operation has not been verified.
- The project uses a public MQTT broker for demonstration, so broker availability and network conditions can affect operation.
- MQTT messages are sent through port 1883 without TLS in this configuration.
- The current dashboard provides basic colour selection and OFF control.
- The implementation controls one simulated RGB LED.

## 16. Future Enhancements

- Add brightness adjustment.
- Add more colours and lighting effects.
- Add a separate power ON/OFF control.
- Show connection and device status on the dashboard.
- Add command validation and improved error/status reporting.
- Use a private MQTT broker with authentication and TLS.
- Test the design with physical ESP32 hardware after suitable wiring and power checks.

## 17. Safety and Security Notes

- The public broker is for demonstrations and experiments, not sensitive or production data.
- The topic `smartlight/control` is predictable. Other public-broker users may publish to the same topic, so use a unique topic for your own testing.
- Do not publish credentials, personal data, or confidential messages.
- For production use, configure an authenticated broker and TLS, and use a unique topic namespace.

## 18. Author

**Saurabh Raut**  
B.Tech — Electronics and Computer Engineering  
Walchand Institute of Technology, Solapur, Maharashtra, India

