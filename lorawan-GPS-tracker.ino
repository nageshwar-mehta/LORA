#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(2, 3); // Arduino RX, TX to connect to GPS module.

uint8_t datasend[6];     // Used to store GPS data for uploading (3 bytes each for lat and lon)
float flat, flon;

// LoRaWAN session keys and device address
static const u1_t NWKSKEY[16] = { 0x33, 0xB2, 0x62, 0xC4, 0x3F, 0x75, 0x44, 0xB7, 0xE8, 0x41, 0x2D, 0x8D, 0xA2, 0x7D, 0x9C, 0xE4 };
static const u1_t APPSKEY[16] = { 0x55, 0x7E, 0x91, 0x0C, 0x70, 0xBB, 0xFB, 0xE8, 0xDE, 0xD1, 0xA3, 0x85, 0x4A, 0x57, 0xBF, 0x25 };
static const u4_t DEVADDR = 0x260111A8;

const unsigned TX_INTERVAL = 20;  // Define the TX interval

static osjob_t sendjob;  // Declare sendjob

const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

// Define placeholders for required functions
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

// Define the do_send function
void do_send(osjob_t* j) {
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println("OP_TXRXPEND, not sending");
    } else {
        GPSRead();
        GPSWrite();
        LMIC_setTxData2(1, datasend, sizeof(datasend), 0);
        Serial.println("Packet queued");
    }
}

void onEvent(ev_t ev) {
    switch(ev) {
        case EV_TXCOMPLETE:
            Serial.println("EV_TXCOMPLETE (includes waiting for RX windows)");
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
            break;
        default:
            Serial.println("Unknown event");
            break;
    }
}

void setup() {
    Serial.begin(9600);
    ss.begin(9600);  
    os_init();
    LMIC_reset();
    LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
    LMIC_setLinkCheckMode(0);
    LMIC.dn2Dr = DR_SF9;
    LMIC_setDrTxpow(DR_SF7,14);
    do_send(&sendjob);  // Pass sendjob to do_send
}

void GPSRead() {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    flat = (flat == TinyGPS::GPS_INVALID_F_ANGLE) ? 0.0 : flat;
    flon = (flon == TinyGPS::GPS_INVALID_F_ANGLE) ? 0.0 : flon;
}

void GPSWrite() {
    int32_t lat = flat * 10000;
    int32_t lon = flon * 10000;

    datasend[0] = lat;
    datasend[1] = lat >> 8;
    datasend[2] = lat >> 16;

    datasend[3] = lon;
    datasend[4] = lon >> 8;
    datasend[5] = lon >> 16;

    Serial.print("Latitude: "); Serial.println(flat, 6);
    Serial.print("Longitude: "); Serial.println(flon, 6);
}

void loop() {
    os_runloop_once();
}
