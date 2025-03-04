#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// LoRaWAN ABP Credentials
static const u1_t NWKSKEY[16] = { 0x25, 0x41, 0xB4, 0xA7, 0x14, 0x59, 0x35, 0x92, 0x73, 0x93, 0x35, 0x86, 0x17, 0x65, 0x1B, 0x9B };
static const u1_t APPSKEY[16] = { 0xBF, 0xF2, 0xCA, 0x2C, 0x71, 0x91, 0xF8, 0x95, 0x36, 0x5C, 0xCF, 0x82, 0x2C, 0x32, 0x24, 0xD1 };
static const u4_t DEVADDR = 0x018229BB;  // Device Address (Big Endian)

// Define missing functions for LMIC
void os_getArtEui (u1_t* buf) { memset(buf, 0, 8); }
void os_getDevEui (u1_t* buf) { memset(buf, 0, 8); }
void os_getDevKey (u1_t* buf) { memcpy(buf, APPSKEY, 16); }  // Using APPSKEY as DevKey

// Pin mapping for ESP32
const lmic_pinmap lmic_pins = {
    .nss  = 18,  // Chip Select (CS)
    .rxtx = LMIC_UNUSED_PIN,
    .rst  = 14,  // Reset
    .dio  = {26, 33, 32}  // DIO0, DIO1, DIO2
};

void setup() {
    Serial.begin(115200);
    Serial.println(F("Starting LoRaWAN ABP Mode..."));

    // LMIC Initialization
    os_init();
    LMIC_reset();

    // Set session keys
    LMIC_setSession(0x1, DEVADDR, (xref2u1_t)NWKSKEY, (xref2u1_t)APPSKEY);

    // Set data rate and transmission power for your region
    LMIC_setDrTxpow(DR_SF9, 14);

    // Select region (e.g., EU868)
    #ifdef CFG_eu868
        LMIC_selectSubBand(1);
    #endif

    // Disable link check validation for ABP
    LMIC_setLinkCheckMode(0);

    // Set ADR (Adaptive Data Rate)
    LMIC_setAdrMode(1);
    
}

void loop() {
    Serial.println(F("Sending Data..."));

    // Prepare payload (example message)
    static uint8_t data[] = "Hello LoRa!";
    LMIC_setTxData2(1, data, sizeof(data) - 1, 0);

    Serial.println(F("Packet queued"));

    // Wait for transmission to complete
    while (LMIC.opmode & OP_TXRXPEND) {
        os_runloop_once();
    }

    Serial.println(F("Transmission completed, waiting..."));
    delay(60000);  // Wait 60 seconds before sending again
}
