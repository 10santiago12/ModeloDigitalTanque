#include <Arduino.h>
#include <WiFi.h>                   // Standard WiFi library for ESP32
#include <ModbusIP_ESP8266.h>       // Modbus TCP server implementation
#include <PubSubClient.h>           // MQTT client

// ----------- WiFi credentials -----------
#define WIFI_NAME       "Hotspot"
#define WIFI_PASSWORD   "Santiago1210"

// ----------- Ubidots STEM -----------
#define UBIDOTS_TOKEN   "BBUS-pFREBdzamEXnQvOOpsILJKFsQqeKlE"
#define DEVICE_LABEL    "tanque"

// ----------- Discrete Inputs (read by CODESYS, FC2): B1, B2, B3 -----------
const int DiscreteInputsAddresses[] = {0, 1, 2};
const int DiscreteInputPins[] = {4, 16, 17};

// ----------- Coils (written by CODESYS, FC15): Vacio, Bajo, Correcto, Alto, Error -----------
const int CoilsAddresses[] = {0, 1, 2, 3, 4};
const int CoilsPins[] = {18, 19, 21, 22, 23};

const int NUM_INPUTS = sizeof(DiscreteInputPins) / sizeof(DiscreteInputPins[0]);
const int NUM_COILS = sizeof(CoilsPins) / sizeof(CoilsPins[0]);

// ----------- Ubidots MQTT -----------
const char* MQTT_HOST = "industrial.api.ubidots.com";
const int MQTT_PORT = 1883;
const char* INPUT_LABELS[] = {"b1", "b2", "b3"};
const char* COIL_LABELS[] = {"vacio", "bajo", "correcto", "alto", "error"};
const char* REMOTE_LABELS[] = {"b1_remoto", "b2_remoto", "b3_remoto"};

const unsigned long RECONNECT_MS = 5000;
const unsigned long MIN_PUBLISH_MS = 250;       // rate limit between publishes
const unsigned long HEARTBEAT_MS = 30000;       // publish even without changes

ModbusIP mb;
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

bool remoteState[3] = {false, false, false};
bool lastPublished[8] = {false};
bool publishedOnce = false;
unsigned long lastReconnect = 0;
unsigned long lastPublish = 0;

// Remote command from Ubidots: topic ends with /<variable>/lv, payload is the value.
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  char buf[16];
  unsigned int n = length < sizeof(buf) - 1 ? length : sizeof(buf) - 1;
  memcpy(buf, payload, n);
  buf[n] = '\0';
  bool value = atof(buf) > 0.5;

  for (int i = 0; i < 3; i++) {
    char suffix[32];
    snprintf(suffix, sizeof(suffix), "/%s/lv", REMOTE_LABELS[i]);
    if (strstr(topic, suffix) != nullptr) {
      remoteState[i] = value;
      Serial.printf("Remote %s = %d\n", REMOTE_LABELS[i], value);
    }
  }
}

void connectMqtt() {
  String clientId = String("esp32-tanque-") + String((uint32_t)ESP.getEfuseMac(), HEX);
  if (mqtt.connect(clientId.c_str(), UBIDOTS_TOKEN, "")) {
    Serial.println("MQTT connected to Ubidots");
    for (int i = 0; i < 3; i++) {
      char topic[96];
      snprintf(topic, sizeof(topic), "/v1.6/devices/%s/%s/lv", DEVICE_LABEL, REMOTE_LABELS[i]);
      mqtt.subscribe(topic);
    }
  } else {
    Serial.printf("MQTT connect failed, rc=%d\n", mqtt.state());
  }
}

// Publishes B1..B3 (effective values) and the five level LEDs when something changed.
void publishState(const bool current[8]) {
  bool changed = !publishedOnce;
  for (int i = 0; i < 8 && !changed; i++) {
    if (current[i] != lastPublished[i]) changed = true;
  }
  unsigned long now = millis();
  bool heartbeat = now - lastPublish >= HEARTBEAT_MS;
  if (!(changed || heartbeat)) return;
  if (now - lastPublish < MIN_PUBLISH_MS) return;

  char payload[256];
  int len = snprintf(payload, sizeof(payload), "{");
  for (int i = 0; i < 3; i++) {
    len += snprintf(payload + len, sizeof(payload) - len, "\"%s\":%d,", INPUT_LABELS[i], current[i]);
  }
  for (int i = 0; i < NUM_COILS; i++) {
    len += snprintf(payload + len, sizeof(payload) - len, "\"%s\":%d%s", COIL_LABELS[i], current[3 + i], i < NUM_COILS - 1 ? "," : "");
  }
  snprintf(payload + len, sizeof(payload) - len, "}");

  char topic[64];
  snprintf(topic, sizeof(topic), "/v1.6/devices/%s", DEVICE_LABEL);
  if (mqtt.publish(topic, payload)) {
    memcpy(lastPublished, current, sizeof(lastPublished));
    publishedOnce = true;
    lastPublish = now;
  }
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < NUM_INPUTS; i++) {
    pinMode(DiscreteInputPins[i], INPUT_PULLUP);   // Dipswitch ON pulls the pin to GND
    mb.addIsts(DiscreteInputsAddresses[i]);
  }
  for (int i = 0; i < NUM_COILS; i++) {
    pinMode(CoilsPins[i], OUTPUT);
    mb.addCoil(CoilsAddresses[i]);
  }

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }

  mb.server();
  Serial.println("\n Modbus TCP Server started!");

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setBufferSize(512);
  mqtt.setCallback(onMqttMessage);
}

void loop() {
  mb.task();

  // ---- Inputs: dipswitch OR remote command, exposed to CODESYS ----
  bool current[8];
  for (int i = 0; i < NUM_INPUTS; i++) {
    bool dip = digitalRead(DiscreteInputPins[i]) == LOW;   // Inverted: switch ON = true
    current[i] = dip || remoteState[i];
    mb.Ists(DiscreteInputsAddresses[i], current[i]);
  }

  // ---- Outputs: LEDs follow the coils written by CODESYS ----
  for (int i = 0; i < NUM_COILS; i++) {
    bool coil = mb.Coil(CoilsAddresses[i]);
    digitalWrite(CoilsPins[i], coil);
    current[3 + i] = coil;
  }

  // ---- MQTT (non-blocking reconnect so Modbus keeps being served) ----
  if (WiFi.status() == WL_CONNECTED) {
    if (!mqtt.connected()) {
      unsigned long now = millis();
      if (now - lastReconnect >= RECONNECT_MS) {
        lastReconnect = now;
        connectMqtt();
      }
    } else {
      mqtt.loop();
      publishState(current);
    }
  }
}
