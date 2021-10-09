/*
 
 Created by Montvydas Klumbys, 5 October, 2020.
 */

#include "TemperatureSensor.h"
#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"

// constructor
TemperatureSensor::TemperatureSensor(uint8_t pin, uint8_t resolution)
{
    this->pin = pin;
    this->resolution = resolution;

    // Setup a oneWire instance to communicate with any OneWire temperature sensor device
    this->oneWire = OneWire(pin);
    // Pass our oneWire reference to Dallas Temperature.
    this->dallasSensor = DallasTemperature(&oneWire);
}

// void ~TemperatureSensor::TemperatureSensor(){
//     this
// }

bool TemperatureSensor::begin()
{
    // locate devices on the bus
    this->dallasSensor.begin();

    if (this->dallasSensor.getDeviceCount() == 0)
    {
#ifdef DEBUG
        Serial.println(F("Temperature Sensor not found!"));
#endif
        return false;
    }

    // Search for devices on the bus and assign the first found device address to this->address.
    if (!this->dallasSensor.getAddress(this->address, 0))
    {
#ifdef DEBUG
        Serial.println(F("Unable to find address for the temperature Sensor!"));
#endif
        return false;
    }
    // Must be called before search()
    //oneWire.reset_search();
    // assigns the first address found to this->address
    // if (!oneWire.search(this->address)) Serial.println("Unable to find address for this->address");

    // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
    this->dallasSensor.setResolution(this->address, this->resolution);

    // show the addresses we found on the bus
#ifdef DEBUG
    Serial.print(F("Temperature Sensor Address: "));
    printAddress();
    Serial.println();

    Serial.print(F("Temperature Sensor Resolution: "));
    Serial.print(this->dallasSensor.getResolution(this->address), DEC);
    Serial.println();

    // report parasite power requirements
    Serial.print(F("Parasite power is: "));
    if (this->dallasSensor.isParasitePowerMode())
        Serial.println(F("ON"));
    else
        Serial.println(F("OFF"));
#endif

    return true;
}

float TemperatureSensor::getTempC()
{
    // method 2 - faster
    return this->dallasSensor.getTempC(this->address);
}

// function to print the temperature for a device
void TemperatureSensor::printTemp()
{
    // method 2 - faster
    float tempC = this->dallasSensor.getTempC(this->address);
    Serial.print(F("Temp C: "));
    Serial.print(tempC);
    Serial.print(F(" Temp F: "));
    Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}

// function to print a device address
void TemperatureSensor::printAddr()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (this->address[i] < 16)
            Serial.print(F("0"));
        Serial.print(this->address[i], HEX);
    }
}

void TemperatureSensor::pollTemp()
{
#ifdef DEBUG
    Serial.println(F("Polling Temperature..."));
#endif
    this->dallasSensor.requestTemperaturesByAddress(this->address);
#ifdef DEBUG
    Serial.println(F("DONE."));
#endif
}