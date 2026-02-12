#include <SerialMotor.h>

#include <PolarRobotics.h>

// Global Packet Serial objects declared in PolarRobotics.h
HardwareSerial HWSerial(2);
USBSabertoothSerial STSerial(HWSerial);
USBSabertooth ST(STSerial, 128);

void SerialMotor::writeRaw(int pwr)
{
    pwr = constrain(pwr, -2047, 2047);
    ST.motor(this->mot_idx, pwr);
}
