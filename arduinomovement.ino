//arduino definitions and wifi server
#include <WiFi.h>
#include <ArduinoWebsockets.h>

using namespace websockets;

// WiFi credentials
const char* ssid = "androidap123";
const char* password = "latwehaslo";

// WebSocket server
WebsocketsServer server;
WebsocketsClient client;  // Declare the WebSocket client object

// Motor control pins
const uint16_t PWMA = 25;
const uint16_t PWMB = 26;
const uint16_t AIN2 = 17;
const uint16_t AIN1 = 21;
const uint16_t BIN1 = 22;
const uint16_t BIN2 = 23;

// Ultrasonic sensor pins
const uint8_t TRIG_PIN = 27; // TRIG pin
const uint8_t ECHO_PIN = 16; // ECHO pin

// Distance threshold
const uint16_t STOP_DISTANCE = 10;  // Stop if object is closer than 10 cm

// Timing variables
unsigned long lastDistanceCheck = 0;
const unsigned long distanceInterval = 10; // Measure distance every 1 second

// Global variable to store the latest distance
long lastDistance = 0;

// Initialize motor control pins
void initMotors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}

// Forward movement for Motor A and B
void forwardA(uint16_t pwm) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, pwm);
}

void forwardB(uint16_t pwm) {
  digitalWrite(BIN1, HIGH); // Adjusted for correct forward logic
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, pwm);
}

// Backward movement for Motor A and B
void backwardA(uint16_t pwm) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, pwm);
}

void backwardB(uint16_t pwm) {
  digitalWrite(BIN1, LOW); // Adjusted for correct backward logic
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, pwm);
}

// Turn left by moving Motor A backward and Motor B forward
void turnLeft(uint16_t pwm) {
  backwardA(pwm);
  forwardB(pwm);
}

// Turn right by moving Motor A forward and Motor B backward
void turnRight(uint16_t pwm) {
  forwardA(pwm);
  backwardB(pwm);
}

// Stop both motors
void stopMotors() {
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

// Measure distance using ultrasonic sensor
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration / 58; // Convert duration to cm
  return distance;
}

// Handle incoming WebSocket commands
void handleCommand(String command) {
  if (command == "forward") {
    forwardA(50);
    forwardB(50);
  } else if (command == "backward") {
    backwardA(50);
    backwardB(50);
  } else if (command == "left") {
    turnLeft(50);
  } else if (command == "right") {
    turnRight(50);
  } else if (command == "stop") {
    stopMotors();
  } else {
    Serial.println("Unknown command: " + command);
  }
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server
  server.listen(8080);
  Serial.println("WebSocket Server started!");

  // Initialize motors
  initMotors();

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Accept a new WebSocket client connection if there's none yet
  if (!client.available()) {
    client = server.accept(); // Accept new client connection
    Serial.println("WebSocket client connected!");
  }

  // Measure distance at regular intervals
  unsigned long currentMillis = millis();
  if (currentMillis - lastDistanceCheck >= distanceInterval) {
    lastDistanceCheck = currentMillis;
    lastDistance = measureDistance();
    Serial.print("Distance: ");
    Serial.print(lastDistance);
    Serial.println(" cm");

    // If an object is too close, stop motors
    if (lastDistance <= STOP_DISTANCE) {
      Serial.println("Object detected too close! Stopping motors.");
      stopMotors();
    }
  }

  // Handle WebSocket messages if the client is available
  if (client.available()) {
    auto message = client.readBlocking(); // Blocking read for incoming message
    String command = message.data();      // Extract message content
    Serial.println("Received command: " + command);
    handleCommand(command);               // Execute the corresponding motor command
  }


}
