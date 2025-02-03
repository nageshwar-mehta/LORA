#include <SPI.h>

// Define the pin mapping for ESP12-F
#define NSS_PIN 15   // D8 (Chip Select)
#define RESET_PIN 2  // D4 (Reset)
#define DIO0_PIN 12  // D6 (Interrupt Pin)

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("SPI Continuous Data Reading Example");

  // Configure pins
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  delay(100);
  digitalWrite(RESET_PIN, LOW);
  delay(100);

  pinMode(NSS_PIN, OUTPUT);
  digitalWrite(NSS_PIN, HIGH); // Set NSS high to start

  // Initialize SPI (no parameters needed for ESP8266)
  SPI.begin();
  Serial.println("SPI Initialized");
}

void loop() {
  // String to send
  const char *message = "Hello, LoRa!";
  
  // Send data
  sendString(message);
  
  // Read incoming data from MISO
  Serial.println("Reading incoming data...");
  readDataFromMISO();

  delay(1000); // Wait before next iteration
}

// Function to send a string over SPI
void sendString(const char *str) {
  const char *originalStr = str; // Store the original pointer to the string
  digitalWrite(NSS_PIN, LOW); // Select the LoRa module

  while (*str) { // Loop through each character in the string
    SPI.transfer(*str); // Send character over SPI
    str++; // Move to the next character
  }

  digitalWrite(NSS_PIN, HIGH); // Deselect the LoRa module
  Serial.print("String sent! : ");
  Serial.println(originalStr); // Print the original string
}


// Function to continuously read data from MISO
void readDataFromMISO() {
  digitalWrite(NSS_PIN, LOW); // Select the LoRa module

  // Read data until no more is available
  while (true) {
    uint8_t receivedData = SPI.transfer(0x00); // Send dummy byte to read data
    if (receivedData != 0) { // Check if data is valid (non-zero)
      Serial.print("Received: ");
      Serial.println((char)receivedData); // Print as a character
    } else {
      break; // Exit loop if no data is received
    }
  }

  digitalWrite(NSS_PIN, HIGH); // Deselect the LoRa module
}
