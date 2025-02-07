This project is a remotely operated robotic platform powered by an ESP32 microcontroller, controlled via a Python-based GUI over WebSocket communication. The robot features DC motors for movement and an ultrasonic sensor for obstacle detection.

Features:
✅ Remote Control – Move the robot using keyboard inputs via a WebSocket-based Python GUI.
✅ Obstacle Detection – Uses an ultrasonic sensor to prevent collisions.
✅ ESP32-Based – WiFi-enabled microcontroller for wireless operation.
✅ Arduino Integration – ESP32 is programmed using the Arduino framework.

Hardware Requirements:
ESP32-WROOM-32 (In my case: General Driver for Robots)
HC-SR04 Ultrasonic Sensor
RS775 DC Motors + Motor Driver
Li-ion Battery Pack

Software Requirements:
Python 3.12 (Other versions may work, but there could be library dependency issues.)
Arduino IDE 2.3.3.0
All required libraries included in both codes

Setup Instructions:
Install the ESP32 board package in Arduino IDE.
Upload the ESP32_websocket.ino script to the ESP32.
Ensure the correct WiFi credentials are set in the code.
Once the ESP32 IP address appears in the serial monitor, copy it and paste it into the appropriate line in the Python code.
Run pythoncontroller.py to start controlling the robot.

Additional Info about libraries included:
nest-asyncio --> 1.5.8
websocket --> 0.2.1
pynput --> 1.7.7
ArduinoWebsockets by Gil --> 0.5.4

Additonal Borards Manager URLs in Arduno settings:
https://dl.espressif.com/dl/package_esp32_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
