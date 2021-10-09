/*
 BatteryMonitor.cpp - Library to perform battery level monitoring and battery measurements.
 Created by Montvydas Klumbys, 14 April, 2020.
 */

#ifndef TEMPERATURESENSOR_H_
#define TEMPERATURESENSOR_H_

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"

// #define DEBUG
#define DEFAULT_TEMPERATURE_SENSOR_RESOLUTION 10

class TemperatureSensor
{
public:
    /**
     * @param pin define what is considered to be a critical level in percentage. Default is 10%.
     * @param resolution define what is considered to be a critical level in percentage. Default is 10%.
     */
    TemperatureSensor(uint8_t pin, uint8_t resolution=DEFAULT_TEMPERATURE_SENSOR_RESOLUTION);

    /**
     * Prepare the battery monitoring.
     */
    bool begin();
    void printTemp();
    void pollTemp();
    float getTempC();
private:

    // arrays to hold device address
    DeviceAddress address;
    DallasTemperature dallasSensor;
    OneWire oneWire;
    void printAddr();
    uint8_t pin;    // pin which is used to perform measurements
    uint8_t resolution;    // temperature sensor resolution
};

#endif
