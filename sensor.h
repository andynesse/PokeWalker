#ifndef SENSOR_H
#define SENSOR_H

struct SensorData
{
    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;

    float temperature;
};

extern float baselineMagnitude;

bool sensorInit();

bool sensorUpdate();

SensorData getSensorData();

float getMagnitude(float x, float y, float z);

bool sensorIsCalibrated();

#endif