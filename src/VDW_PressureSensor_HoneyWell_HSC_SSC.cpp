#include "VDW_PressureSensor_HoneyWell_HSC_SSC.h"

void VDW_PressureSensor_Honeywell_HSC_SSC::update(){
    // Check if a read is due
    if(millis() - _lastRead > _sampleRate){
        // increment the sample index
        _index += 1;
        if(_index > _sampleQTY){
            _index = 0;
            _samplesFilled = true;
        }

        // read pressure sensor
        _samples[_index] = analogRead(_pin);
        
        // Reset the timer
        _lastRead = millis();
    }
}

uint32_t VDW_PressureSensor_Honeywell_HSC_SSC::averageSamples(){
    // sum all the samples
    uint32_t sumOfSamples = 0;
    for(uint32_t i=0; i<_sampleQTY; i++){
        sumOfSamples += _samples[i];
    }
    
    // Calculate the average Sample
    uint32_t average = 0;
    if(_samplesFilled) average = sumOfSamples/_sampleQTY;
    else if(_index > 0) average = sumOfSamples/_index;

    return average;
}

int32_t VDW_PressureSensor_Honeywell_HSC_SSC::getPressureAverage(){
    return pressureInMilliPSI(averageSamples());
}

// returns last pressure reading in milli-psi
int32_t VDW_PressureSensor_Honeywell_HSC_SSC::getPressureInstant(){
    return pressureInMilliPSI(_samples[_index]);
}

int32_t VDW_PressureSensor_Honeywell_HSC_SSC::pressureInMilliPSI(uint32_t rawADC){
    // Convert ADC Raw to milli-volts
    int32_t milliVolts = rawADC*3300/4095 + _offset;

    // Convert to milli-PSI
    return _pMin + ((10*_deltaP*(milliVolts - (_vSupply/10)))/(8*_vSupply));
}

void VDW_PressureSensor_Honeywell_HSC_SSC::zero(){
    // ideal voltage at 0 PSI
    int32_t idealVoltage = ((0.8 * _vSupply)/_deltaP)*(-_pMin)+(.1 * _vSupply);
    Serial.printlnf("Ideal Voltage: %d", idealVoltage);

    // Calculate the current actual voltage;
    int32_t milliVolts = averageSamples() * 3300 / 4095;
    Serial.printlnf("Current Voltage: %d", milliVolts);

    _offset = idealVoltage - milliVolts;
    Serial.printlnf("Offset: %d",_offset);
}