#ifndef PAIRING_H
#define PAIRING_H

#include <PolarRobotics.h>
#include <builtInLED.h> // pairing routine flashes LED to signify stages of pairing

// External Includes
#include <map>
#include <cstring>
#include <BluetoothSerial.h>
#include <Preferences.h> // to store address of controller on flash

// PS5 Controller Library
#include <ps5Controller.h>

#define DEFAULT_BT_DISCOVER_TIME 15000

bool addressIsController(const char *addrCharPtr);
bool startDiscovery();
void storeAddress(const char *addr, bool clear);
void getAddress(const char *&addr);
void activatePairing(bool doRePair = true, int discoverTime = DEFAULT_BT_DISCOVER_TIME);

#endif // PAIRING_H