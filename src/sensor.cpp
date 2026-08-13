#include "sensor.h"
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

static Adafruit_MPU6050 mpu;
static SensorData data;
float baselineMagnitude = 0.0f;
static bool sensorReady = false;
static bool sensorCalibrated = false;
static float calibrationMagnitudeSum = 0.0f;
static unsigned int calibrationSampleCount = 0;
static const unsigned int calibrationTargetSamples = 200;

static void scanI2CBus()
{
    Serial.println("Scanning I2C bus...");

    for (uint8_t address = 1; address < 127; ++address) {
        Wire.beginTransmission(address);
        uint8_t error = Wire.endTransmission();
        if (error == 0) {
            Serial.printf("Found I2C device at 0x%02X\n", address);
        }
    }

    Serial.println("I2C scan complete.");
}

bool sensorInit()
{
    if (!mpu.begin(0x68, &Wire) && !mpu.begin(0x69, &Wire)) {
        Serial.println("MPU6050 not found!");
        scanI2CBus();
        return false;
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    sensorReady = true;
    sensorCalibrated = false;
    calibrationMagnitudeSum = 0.0f;
    calibrationSampleCount = 0;
    baselineMagnitude = 0.0f;

    Serial.println("MPU6050 ready!");

    return true;
}

bool sensorUpdate()
{
    if (!sensorReady) {
        return false;
    }

    sensors_event_t accel, gyro, temp;
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

    if (!sensorCalibrated) {
        calibrationMagnitudeSum += sqrt(data.ax * data.ax + data.ay * data.ay + data.az * data.az);
        calibrationSampleCount++;

        if (calibrationSampleCount >= calibrationTargetSamples) {
            baselineMagnitude = calibrationMagnitudeSum / calibrationSampleCount;
            sensorCalibrated = true;
            Serial.println("MPU6050 calibrated!");
        }
    }

    return true;

}

SensorData getSensorData()
{
    return data;
}

float getMagnitude(float x, float y, float z)
{
    return sqrt(x * x + y * y + z * z) - baselineMagnitude;
}

bool sensorIsCalibrated()
{
    return sensorCalibrated;
}