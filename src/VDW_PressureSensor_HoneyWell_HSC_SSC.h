#ifndef VDW_PRESSURE_SENSOR_HONEYWELL_HSC_SSC
#define VDW_PRESSURE_SENSOR_HONEYWELL_HSC_SSC

#include "Particle.h"

class VDW_PressureSensor_Honeywell_HSC_SSC{
    public:
        // C
        VDW_PressureSensor_Honeywell_HSC_SSC(uint16_t sampleQTY = 100)
            : _sampleQTY(sampleQTY)
            , _samples(new uint16_t[sampleQTY]){
                for(int i=0; i<_sampleQTY; i++){
                    _samples[i] = 0;
                }
        }

        void init(pin_t pin, int32_t deltaP, int32_t pMin, int32_t vSupply, uint32_t sampleRate = 10){
            _pin = pin;
            _sampleRate = sampleRate;
            _deltaP = deltaP;
            _pMin = pMin;
            _vSupply = vSupply;

            // Setup the pin
            pinMode(_pin, INPUT);

            // Reset Read Variables
            _index = 0;
            _lastRead = millis();
        }

        void update();

        // returns rolling average pressure in milli-psi.
        int32_t getPressureAverage();

        // returns last pressure reading in milli-psi
        int32_t getPressureInstant();

        // calculates a voltage offset that sets the current value to 0
        void zero();

        // clears the offset
        void clearOffset();
    private:
        // Pressure Container
        uint16_t _sampleQTY;
        uint16_t* _samples;
        uint8_t _index = 0;
        bool _samplesFilled = false;
        uint32_t _lastRead = 0;

        pin_t _pin = 0;
        uint32_t _sampleRate = 0;
        int32_t _deltaP = 0;
        int32_t _pMin = 0;
        int32_t _vSupply = 0;
        int32_t _offset = 0;

        int32_t pressureInMilliPSI(uint32_t rawADC);
        uint32_t averageSamples();
};

#endif