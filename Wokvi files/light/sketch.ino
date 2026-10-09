#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

// =====================================================
// ESP32 IoT SMART LIGHTING CONTROL
// 13 WS2812 Strips × 2 Pixels = 26 LEDs
// =====================================================

// ---------------- LED SETTINGS ----------------
#define LED_PIN 5
#define NUM_LEDS 26

Adafruit_NeoPixel strip(
  NUM_LEDS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// ---------------- WIFI SETTINGS ----------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ---------------- MQTT SETTINGS ----------------
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

const char* mqttTopic = "smartlight/control";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ---------------- LIGHT MODES ----------------
enum LightMode {
  OFF_MODE,
  STATIC_MODE,
  AUTO_MODE,
  RUN_MODE
};

LightMode currentMode = OFF_MODE;

// ---------------- VARIABLES ----------------
unsigned long previousMillis = 0;

int runningPosition = 0;
int autoColor = 0;

// Running light speed
const unsigned long RUN_INTERVAL = 70;

// Auto color change interval
const unsigned long AUTO_INTERVAL = 1000;


// =====================================================
// SET ALL LEDS TO COLOR
// =====================================================

void setColor(byte r, byte g, byte b) {

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  strip.show();
}


// =====================================================
// TURN OFF ALL LEDS
// =====================================================

void turnOff() {

  strip.clear();
  strip.show();

  Serial.println("ALL LEDs OFF");
}


// =====================================================
// RUNNING LIGHT
// =====================================================

void runningLight() {

  unsigned long now = millis();

  if (now - previousMillis >= RUN_INTERVAL) {

    previousMillis = now;

    // Turn all LEDs OFF
    strip.clear();

    // Main running LED - RED
    strip.setPixelColor(
      runningPosition,
      strip.Color(255, 0, 0)
    );

    // Small trailing effect
    if (runningPosition > 0) {
      strip.setPixelColor(
        runningPosition - 1,
        strip.Color(100, 0, 0)
      );
    }

    if (runningPosition > 1) {
      strip.setPixelColor(
        runningPosition - 2,
        strip.Color(30, 0, 0)
      );
    }

    strip.show();

    // Move to next LED
    runningPosition++;

    // IMPORTANT:
    // LED 26 ke baad immediately LED 1
    if (runningPosition >= NUM_LEDS) {
      runningPosition = 0;
      previousMillis = millis();
    }
  }
}


// =====================================================
// AUTO COLOR CHANGE
// =====================================================

void autoColorChange() {

  unsigned long now = millis();

  if (now - previousMillis >= AUTO_INTERVAL) {

    previousMillis = now;

    switch (autoColor) {

      case 0:
        setColor(255, 0, 0);
        Serial.println("AUTO: RED");
        break;

      case 1:
        setColor(0, 255, 0);
        Serial.println("AUTO: GREEN");
        break;

      case 2:
        setColor(0, 0, 255);
        Serial.println("AUTO: BLUE");
        break;

      case 3:
        setColor(255, 255, 0);
        Serial.println("AUTO: YELLOW");
        break;

      case 4:
        setColor(255, 0, 255);
        Serial.println("AUTO: PURPLE");
        break;

      case 5:
        setColor(0, 255, 255);
        Serial.println("AUTO: CYAN");
        break;

      case 6:
        setColor(255, 255, 255);
        Serial.println("AUTO: WHITE");
        break;
    }

    autoColor++;

    if (autoColor >= 7) {
      autoColor = 0;
    }
  }
}


// =====================================================
// PROCESS COMMAND
// =====================================================

void processCommand(String command) {

  command.trim();
  command.toUpperCase();

  Serial.print("Command Received: ");
  Serial.println(command);


  // ---------------- RED ----------------

  if (command == "RED") {

    currentMode = STATIC_MODE;

    setColor(255, 0, 0);

    Serial.println("RED MODE");
  }


  // ---------------- GREEN ----------------

  else if (command == "GREEN") {

    currentMode = STATIC_MODE;

    setColor(0, 255, 0);

    Serial.println("GREEN MODE");
  }


  // ---------------- BLUE ----------------

  else if (command == "BLUE") {

    currentMode = STATIC_MODE;

    setColor(0, 0, 255);

    Serial.println("BLUE MODE");
  }


  // ---------------- YELLOW ----------------

  else if (command == "YELLOW") {

    currentMode = STATIC_MODE;

    setColor(255, 255, 0);

    Serial.println("YELLOW MODE");
  }


  // ---------------- PURPLE ----------------

  else if (command == "PURPLE") {

    currentMode = STATIC_MODE;

    setColor(255, 0, 255);

    Serial.println("PURPLE MODE");
  }


  // ---------------- PINK ----------------

  else if (command == "PINK") {

    currentMode = STATIC_MODE;

    setColor(255, 20, 147);

    Serial.println("PINK MODE");
  }


  // ---------------- CYAN ----------------

  else if (command == "CYAN") {

    currentMode = STATIC_MODE;

    setColor(0, 255, 255);

    Serial.println("CYAN MODE");
  }


  // ---------------- WHITE ----------------

  else if (command == "WHITE") {

    currentMode = STATIC_MODE;

    setColor(255, 255, 255);

    Serial.println("WHITE MODE");
  }


  // ---------------- AUTO ----------------

  else if (command == "AUTO") {

    currentMode = AUTO_MODE;

    autoColor = 0;
    previousMillis = 0;

    Serial.println("AUTO COLOR MODE");
  }


  // ---------------- RUNNING ----------------

  else if (
    command == "RUN" ||
    command == "RUNNING" ||
    command == "RUNNING LIGHT"
  ) {

    currentMode = RUN_MODE;

    // Start from LED 1
    runningPosition = 0;

    // No unnecessary waiting before first LED
    previousMillis = millis();

    strip.clear();
    strip.show();

    Serial.println("RUNNING LIGHT MODE");
  }


  // ---------------- OFF ----------------

  else if (command == "OFF") {

    currentMode = OFF_MODE;

    turnOff();
  }


  // ---------------- UNKNOWN COMMAND ----------------

  else {

    Serial.println("UNKNOWN COMMAND");

    Serial.println(
      "RED | GREEN | BLUE | YELLOW | PURPLE"
    );

    Serial.println(
      "PINK | CYAN | WHITE | AUTO | RUNNING | OFF"
    );
  }
}


// =====================================================
// MQTT MESSAGE CALLBACK
// =====================================================

void mqttCallback(
  char* topic,
  byte* payload,
  unsigned int length
) {

  String command = "";

  for (unsigned int i = 0; i < length; i++) {
    command += (char)payload[i];
  }

  Serial.println();
  Serial.println("================================");
  Serial.println("MQTT MESSAGE RECEIVED");

  Serial.print("Topic: ");
  Serial.println(topic);

  Serial.print("Command: ");
  Serial.println(command);

  Serial.println("================================");

  processCommand(command);
}


// =====================================================
// CONNECT TO WIFI
// =====================================================

void connectWiFi() {

  Serial.println();
  Serial.println("Connecting to Wi-Fi...");

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("Wi-Fi Connected!");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


// =====================================================
// CONNECT TO MQTT
// =====================================================

void connectMQTT() {

  while (!mqttClient.connected()) {

    Serial.print("Connecting to MQTT...");

    // Unique MQTT Client ID
    String clientID = "ESP32-SmartLight-";
    clientID += String(random(1000, 9999));

    if (mqttClient.connect(clientID.c_str())) {

      Serial.println("connected!");

      // Subscribe
      mqttClient.subscribe(mqttTopic);

      Serial.print("Subscribed to: ");
      Serial.println(mqttTopic);
    }

    else {

      Serial.print("MQTT connection failed. State = ");
      Serial.println(mqttClient.state());

      Serial.println("Retrying in 2 seconds...");

      delay(2000);
    }
  }
}


// =====================================================
// SERIAL COMMAND INPUT
// =====================================================

void readSerialCommand() {

  if (Serial.available()) {

    String command =
      Serial.readStringUntil('\n');

    processCommand(command);
  }
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("====================================");
  Serial.println("       ESP32 SMART LIGHT STRIP");
  Serial.println("====================================");

  Serial.println("LED Configuration:");
  Serial.println("13 Strips × 2 Pixels = 26 LEDs");

  Serial.println();

  // Initialize LED strip
  strip.begin();

  // Brightness
  strip.setBrightness(80);

  // Initially OFF
  strip.clear();
  strip.show();

  // ---------------- WIFI ----------------

  connectWiFi();

  // ---------------- MQTT ----------------

  mqttClient.setServer(
    mqttServer,
    mqttPort
  );

  mqttClient.setCallback(
    mqttCallback
  );

  connectMQTT();

  // ---------------- START OFF ----------------

  currentMode = OFF_MODE;

  turnOff();

  Serial.println();
  Serial.println("====================================");
  Serial.println("SYSTEM READY");
  Serial.println("====================================");

  Serial.println();
  Serial.println("MQTT Broker:");
  Serial.println(mqttServer);

  Serial.println();
  Serial.println("MQTT Topic:");
  Serial.println(mqttTopic);

  Serial.println();
  Serial.println("Available Commands:");

  Serial.println("RED");
  Serial.println("GREEN");
  Serial.println("BLUE");
  Serial.println("YELLOW");
  Serial.println("PURPLE");
  Serial.println("PINK");
  Serial.println("CYAN");
  Serial.println("WHITE");
  Serial.println("AUTO");
  Serial.println("RUNNING");
  Serial.println("OFF");

  Serial.println();
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // Check MQTT connection
  if (!mqttClient.connected()) {

    connectMQTT();
  }

  // MQTT processing
  mqttClient.loop();

  // Serial command processing
  readSerialCommand();

  // AUTO mode
  if (currentMode == AUTO_MODE) {

    autoColorChange();
  }

  // RUNNING mode
  else if (currentMode == RUN_MODE) {

    runningLight();
  }
}