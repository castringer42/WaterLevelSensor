
//const char *MQTT_HOST= "io.adafruit.com";
//const int MQTT_PORT= 8883;
//const char *MQTT_USER= "castringer42";
//const char *MQTT_PASS= "641ad96c95474db7b377177fb3f70b18";
//const char *MQTT_ID= "castringer42/feeds/729246";

// This #include statement was automatically added by the Particle IDE.
//#include <MQTT.h>

// This #include statement was automatically added by the Particle IDE.
#include "UltrasonicSensor.h"
#include "StatsTools.h"
#include "StatsArray.h"

SYSTEM_MODE(AUTOMATIC);
//SYSTEM_MODE(MANUAL);
STARTUP(cellular_credentials_set("RESELLER", "", "", NULL));
//STARTUP(System.enableFeature(FEATURE_RETAINED_MEMORY)); // Enable Backup RAM

UltrasonicSensor cheapSensor(UltrasonicSensor::SensorStyle::FourPin, B0, B1); // Rx, Tx
UltrasonicSensor wpSensor(UltrasonicSensor::SensorStyle::FourPin, B3, B2); // Rx, Tx
UltrasonicSensor mbSensor(UltrasonicSensor::SensorStyle::Analog, A0); // Rx, Tx

StatsTools::StatsArray<float> vals(10); // max 10 elements

//MQTT mq;

const int transmitNotifierPin = B4;
int txCount = 0;

float GetTrialAverage(UltrasonicSensor &sensor, int numTrials)
{
    vals.Clear();
    for (int i = 0; i < numTrials; ++i)
    {
        vals.Add(sensor.GetDistance_cm());
        delay(100);
    }
    vals.TrimOutliers();

    return vals.Average();
}

int Reading(String str)
{
    float cheapDist_cm = GetTrialAverage(cheapSensor, 10);
    Serial.print("Cheap: ");Serial.println(cheapDist_cm/2.54);
    delay(500);

    float wpDist_cm = GetTrialAverage(wpSensor, 10);
    Serial.print("WP: ");Serial.println(wpDist_cm/2.54);
    delay(500);

    float mbDist_cm = GetTrialAverage(mbSensor, 10);
    Serial.print("MB: ");Serial.println(mbDist_cm/2.54);
    delay(500);

    digitalWrite(transmitNotifierPin, HIGH);

    //mq.publish("castringer42/f/waterproofsensor", String(cheapDist_cm).c_str());

    Particle.publish("cheap", String(cheapDist_cm), PRIVATE);
    Particle.publish("waterproof", String(wpDist_cm), PRIVATE);
    Particle.publish("maxbotix", String(mbDist_cm), PRIVATE);
    delay(10);
    digitalWrite(transmitNotifierPin, LOW);

    return 0;
}

void setup()
{
    Serial.begin(115200);

    Particle.function("ForceReading", Reading);
    Particle.variable("TX_Count", txCount);

    Particle.keepAlive(30);

    //mq.setBroker("io.adafruit.com", 8883);
    //mq.connect("castringer42/feeds/729246", "castringer42", "641ad96c95474db7b377177fb3f70b18");
    //mq.setBroker(MQTT_HOST, MQTT_PORT);
    //mq.connect(MQTT_ID, MQTT_USER, MQTT_PASS);

    mbSensor.SetAnalogRef_V(3.3);
    mbSensor.SetAnalogScale_cmpV(120*2.54); //10ft per volt
    mbSensor.SetAnalogDivs(4095);

    pinMode(transmitNotifierPin, OUTPUT);
    digitalWrite(transmitNotifierPin, LOW);

    //Cellular.off(); // REMEMBER THAT I AM HERE
    //RGB.control(true);
    //RGB.color(0, 0, 0);
}

void loop()
{
    unsigned long start = millis();
    Reading("");
    txCount++;

    delay(10*60*1000 - (unsigned long)(millis() - start));
}
