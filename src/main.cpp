#include <ETH.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

// Adjustable variables for OSC configuration
const char* sensorOscAddress = "/1/sensor";  // OSC address for touch sensor
const char* pingOscAddress = "/1/ping";      // OSC address for periodic ping
const char* remoteIP = "192.168.0.10";       // IP address to send OSC messages to
const int remotePort = 8000;                 // Port to send OSC messages
bool triggerOnHigh = false;  // Set to true for triggering on HIGH, false for triggering on LOW

// Define the capacitive touch pin
const int touchPin = 5;  // Use GPIO5 for TTP223B sensor

// Network configuration
IPAddress local_IP(192, 168, 0, 5); // Static IP
IPAddress gateway(192, 168, 0, 1);  // Replace with your network gateway
IPAddress subnet(255, 255, 255, 0); // Subnet mask

// UDP client for OSC
WiFiUDP Udp;

// Debounce settings
unsigned long lastTouchTime = 0;  // Last time a touch was detected
const unsigned long debounceDelay = 50;  // Minimum time between touches (in milliseconds)

// Timer settings for sending "/1/ping"
unsigned long lastPingTime = 0;
const unsigned long pingInterval = 5000;  // 5 seconds in milliseconds

static bool eth_connected = false;

// React to Ethernet events:
void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {

    case ARDUINO_EVENT_ETH_START:
      Serial.println("ETH Started");
      ETH.setHostname("esp32-ethernet");

      // Set the static IP configuration here
      if (!ETH.config(local_IP, gateway, subnet)) {
        Serial.println("Failed to configure Static IP");
      } else {
        Serial.print("Static IP set to: ");
        Serial.println(ETH.localIP());
      }
      break;

    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;

    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("Got an IP Address for ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;

      // Initialize UDP connection for OSC
      Udp.begin(remotePort);  // Open the UDP port for communication

      break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;

    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;

    default:
      break;
  }
}

// Function to send OSC messages
void sendOSCMessage(const char* address)
{
  OSCMessage msg(address);
  Udp.beginPacket(remoteIP, remotePort);
  msg.send(Udp);  // Send the OSC message
  Udp.endPacket();  // End the UDP packet
  msg.empty();  // Empty the OSC message
}

// Initializing everything at start up / after reset:
void setup()
{
  delay(500);
  Serial.begin(115200);
  Serial.print("Setup...");

  WiFi.onEvent(WiFiEvent);
  Serial.print("Registering event handler for ETH events...");

  Serial.print("Starting ETH interface...");
  ETH.begin();

  // Initialize touch sensor as input
  pinMode(touchPin, INPUT);  // Set GPIO5 as input for TTP223B sensor

  Serial.println("Touch sensor initialized.");
}

void loop()
{
  unsigned long currentTime = millis();

  // Check the state of the touch sensor
  int sensorState = digitalRead(touchPin);

  // Trigger based on the configured triggerOnHigh value
  if ((triggerOnHigh && sensorState == HIGH) || (!triggerOnHigh && sensorState == LOW)) {
    // Only register a touch if it's been more than debounceDelay since the last one
    if (currentTime - lastTouchTime > debounceDelay) {
      Serial.println("Touch detected on TTP223B!");
      sendOSCMessage(sensorOscAddress);  // Send OSC message for touch
      lastTouchTime = currentTime;
    }
  }

  // Send a "/1/ping" OSC message every 5 seconds
  if (currentTime - lastPingTime > pingInterval) {
    Serial.println("Sending /1/ping OSC message");
    sendOSCMessage(pingOscAddress);
    lastPingTime = currentTime;
  }
}
