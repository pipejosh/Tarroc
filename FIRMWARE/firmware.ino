#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

int mos1 = 2;
int mos2 = 3;
int mos3 = 4;
int btn = 10;

long start;
long last = 0;

float groundPressure;

bool flight = false;

void setup() {

  Serial.begin(115200);
  Wire.begin();

  pinMode(mos1, OUTPUT);
  pinMode(mos2, OUTPUT);
  pinMode(mos3, OUTPUT);
  pinMode(btn, INPUT_PULLUP);

  digitalWrite(mos1, LOW);
  digitalWrite(mos2, LOW);
  digitalWrite(mos3, LOW);

  // BMP270
  if (!bmp.begin(0x76)) {
    Serial.println("BMP error");
    while (1);
  }

  // MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU error");
    while (1);
  }

  groundPressure = bmp.readPressure() / 100.0;

  Serial.println("Flight computer ready");
  Serial.print("Ground pressure: ");
  Serial.println(groundPressure);
}

void loop() {

  if (digitalRead(btn) == LOW && flight == false) {
    flight = true;
    start = millis();
    last = millis();

    Serial.println("START");

    delay(500);
  }

  if (flight) {

    long t = millis() - start;

    // 30 seconds
    if (t > 30000) {
      digitalWrite(mos1, HIGH);
    }

    // 40 seconds
    if (t > 40000) {
      digitalWrite(mos2, HIGH);
    }

    // 55 seconds
    if (t > 55000) {
      digitalWrite(mos3, HIGH);
    }

    if (millis() - last >= 1000) {

      last = millis();

      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      float pressure = bmp.readPressure() / 100.0;
      float altitude = bmp.readAltitude(groundPressure);

      Serial.print(t / 1000);
      Serial.print("s ");

      Serial.print("ALT:");
      Serial.print(altitude);
      Serial.print("m ");

      Serial.print("P:");
      Serial.print(pressure);
      Serial.print("hPa ");

      Serial.print("GX:");
      Serial.print(g.gyro.x);
      Serial.print(" ");

      Serial.print("GY:");
      Serial.print(g.gyro.y);
      Serial.print(" ");

      Serial.print("GZ:");
      Serial.println(g.gyro.z);
    }
  }
}
