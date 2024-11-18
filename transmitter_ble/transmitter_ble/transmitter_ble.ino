#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SWITCH_PIN 22
#define RGB_BUILTIN 8  // GPIO for built-in RGB LED

// UUIDs for the service and characteristic
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool switchState = false;

void setup() {
  Serial.begin(115200);
  
  // Initialize Switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  
  // Initialize RGB LED to red
  neopixelWrite(RGB_BUILTIN, 255, 0, 0);  // Red color
  
  // Start BLE
  BLEDevice::init("ESP32_BLE_Transmitter");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE characteristic for the RGB color state
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  // Start the service
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void loop() {
  if (digitalRead(SWITCH_PIN) == LOW) { // Switch is pressed
    // Change RGB LED to green and update BLE characteristic
    neopixelWrite(RGB_BUILTIN, 0, 255, 0);  // Green color
    pCharacteristic->setValue("GREEN");
    pCharacteristic->notify();  // Notify connected client of the change
    delay(200);  // Debounce delay
  } else {
    // Change RGB LED to red and update BLE characteristic
    neopixelWrite(RGB_BUILTIN, 255, 0, 0);  // Red color
    pCharacteristic->setValue("RED");
    pCharacteristic->notify();  // Notify connected client of the change
    delay(200);  // Debounce delay
  }
}