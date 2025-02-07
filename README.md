# Remotely-Controlled-Driving-Platform

This project is a remotely operated robotic platform powered by an ESP32 microcontroller, controlled via a Python-based GUI over WebSocket communication. The robot features DC motors for movement and an ultrasonic sensor for obstacle detection.

Features:
1. Remote Control – Move the robot using keyboard inputs via a WebSocket-based Python GUI.
2. Obstacle Detection – Uses an ultrasonic sensor to prevent collisions.
3. ESP32-Based – WiFi-enabled microcontroller for wireless operation.
4. Arduino Integration – ESP32 is programmed using the Arduino framework.

Hardware Requirements:
1. ESP32-WROOM-32 (In my case: General Driver for Robots)
2. HC-SR04 Ultrasonic Sensor
3. RS775 DC Motors + Motor Driver
4. Li-ion Battery Pack

Software Requirements:
1. Python 3.12 (Other versions may work, but there could be library dependency issues.)
2. Arduino IDE 2.3.3.0
3. All required libraries included in both codes

Setup Instructions:
1. Install the ESP32 board package in Arduino IDE.
2. Upload the ESP32_websocket.ino script to the ESP32.
3. Ensure the correct WiFi credentials are set in the code.
4. Once the ESP32 IP address appears in the serial monitor, copy it and paste it into the appropriate line in the Python code.
5. Run pythoncontroller.py to start controlling the robot.

Additional Info about libraries included:
nest-asyncio --> 1.5.8
websocket --> 0.2.1
pynput --> 1.7.7
ArduinoWebsockets by Gil --> 0.5.4

Additonal Borards Manager URLs in Arduno settings:
https://dl.espressif.com/dl/package_esp32_index.json
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
![fotka2](https://github.com/user-attachments/assets/b889c403-3bd1-45b2-9d6d-d609fd53b611)
![fotka3](https://github.com/user-attachments/assets/3590856d-3216-43c8-89af-5ac38025b6ba)
