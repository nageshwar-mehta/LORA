**LoRaWAN ABP Communication Guide**

## **Introduction**
This guide provides step-by-step instructions on how to set up and run a LoRaWAN node using the **LoRaMac** library with **Activation by Personalization (ABP)**. The provided C program initializes the LoRaWAN stack, configures ABP credentials, and transmits a simple "Hello" message every 10 seconds.

---

## **Code Explanation**

### **1. Header Files**
```c
#include "LoRaMac.h"
#include "board.h"
```
- `LoRaMac.h`: Provides functions for LoRaWAN communication.
- `board.h`: Handles board-specific hardware initialization.

### **2. Defining ABP Credentials**
```c
#define DEV_ADDR  0x018229BB  // ABP device address
#define NWK_SKEY  { 0x25, 0x41, 0xBA, 0x47, 0x14, 0x59, 0x35, 0x92, 0x73, 0x93, 0x35, 0x86, 0x17, 0x65, 0x1B, 0x9B }
#define APP_SKEY  { 0xBF, 0xF2, 0xCA, 0x2C, 0x71, 0x91, 0xF8, 0x95, 0x36, 0x5C, 0xCF, 0x82, 0x2C, 0x32, 0x24, 0xD1 }
```
These values are used for authentication and encryption in the LoRaWAN network.

### **3. Sending Data Over LoRaWAN**
```c
void SendFrame(void) {
    McpsReq_t mcpsReq;
    mcpsReq.Type = MCPS_UNCONFIRMED;
    mcpsReq.Req.Unconfirmed.fPort = 1;
    mcpsReq.Req.Unconfirmed.fBuffer = txBuffer;
    mcpsReq.Req.Unconfirmed.fBufferSize = sizeof(txBuffer);

    if (LoRaMacMcpsRequest(&mcpsReq) == LORAMAC_STATUS_OK) {
        printf("Packet sent successfully!\n");
    } else {
        printf("Failed to send packet\n");
    }
}
```
- Sends a "Hello" message every **10 seconds** using **unconfirmed messages**.

### **4. Initializing the LoRaWAN Stack**
```c
int main(void) {
    BoardInitMcu();
    BoardInitPeriph();
    LoRaMacInitialization();
```
- Initializes the **MCU**, **peripherals**, and **LoRaWAN stack**.

### **5. Setting ABP Credentials**
```c
    MibRequestConfirm_t mibReq;
    mibReq.Type = MIB_DEV_ADDR;
    mibReq.Param.DevAddr = DEV_ADDR;
    LoRaMacMibSetRequestConfirm(&mibReq);
```
- Configures **Device Address, Network Session Key, and Application Session Key**.

### **6. Sending Data in a Loop**
```c
    while (1) {
        SendFrame();
        sleep(10);
    }
    return 0;
}
```
- Transmits data every **10 seconds** indefinitely.

---

## **How to Run This Code?**

### **1. Prerequisites**
- **Hardware:** LoRa module (e.g., SX1276, RFM95) connected to a microcontroller (e.g., STM32, ESP32).
- **Software:** GCC for ARM, PlatformIO, or Arduino IDE.

### **2. Compilation & Upload**
- **Using GCC ARM:**
  ```sh
  arm-none-eabi-gcc -o lorawan.out main.c -lloRaMac -lboard
  ```
- **Using PlatformIO:**
  ```sh
  pio run --target upload
  ```

### **3. Monitoring Serial Output**
- Use a serial monitor:
  ```sh
  minicom -b 115200 -D /dev/ttyUSB0
  ```
- Expected output:
  ```
  Packet sent successfully!
  ```

### **4. Checking Data on LoRaWAN Network**
- Connect to **a LoRa gateway**.
- Use **The Things Network (TTN)** or a **private LoRaWAN server** to verify received data.

---

## **Final Notes**
- Modify `DEV_ADDR`, `NWK_SKEY`, and `APP_SKEY` as per your LoRaWAN provider.
- Ensure **LoRaWAN frequency** matches your region (EU868, US915, etc.).
- Use **Confirmed Messages (MCPS_CONFIRMED)** if acknowledgment is required.
- Prefer **OTAA (Over-The-Air Activation)** for better security instead of ABP.




