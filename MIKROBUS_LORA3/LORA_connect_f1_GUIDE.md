 **complete step-by-step guide** covering both the **hardware** and **software** parts of setting up LoRaWAN ABP mode with ESP32 and a LoRa module using the MCCI LoRaWAN LMIC library.  

---

# **LoRaWAN ABP Mode with ESP32 and LoRa Module – Complete Guide**  
This guide walks you through **hardware setup**, **code implementation**, and **troubleshooting** to ensure successful LoRaWAN communication.

---

## **1️⃣ Introduction**
LoRaWAN (Long Range Wide Area Network) enables low-power, long-range communication for IoT devices. This guide explains how to set up **ESP32 with a LoRa module (SX1276/78) using the ABP (Activation by Personalization) method**.

---

## **2️⃣ Components Required**
- **ESP32 Development Board** (NodeMCU ESP32, TTGO LoRa32, etc.)
- **LoRa Module** (RA-02 SX1276, RFM95, etc.)
- **Jumper Wires**
- **Power Source** (USB or battery)

---

## **3️⃣ Hardware Setup (Wiring)**
Connect your **ESP32** and **LoRa module** as follows:

| **ESP32 Pin** | **LoRa Module (SX1276)** |
|--------------|-----------------------|
| **3.3V**      | VCC (Ensure 3.3V Logic) |
| **GND**       | GND |
| **18**        | NSS (Chip Select) |
| **23**        | SCK (SPI Clock) |
| **19**        | MISO (SPI MISO) |
| **22**        | MOSI (SPI MOSI) |
| **14**        | RST (Reset) |
| **26**        | DIO0 (Interrupt Pin) |
| **33**        | DIO1 |
| **32**        | DIO2 |

> ⚠️ **Important:** The LoRa module operates at **3.3V logic**, so do **NOT** connect it to 5V! Use **level shifters** if needed.

---

## **4️⃣ Software Setup**
### **Step 1: Install Required Libraries**
1. Open **Arduino IDE**.
2. Go to **Sketch → Include Library → Manage Libraries**.
3. Search for **"MCCI LoRaWAN LMIC"**.
4. Install version **4.1.1**.

Alternatively, for **PlatformIO** users, run:
```sh
pio lib install "MCCI LoRaWAN LMIC" --version 4.1.1
```

---

## **5️⃣ Understanding the Code**
### **Step 1: Include Required Libraries**
```cpp
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
```
- `lmic.h` and `hal/hal.h`: Libraries for LoRaWAN communication.
- `SPI.h`: Handles SPI communication.

---

### **Step 2: Define LoRaWAN ABP Credentials**
```cpp
static const u1_t NWKSKEY[16] = { 0x25, 0x41, 0xB4, 0xA7, 0x14, 0x59, 0x35, 0x92, 0x73, 0x93, 0x35, 0x86, 0x17, 0x65, 0x1B, 0x9B };
static const u1_t APPSKEY[16] = { 0xBF, 0xF2, 0xCA, 0x2C, 0x71, 0x91, 0xF8, 0x95, 0x36, 0x5C, 0xCF, 0x82, 0x2C, 0x32, 0x24, 0xD1 };
static const u4_t DEVADDR = 0x018229BB;
```
- **NWKSKEY**: Network session key.
- **APPSKEY**: Application session key.
- **DEVADDR**: Unique device address assigned by the network provider.

> 💡 **Get these credentials from your LoRaWAN network server (e.g., The Things Network, ChirpStack).**

---

### **Step 3: Define LoRa Pin Mapping**
```cpp
const lmic_pinmap lmic_pins = {
    .nss  = 18,  
    .rst  = 14,  
    .dio  = {26, 33, 32}  
};
```
- `nss`: Chip select pin.
- `rst`: Reset pin.
- `dio`: Interrupt pins for LoRa.

> **Modify these pins if using a different board!**

---

### **Step 4: Initialize LoRa and Set Configuration**
```cpp
void setup() {
    Serial.begin(115200);
    Serial.println(F("Starting LoRaWAN ABP Mode..."));

    os_init();
    delay(100);
    LMIC_reset();

    LMIC_setSession(0x1, DEVADDR, (xref2u1_t)NWKSKEY, (xref2u1_t)APPSKEY);

    LMIC_setDrTxpow(DR_SF9, 14);
    
    #ifdef CFG_us915
        LMIC_selectSubBand(1);
    #endif

    LMIC_setLinkCheckMode(0);
    Serial.println(F("LoRaWAN Setup Complete!"));
}
```
- `os_init()`: Initializes LMIC stack.
- `LMIC_reset()`: Resets the LoRa chip.
- `LMIC_setSession()`: Sets ABP credentials.
- `LMIC_setDrTxpow(DR_SF9, 14)`: Sets data rate (SF9) and transmission power (14 dBm).
- `LMIC_setLinkCheckMode(0)`: Disables link check (since ABP does not need it).

---

### **Step 5: Transmit Data Periodically**
```cpp
void loop() {
    Serial.println(F("Sending Data..."));

    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("Transmission pending, skipping..."));
        return;
    }

    static uint8_t data[] = "Hello LoRa!";
    LMIC_setTxData2(1, data, sizeof(data) - 1, 0);

    Serial.println(F("Packet queued"));

    while (LMIC.opmode & OP_TXRXPEND) {
        os_runloop_once();
    }

    Serial.println(F("Transmission completed, waiting..."));
    delay(60000);
}
```
- **Checks if transmission is pending** before sending new data.
- **Prepares a payload** (`"Hello LoRa!"`).
- **Queues the packet** for transmission.
- **Waits 60 seconds** before sending the next message.

---

## **6️⃣ Upload & Test**
1. Connect your **ESP32** to your computer via USB.
2. **Select Board & Port** in Arduino IDE:
   - **Tools → Board → ESP32 Dev Module**
   - **Tools → Port → Select your ESP32 port**
3. Click **Upload** (✅).
4. Open **Serial Monitor** (Baud Rate: 115200).
5. Check for **“LoRaWAN Setup Complete!”** and **packet transmission logs**.

---

## **7️⃣ Troubleshooting**
### **Common Issues & Fixes**
| Issue | Possible Cause | Solution |
|-------|---------------|----------|
| `FAILURE` at `osmic.c:53` | Wrong SPI pins | Check `lmic_pinmap` matches your board |
| No response on Serial Monitor | Baud rate mismatch | Set Serial Monitor to `115200` |
| Transmission not working | Wrong LoRaWAN region | Set correct frequency in `config.h` |
| LMIC hangs at startup | Power issue | Ensure 3.3V power supply |

---

## **8️⃣ Summary**
🔹 **Connected ESP32 to LoRa module**  
🔹 **Configured ABP credentials**  
🔹 **Initialized LMIC for LoRaWAN**  
🔹 **Sent periodic data over LoRaWAN**  

🎯 **Next Steps:**  
1. **Register your device** on The Things Network (TTN).  
2. **Monitor received packets** on TTN console.  
3. **Integrate with cloud dashboards** (Node-RED, MQTT, etc.).  

🚀 Now your **ESP32 LoRa device** is ready for **IoT applications!** 🚀
