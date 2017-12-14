/////////////////////////////////////////////////
// Tennessee Tech Autonomous Robotics Club     //
// Third Day: Boe Bot Navigation               //
//            19 September, 2017               //
// Prepared by Chris Stringer, 19 Sept, 2017   //
/////////////////////////////////////////////////

#include "Particle.h"
#include "application.h"

#ifndef _ULTRASONIC_SENSOR_H_
#define _ULTRASONIC_SENSOR_H_

    class UltrasonicSensor
    {
    public:
        enum SensorStyle
        {
            FourPin,
            ThreePin,
            Analog
        };

        UltrasonicSensor(SensorStyle style, const int pinRx, const int pinTx = -1);
        virtual ~UltrasonicSensor();

        float GetDistance_cm();

        void SetAnalogScale_cmpV(const float scale)
        {
            analogScale_cmpV_ = scale;
        }

        void SetAnalogDivs(const unsigned int divs)
        {
            analogDivs_ = divs;
        }

        void SetAnalogRef_V(const float voltage)
        {
            analogReference_V_ = voltage;
        }

    private:
        int triggerPin_;
        int receivePin_;
        SensorStyle sensorStyle_;

        float analogScale_cmpV_;
        float analogReference_V_;
        unsigned int analogDivs_;

        void TriggerSensor();

        unsigned int GetReading_us();
        float GetReading_V();
    };

#endif // _ULTRASONIC_SENSOR_H_
