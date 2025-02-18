**LoRaWAN ABP Communication Guide for Arduino IDE**

## **Introduction**
This guide provides step-by-step instructions to set up and run a LoRaWAN node using the **Arduino IDE** with **Activation by Personalization (ABP)**. The modified C++ program initializes the LoRaWAN stack, configures ABP credentials, and transmits a simple "Hello" message periodically.

---

## **Hardware Requirements**
- **Microcontroller:** ESP32, Arduino (with SPI support)
- **LoRa Module:** RFM95W (SX1276-based) or similar
- **Connections:** SPI interface (MISO, MOSI, SCK, NSS, DIO pins)

---

## **Code Explanation**

### **1. Include Necessary Libraries**
```cpp
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
```
- `lmic.h`: Manages LoRaWAN communication.
- `hal/hal.h`: Provides hardware abstraction layer.
- `SPI.h`: Enables SPI communication with the LoRa module.

### **2. Define ABP Credentials**
```cpp
#define DEVADDR  0x018229BB // ABP device address
static const u1_t NWKSKEY[16] = { 0x25, 0x41, 0xBA, 0x47, 0x14, 0x59, 0x35, 0x92, 0x73, 0x93, 0x35, 0x86, 0x17, 0x65, 0x1B, 0x9B };
static const u1_t APPSKEY[16] = { 0xBF, 0xF2, 0xCA, 0x2C, 0x71, 0x91, 0xF8, 0x95, 0x36, 0x5C, 0xCF, 0x82, 0x2C, 0x32, 0x24, 0xD1 };
```
- The **Device Address**, **Network Session Key**, and **Application Session Key** are used for authentication and encryption.

### **3. Configure LoRa Module Pin Mapping**
```cpp
const lmic_pinmap lmic_pins = {
    .nss = 18,   // Chip Select (CS)
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,   // Reset Pin
    .dio = {26, 33, 32}, // DIO0, DIO1, DIO2
};
```
- Defines the SPI and DIO pin mappings based on the microcontroller being used.

### **4. Event Handling for LoRa Transmission**
```cpp
void onEvent(ev_t ev) {
    switch(ev) {
        case EV_TXCOMPLETE:
            Serial.println("Packet Sent!");
            break;
        default:
            break;
    }
}
```
- Handles **LoRaWAN transmission events**, ensuring proper communication with the network.

### **5. Function to Send Data**
```cpp
void sendData() {
    static uint8_t mydata[] = "Hello";
    LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
}
```
- Sends the "Hello" message over LoRaWAN on **port 1**.

### **6. Setup Function for Initialization**
```cpp
void setup() {
    Serial.begin(115200);
    SPI.begin(5, 19, 27, 18); // MISO, MOSI, SCK, CS
    os_init();
    LMIC_reset();
    
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    LMIC_selectSubBand(1);
    LMIC_setLinkCheckMode(0);
    LMIC_setDrTxpow(DR_SF7, 14);
    
    sendData();
}
```
- Initializes **serial communication, SPI, and LoRaWAN settings**.
- Configures the **ABP session, sub-band, and transmission power**.

### **7. Loop Function to Maintain LoRa Communication**
```cpp
void loop() {
    os_runloop_once();
}
```
- Ensures LoRaWAN events are processed continuously.

---

## **How to Run This Code?**

### **1. Install Required Libraries**
- Open **Arduino IDE** and navigate to **Sketch → Include Library → Manage Libraries**.
- Search for `MCCI LoRaWAN LMIC` and install it.

### **2. Configure Board and Port**
- Connect your **ESP32/Arduino board**.
- Select the correct **Board** and **COM Port** under **Tools → Board**.

### **3. Upload the Code**
- Click the **Upload** button.
- Open the **Serial Monitor** (115200 baud rate) to view logs.

### **4. Expected Serial Output**
```
Packet Sent!
```
- If successfully transmitted, you will see this message.
- Use **The Things Network (TTN)** or a private **LoRaWAN server** to verify reception.

---

## **Troubleshooting**

### **1. Compilation Issues**
- Ensure **MCCI LoRaWAN LMIC** library is installed.
- Use an **ESP32 or Arduino** with SPI support.

### **2. No Data on LoRaWAN Network**
- Check if the **LoRaWAN gateway** is online.
- Verify that **DEVADDR, NWKSKEY, and APPSKEY** match your LoRaWAN provider.
- Ensure the **frequency band** matches your region (EU868, US915, etc.).

### **3. Incorrect Pin Mapping**
- If using a different board, adjust the **SPI and DIO pins** accordingly.

---

## **Conclusion**
- This guide provides an **Arduino-compatible** LoRaWAN ABP implementation.
- Modify **LoRa credentials and pin configurations** based on your setup.
- Prefer **OTAA (Over-The-Air Activation)** for enhanced security.

For additional support, refer to **MCCI LoRaWAN LMIC documentation** or reach out to the LoRa community! 🚀

