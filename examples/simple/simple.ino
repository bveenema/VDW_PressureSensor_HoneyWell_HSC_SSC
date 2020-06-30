/*
 * Project DAF021_ECM_3G-Boron
 * Description: Primary controller for DAF021_ECM_3G
 * Author: Veenema Design Works
 */

#include "Particle.h"
#include "VDW_PressureSensor_HoneyWell_HSC_SSC.h"

// Cloud not needed for this example
SYSTEM_MODE(MANUAL);

// Create Pressure Sensor
VDW_PressureSensor_Honeywell_HSC_SSC PressureSensor; // Default sample QTY is 100 for rolling average
// VDW_PressureSensor_Honeywell_HSC_SSC PressureSensor2(1000); // Alt Constructor specifying 1000 sample rolling average

void setup()
{
    // Initialize Serial
    Serial.begin(57000);
    delay(1000);

    /// Initialize Pressure Sensor
    // Pin: A1
    // Pressure Range: 5000 mPSI
    // Min Pressure: 0 mPSI
    // Voltage Supply: 3300 mV
    // Sample Rate: 10 ms
    PressureSensor.init(A1, 5000, 0, 3300, 10);

    // Allow Time for Pressure Sensor to settle
    uint InitialTime = millis();
    while(millis() - InitialTime < 500)
    {
        PressureSensor.update();
    }

    // Zero the pressure
    PressureSensor.zero();
}

void loop()
{
    // update must be called every loop to gather samples
    PressureSensor.update();

    // Print Instantaneous and Rolling Average Pressure every 1000 ms
    static uint LastUpdate = 0;
    if(millis() - LastUpdate > 1000)
    {
        LastUpdate = millis();

        int32_t Pressure_Average = PressureSensor.getPressureAverage();
        int32_t Pressure_Instant = PressureSensor.getPressureInstant();

        Serial.printlnf("Average Pressure: %i, Instant Pressure: %i", Pressure_Average, Pressure_Instant);
    }
}