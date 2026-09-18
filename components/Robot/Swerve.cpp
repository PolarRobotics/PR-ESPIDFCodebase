#include <Arduino.h>
#include <HardwareSerial.h>
#include <ps5Controller.h>

// Standard
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include <cstring>

// Motor
#include <CAN.h>

// Drive
#include <inverseKin.h>

// Pairing
#include <pairing.h>


#define SPI_CS_PIN 5
#define HALT_BUTTON 4

// creates a kinematics object which serves to calculate and store all inematics information and equations
InverseKin kinematics;

// 8 Motors reciving 8 byte packets
uint8_t can_data[64];

// For toggling between brake and coast mode on the motors
bool idle_toggle = false;

// For calculating debounce on the triangle button
unsigned long lastTrianglePress = 0;
const unsigned long debounceDelay = 250; // 250 milliseconds

unsigned long lastVoltageCheck = 0;
const unsigned long voltageCheckInterval = 5000; // Check voltage every 5 seconds

/* Input collection function, and bluetooth implementation functions for use with pairing.cpp
 * Implementations located at the bottom of this file
 */
void getInputs();
void onConnection();
void onDisconnect();
bool initBluetooth();
void printDeviceAddress();

void setup()
{
    Serial.begin(115200);
    pinMode(HALT_BUTTON, INPUT_PULLUP);

    initalizeCANDriver();
    setupPacket(&heartbeat, heartbeat_id, 0, heartbeat_dlc, heartbeat_data);
    setupPacket(&halt, halt_id, 0, halt_dlc, NULL);

    initBluetooth();
    printDeviceAddress();
    activatePairing();
    ps5.attachOnConnect(onConnection);
    ps5.attachOnDisconnect(onDisconnect);
}

void loop()
{
    if (ps5.isConnected())
    {
        getInputs();

        // if(ps5.Triangle()){
        //   if (millis() - lastTrianglePress > debounceDelay) {
        //     idle_toggle = !idle_toggle;
        //     updateParameters(idle_toggle);
        //     lastTrianglePress = millis();
        //   }
        // }

        kinematics.getData();
        if (!kinematics.checkHalt())
        {
            createDataCAN(kinematics.getData(), can_data);
            updateMotors(can_data);
        }
        else
        {
            mcp2515.sendMessage(&halt);
            // if(millis() - lastVoltageCheck > voltageCheckInterval){
            //   readStatusFrame1();
            //   lastVoltageCheck = millis();
            // }
        }
        // kinematics.debugPrint();
    }
}

/// @brief Reads the PS5 controller stick inputs and updates the kinematics class with the new values
void getInputs()
{
    if (ps5.R1())
    {
        kinematics.BSN = "boost";
    }
    else if (ps5.L1())
    {
        kinematics.BSN = "slow";
    }
    else
    {
        kinematics.BSN = "normal";
    }

    kinematics.leftStickX = ps5.LStickX();
    kinematics.leftStickY = ps5.LStickY();
    kinematics.rightStickX = ps5.RStickX();

    kinematics.setSpeedScalars();

    kinematics.normalizeInputs();
}

/// @brief Initializes Bluetooth functionality
/// @return True if initialization was successful, false otherwise
bool initBluetooth()
{
    if (!btStart())
    {
        Serial.println("Failed to initialize controller");
        return false;
    }

    if (esp_bluedroid_init() != ESP_OK)
    {
        Serial.println("Failed to initialize bluedroid");
        return false;
    }

    if (esp_bluedroid_enable() != ESP_OK)
    {
        Serial.println("Failed to enable bluedroid");
        return false;
    }
    return true;
}

/// @brief Prints the Bluetooth device address to Serial
void printDeviceAddress()
{
    const uint8_t *point = esp_bt_dev_get_address();

    for (int i = 0; i < 6; i++)
    {

        char str[3];

        sprintf(str, "%02X", (int)point[i]);
        Serial.print(str);

        if (i < 5)
        {
            Serial.print(":");
        }
    }
}

/// @brief Callback function for controller connection
void onConnection()
{
    if (ps5.isConnected())
    {
        Serial.println(F("Controller Connected."));
        ps5.setLed(0, 255, 0); // set LED green
    }
}

/// @brief Callback function for controller disconnection; sends halt command to motors upon controller disconnection
void onDisconnect()
{
    Serial.println(F("Controller Disconnected."));
    mcp2515.sendMessage(&halt);
}
