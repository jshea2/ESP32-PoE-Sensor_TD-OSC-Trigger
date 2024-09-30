
# ESP32-PoE Sensor -> TD OSC Trigger

Use ESP32-PoE with sensor to send OSC filtered by Touchdesigner.

https://github.com/user-attachments/assets/93a61eb2-cb9a-4f0a-9d0c-53f0e3ab7c55


## Hardware Requirements:
- [ESP32-PoE-ISO](https://www.digikey.com/en/products/detail/olimex-ltd/ESP32-POE-ISO/10258716)
- [Sensor Example](https://www.amazon.com/HiLetgo-TTP223B-Capacitive-Digital-Raspberry/dp/B00HFQEFWQ/ref=sr_1_1_sspa)
- [PoE Switch or Injector](https://www.amazon.com/Injector-Compliant-Compatible-TL-POE150S-TPE-113GI/dp/B09BFBM6PQ/ref=sr_1_9)
- [Breadboard Jumper Wire](https://www.amazon.com/Elegoo-EL-CP-004-Multicolored-Breadboard-arduino/dp/B01EV70C78/)

## Setup:
- Configure all settings
  - `sensorOSCAddress` and `pingOSCAddress` should change the first number to what device is set. "/?/ping" and "/?/sensor"
    - This is used for multiple sensors
  - `remoteIP` and `remotePort` are your computers ip address and what port OSC will be sent.
  - `triggerOnHigh` if true will send OSC if a `HIGH` is sent via the sensor. Some sensors act opposite
  - `touchPin` is the data pin the sensor sends data to
  - `localIP` and `subnet` should configure to be in the same range as your computers network.
- Flash ESP32-PoE-ISO with file
- Connect Sensor data pin to whatever `touchPin` is on ESP32-PoE-ISO
- Connect computer to ESP32-PoE-ISO via PoE network
- Open Touchdesigner project
  - It should read "Online" if connected. This knows its online because the ESP32-PoE sends a "/*/ping" to check heartbeat.

Features:
- Touchdesigner UI
- Online / Offline indicator
- Feild for amount of time for cooldown state. This is incase the sensor is triggered too quickly. 
- It also shows an indicator everytime it is touched
- "Test" button is included to test trigger without ESP32-PoE online
- Feilds to edit ping and sensor trigger OSC address
- OSC Out Client configure
  - IP, Port, OSC Address & Argument