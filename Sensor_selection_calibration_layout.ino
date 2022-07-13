// This library is not required in Arduino IDE
//#include <Arduino.h>
// Including the libraries needed to use the Temperature sensor DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

// Library needed for the ADC Module
#include <Adafruit_ADS1X15.h>

// Initialising the ADC module
Adafruit_ADS1115 eadc;

// LED Pins
int LED_base = 3; // pH <7 (green)
int LED_acid = 4; // pH >7 (red)
// int LED_EC = 5;   // Electrical conductivity < 1.2dS m-1 (yellow)
int LED_Lum = 6;  // Light sensitivity < 3000 Lux (blue)
int LED_temp = 7; // Actuator power on/off (green)

// Initialising the variable needed to collect data from the Phototransistors
int photoSensorValue = 0;
int photoSensorValue2 = 0;
int photoSensorValue3 = 0;
int photoSensorValue4 = 0;

// Variables to store values from the Phototransistors
int PhotoSensor_a0 = 0;
int PhotoSensor_a1 = 0;
int PhotoSensor_a2 = 0;
int PhotoSensor_a3 = 0;

// Different thresholds for different sensors
const float pH_Lthreshold = 6.4;   // Lower Threshold of the pH Sensor
const float pH_Hthreshold = 7.3;   // Higher Threshold of the pH Sensor
const float EC_threshold = 1.2;    //
const float Lum_threshold = 250;   // Phototransistor Threshold
const float temp_threshold = 25.0; // Temperature Sensor Threshold

// Pin for the Temperature sensor
#define temp_bus_pin 2
// Analog pin for the pH sensor
#define ph_analog_pin A0
// Total sample size to collect in every loop
#define sampleSize 10
// Creating an instance of the OneWire object, this is used for any device that uses 1-wire bus system, developed by Dallas Semiconductor Corp.
OneWire oneWire(temp_bus_pin);
// Passing the reference of the oneWire object to DallasTemperature
DallasTemperature sensors(&oneWire);
// Creating an array which will be used to store the sample voltage values from the pH sensor
float samples[int(sampleSize)];
// An offset value for the pH sensor determined using the method described in the manufacuterer's website, link: https://wiki.dfrobot.com/PH_meter_SKU__SEN0161_
float calibrationOffset = 0.13;

void setup()
{
  // Starting serial port with baud rate of 9600
  Serial.begin(9600);
  // Configuring the reference voltage for analog inputs as DEFAULT, which is 5V for Arduino mega
  analogReference(DEFAULT);
  // Starting up the temperature sensor library
  sensors.begin();
  // Starting the ADC Module
  eadc.begin();

	// Initialising all the LEDs
  pinMode(LED_base, OUTPUT);
  pinMode(LED_acid, OUTPUT);
  // pinMode(LED_EC, OUTPUT);
  pinMode(LED_Lum, OUTPUT);
  pinMode(LED_temp, OUTPUT);

  //By default all the pumps are turned off / all LEDs are off
  digitalWrite(LED_base, LOW);
  digitalWrite(LED_acid, LOW);
  // digitalWrite(LED_EC, LOW);
  digitalWrite(LED_Lum, LOW);
  digitalWrite(LED_temp, LOW);
}

// Function to convert the analog voltage value to digital from the pH sensor according to the manufacturer's website
float voltageReader()
{
  float sensorVoltage = analogRead(ph_analog_pin) * 5.0 / 1024;
  return sensorVoltage;
}

// Function to store the sample values to the array from the pH sensor
void sampling()
{
  int index = 0;
  while (index < int(sampleSize))
  {
    samples[index] = voltageReader();

    index++;
  }
}

// Function to calculate the final pH value
float phCalculator(float array[])
{
  // Calculating the average from the sample values
  float sum = 0;
  int index;
  for (index = 0; index < int(sampleSize); index++)
  {
    sum = sum + array[index];
  }
  float sampleAvg = float(sum / float(sampleSize));
  // Calculating the final pH value using the formula used in the manufacturer's website
  float ph = sampleAvg * 3.5 + calibrationOffset;
  return ph;
}

// Function to retrieve the temperature value from the temperature sensor
float tempMeasurement()
{
  // Requesting for the temperature value
  sensors.requestTemperatures();
  // Returning the value by index 0, because it is possible to have more than one sensor on the same bus
  return sensors.getTempCByIndex(0);
}

// Function to map sensor analogue readings to current values in units of microampere
int mp(int x)
{ 
  int y;
  y = map(x, 0, 28000, 0, 500);
  return y;
}

// Function to get the values for the Phototransistor, taking the ADC pin number as the argument
int getPhotoSensorValue(int pin) {
	int value = eadc.readADC_SingleEnded(pin);
	int mappedValue = mp(value);
	return mappedValue;
}

void loop()
{
  // Simplified Phototransistor + ADC code
	// Pin 0
	PhotoSensor_a0 = getPhotoSensorValue(0);
	int Lum_value = PhotoSensor_a0;
	Serial.print("A0:");
  Serial.print(PhotoSensor_a0); // Prints the values coming from the sensor on the screen
  Serial.print(" microAmp; ");
	// Pin 1
	PhotoSensor_a1 = getPhotoSensorValue(1);
	Serial.print("A1:");
  Serial.print(PhotoSensor_a1); // Prints the values coming from the sensor on the screen
  Serial.print(" microAmp; ");
	// Pin 2
	PhotoSensor_a2 = getPhotoSensorValue(2);
	Serial.print("A2:");
  Serial.print(PhotoSensor_a2); // Prints the values coming from the sensor on the screen
  Serial.print(" microAmp; ");
	// Pin 3
	PhotoSensor_a3 = getPhotoSensorValue(3);
	Serial.print("A3:");
  Serial.print(PhotoSensor_a3); // Prints the values coming from the sensor on the screen
  Serial.print(" microAmp; ");
	
	// Phototransistor + ADC code
  // photoSensorValue = eadc.readADC_SingleEnded(0); // Reading the value of the Phototransistor connected to the ADC Pin 0
  // Serial.println(photoSensorValue);
  // int a0;
  // a0 = mp(photoSensorValue);	// Final mapped value from the Phototransistor
  // int Lum_value = a0;
  // Serial.print("A0:");
  // Serial.print(a0); // Prints the values coming from the sensor on the screen
  // Serial.print(" microAmp; ");

  // photoSensorValue2 = eadc.readADC_SingleEnded(1); // Reading the value of the Phototransistor connected to the ADC Pin 1 (Currently unused)
  // int a1;
  // a1 = mp(photoSensorValue2);	// Final mapped value from the Phototransistor
  // Serial.print("A1:");
  // Serial.print(a1);
  // Serial.print(" microAmp; ");

  // photoSensorValue3 = eadc.readADC_SingleEnded(2); // Reading the value of the Phototransistor connected to the ADC Pin 2 (Currently unused)
  // int a2;
  // a2 = mp(photoSensorValue3);	// Final mapped value from the Phototransistor
  // Serial.print("A2:");
  // Serial.print(a2);
  // Serial.print(" microAmp; ");

  // photoSensorValue4 = eadc.readADC_SingleEnded(3); // Reading the value of the Phototransistor connected to the ADC Pin 3 (Currently unused)
  // int a3;
  // a3 = mp(photoSensorValue4);	// Final mapped value from the Phototransistor
  // Serial.print("A3:");
  // Serial.print(a3);
  // Serial.println(" microAmp");

	// pH sensor code
  sampling();
  float phValue = phCalculator(samples);
  float tempValue = tempMeasurement();
  Serial.print("pH = ");

  Serial.print(phValue);
  Serial.print(", Temp = ");
  Serial.print(tempValue);
  Serial.println("*C");

  // LED Code
	// pH Sensor
  if (phValue < pH_Lthreshold)
  {
    digitalWrite(LED_base, HIGH); // Pump/LED is turned on. If pH < 6.5 the solution is too acidic, a base needs to be pumped in
    digitalWrite(LED_acid, LOW);  // the acid pump is turned off
  }
  else if (phValue > pH_Hthreshold)
  {
    digitalWrite(LED_base, LOW);  // the base pump is turned off
    digitalWrite(LED_acid, HIGH); // Pump/LED is turned on. If pH > 7.5 the solution is too alkaline, an acid needs to be pumped in
  }
  else
  {
    digitalWrite(LED_base, LOW); // if  6.5 < pH > 7.5  , pump is turned off
    digitalWrite(LED_acid, LOW); // if  6.5 < pH > 7.5  , pump is turned off
  }

	// Unused code of Electrical conductivity
  //if (EC_value < EC_threshold) {
  //digitalWrite(LED_EC, HIGH); // if electical conductivity < 1.2, the nutrient solution pump is switched on
  //}
  //else {
  //digitalWrite(LED_EC, LOW); // if electical conductivity > 1.2,the nutrient solution pump is switched off
  //}

	// Phototransistor
  if (Lum_value < Lum_threshold)
  {
    digitalWrite(LED_Lum, HIGH); // if Luminosity < specified threshold, the LED is switched on
  }
  else
  {
    digitalWrite(LED_Lum, LOW); //  if Luminosity > specified threshold,the LED is switched off
  }

	// Temperature sensor
  if (tempValue > temp_threshold)
  {
    digitalWrite(LED_temp, HIGH); // if temperature > specified threshold, the LED is switched on
  }
  else
  {
    digitalWrite(LED_temp, LOW); // if temperature < specified threshold, the LED is switched on
  }

  delay(1000);
}
