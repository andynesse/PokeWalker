#include "sensor.h"
#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

static Adafruit_MPU6050 mpu;
static SensorData data;

static bool sensorReady = false;

static void scanI2CBus()
{
    Serial.println("Scanning I2C bus...");

    for (uint8_t address = 1; address < 127; ++address) {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();

        if (error == 0) {
            Serial.printf(
                "Found I2C device at 0x%02X\n",
                address
            );
        }
    }

    Serial.println("I2C scan complete.");
}

bool sensorInit()
{
    bool found = false;

    if (mpu.begin(0x68, &Wire)) {
        found = true;
    }
    else if (mpu.begin(0x69, &Wire)) {
        found = true;
    }

    if (!found) {
        Serial.println("MPU6050 not found!");
        scanI2CBus();
        return false;
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);

    // Good starting point for 50 Hz application sampling.
    mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

    sensorReady = true;

    Serial.println("MPU6050 ready!");

    return true;
}

bool sensorUpdate()
{
    if (!sensorReady) {
        return false;
    }

    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    if (!mpu.getEvent(&accel, &gyro, &temp)) {
        Serial.println("Failed to get sensor data!");
        return false;
    }

    data.ax = accel.acceleration.x;
    data.ay = accel.acceleration.y;
    data.az = accel.acceleration.z;

    data.gx = gyro.gyro.x;
    data.gy = gyro.gyro.y;
    data.gz = gyro.gyro.z;

    data.temperature = temp.temperature;

    return true;
}

SensorData getSensorData()
{
    return data;
}

float getMagnitude(float x, float y, float z)
{
    return sqrtf(
        x * x +
        y * y +
        z * z
    );
}

const char *sensorDataToString(const SensorData &data)
{
    static char buffer[100];

    snprintf(
        buffer,
        sizeof(buffer),
        "Accel: %.2f, %.2f, %.2f\n"
        "Gyro: %.2f, %.2f, %.2f\n"
        "Temp: %.2f",
        data.ax,
        data.ay,
        data.az,
        data.gx,
        data.gy,
        data.gz,
        data.temperature
    );

    return buffer;
}