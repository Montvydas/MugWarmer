#include <Arduino.h>
#include <TemperatureSensor.h>
#include <PID_v1.h>

#define PIN_HEATER 3           // Heaing pad connected to this pin
#define ONE_WIRE_BUS 2         // Dallas Temperature sensor
#define DEFAULT_TEMPERATURE 32 // Could store the value in memory and if no value stored then use this

double Setpoint, Input, Output;              // PID variables
double consKp = 30, consKi = 5, consKd = 10; // Tuning Parameters
// double consKp = 1, consKi = 0.05, consKd = 0.25; // Tuning Parameters

// Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

// Create instance of Temperature Sensor
TemperatureSensor temperatureSensor(ONE_WIRE_BUS);

// function signatures
void drawPoint(char *name, float value);

void setup(void)
{
  // start serial port - need to check which value for bluetooth is used
  Serial.begin(115200);
  pinMode(PIN_HEATER, OUTPUT);
  pinMode(5, INPUT_PULLUP);

  // set frequency to 30HZ to be able to display analog signal on the plotter
  TCCR2B = TCCR2B & B11111000 | B00000111; 
  analogWrite(PIN_HEATER, 100);

  if (!temperatureSensor.begin())
  {
    Serial.println(F("Could not initialise temperature sensor, enable DEBUG to see why. Halting!"));
    while (true)
      ;
  }

  // Read the current temperature and set it as PID input
  temperatureSensor.pollTemp();
  Input = temperatureSensor.getTempC();

  // Set the desired temperature
  Setpoint = DEFAULT_TEMPERATURE;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop(void)
{
  while (true)
  {
    drawPoint("Analog", 1.0 * analogRead(A0) / 1023 * 5);
    Serial.println();
  }

  // Bluetooth communication - Maybe check how to create characteristis
  // to read/write and use that instead of Serial UART?
  if (Serial.available() > 0)
  {
    int rec = Serial.read();
    Serial.print("Received: ");
    Serial.println(rec, DEC);
    Setpoint = rec;

    while (Serial.available())
      Serial.read();
  }

  // Read the current temperature and set it as PID input
  temperatureSensor.pollTemp();
  Input = temperatureSensor.getTempC();

  // Compute the PID output and use PWM control the temperature
  myPID.Compute();
  analogWrite(PIN_HEATER, Output);

  drawPoint("Setpoint:", Setpoint);
  drawPoint("Temperature:", Input);
  drawPoint("Output:", Output);
  Serial.println();

  // temperatureSensor.pollTemperature();
  // temperatureSensor.printTemperature(); // Use a simple function to print out the data
  delay(100);
}

void drawPoint(char *name, float value)
{
  Serial.print(name);
  Serial.print(":");
  Serial.print(value, 3);
  Serial.print(",");
}