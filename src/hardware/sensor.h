#ifndef SENSOR_H
#define SENSOR_H

struct SensorData {
    float ax;
    float ay;
    float az;

    float gx;
    float gy;
    float gz;

    float temperature;
};

bool sensorInit();
bool sensorUpdate();
bool sensorIsReady();

SensorData getSensorData();

float getMagnitude(float x, float y, float z);

const char *sensorDataToString(const SensorData &data);

#endif