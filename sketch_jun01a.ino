#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ESP32Servo.h>

#define SERVO1_PIN 18
#define SERVO2_PIN 19

Servo servo1;
Servo servo2;

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-cba987654321"

class ServoCallbacks : public BLECharacteristicCallbacks {

  void onWrite(BLECharacteristic *pCharacteristic) {

    String cmd = pCharacteristic->getValue().c_str();

    Serial.print("Received: ");
    Serial.println(cmd);

    if (cmd.startsWith("S1:")) {

      int angle = cmd.substring(3).toInt();

      if (angle >= 0 && angle <= 180) {
        servo1.write(angle);
        Serial.print("Servo 1 -> ");
        Serial.println(angle);
      }
    }

    else if (cmd.startsWith("S2:")) {

      int angle = cmd.substring(3).toInt();

      if (angle >= 0 && angle <= 180) {
        servo2.write(angle);
        Serial.print("Servo 2 -> ");
        Serial.println(angle);
      }
    }
  }
};

void setup() {

  Serial.begin(115200);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);

  servo1.write(90);
  servo2.write(90);

  BLEDevice::init("ESP32 Servo Controller");

  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService =
      pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic =
      pService->createCharacteristic(
          CHARACTERISTIC_UUID,
          BLECharacteristic::PROPERTY_WRITE
      );

  pCharacteristic->setCallbacks(new ServoCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising =
      BLEDevice::getAdvertising();

  pAdvertising->start();

  Serial.println("BLE Servo Controller Ready");
}

void loop() {
  delay(100);
}
