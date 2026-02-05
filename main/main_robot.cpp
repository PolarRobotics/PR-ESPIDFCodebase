/**
 * @brief Main Code File
 *
 * The place where the magic happens.
 * This code is what's run by default by the ESP32.
 * `setup()` is called once upon startup. Initialization and preparation occur here.
 * `loop()` is called infinitely after `setup()` completes. Most real-time logic is here.
 **/

#include <Arduino.h>
#include <ps5Controller.h> // ESP PS5 library, access using global instance `ps5`

// my dumb code

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Debounce for Tacke Sensor Switch
// 50 ms for default delay (50L)
#define TACKLE_SENSOR_DELAY 50L

#include <PolarRobotics.h>

// Drive Includes
#include <Drive.h>

// Pairing Includes
#include <pairing.h>

// Robot Includes
#include <Robot.h>
#include <Lineman.h>
#include <Center.h>
#include <Kicker.h>
#include <Quarterback.h>
#include <QuarterbackBase.h>
#include <QuarterbackTurret.h>

// Types Includes
#include <BotTypes.h>

// Utilities Includes
#include <ConfigManager.h>

// Sabertooth USB Serial Library
#include <sabertoothinst.h>

// Primary Parent Component Pointers
Robot *robot = nullptr; // subclassed if needed
Drive *drive = nullptr; // subclassed if needed

//* How to use subclasses: ((SubclassName*) robot)->function()
//! You must downcast each time you use a special function

// Robot Information from EEPROM/Preferences
BotType robotType;
drive_param_t driveParams;

// Config
ConfigManager config;

// Input Debouncer
Debouncer *dbOptions;

// Prototypes for Controller Callbacks
// Implementations located at the bottom of this file
void onConnection()
{
  if (ps5.isConnected())
  {
    Serial.println(F("Controller Connected."));
    // ps5.setLed(0, 255, 0);   // set LED green
  }

  // TODO: perm sln
  if (robotType != quarterback_turret)
  {
    drive->emergencyStop();
  }
  else
  {
    ((QuarterbackTurret *)robot)->emergencyStop();
  }
}

void onDisconnect()
{
  Serial.println(F("Controller Disconnected."));

  // TODO: perm sln
  if (robotType != quarterback_turret)
  {
    drive->emergencyStop();
  }
  else
  {
    ((QuarterbackTurret *)robot)->emergencyStop();
  }
}

extern "C" void main_app(void)
{
  initArduino();
  /*
     ____    _____   _____   _   _   ____
    / ___|  | ____| |_   _| | | | | |  _ \
    \___ \  |  _|     | |   | | | | | |_) |
     ___) | | |___    | |   | |_| | |  __/
    |____/  |_____|   |_|    \___/  |_|

  */
  bool sabertoothReady = false;

  // runs once at the start of the program

  // Arduino-like setup()
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TACKLE_PIN, OUTPUT); // Try INPUT_PULLUP
  digitalWrite(TACKLE_PIN, 0); // Initially sets tackle sensor to home

  // Initialize debouncer
  dbOptions = new Debouncer(TACKLE_SENSOR_DELAY);

  // Read robot info from "EEPROM" (ESP32 Preferences) using ConfigManager
  config.read();
  Serial.println(config.toString());
  robotType = config.getBotType();
  driveParams = config.getDriveParams();

  // Drive motor identifiers:
  // - Packet Serial robots: use Sabertooth motor indices (M1_IDX/M2_IDX)
  // - PWM robots: use GPIO pins (M1_PWM/M2_PWM)
  const bool driveUsesPwmPins = (MOTORTYPE_INTERFACE_ARRAY[driveParams.motor_type] == pwm);
  const uint8_t DRIVE_M1 = driveUsesPwmPins ? M1_PWM : M1_IDX;
  const uint8_t DRIVE_M2 = driveUsesPwmPins ? M2_PWM : M2_IDX;

  // work backwards from highest ordinal enum since lineman should be default case
  switch (robotType)
  {
  //* Each case should have the following:
  // An initialization of `robot` as a new Robot subclass
  // An initialization of `drive` as a new Drive subclass
  // A call to drive->setupMotors (or downcast and call to override)
  case kicker:
    robot = new Kicker(SPECBOT_PIN1, SPECBOT_PIN2, ENC1_CHA, ENC1_CHB);
    drive = new Drive(kicker, driveParams);
    drive->setupMotors(DRIVE_M1, DRIVE_M2);
    break;
  case quarterback_old:
    robot = new Quarterback(SPECBOT_PIN1, SPECBOT_PIN2, SPECBOT_PIN3);
    drive = new Drive(quarterback_old, driveParams);
    drive->setupMotors(DRIVE_M1, DRIVE_M2);
    break;
  case center:
    robot = new Center(SPECBOT_PIN1, SPECBOT_PIN2);
    drive = new Drive(center, driveParams);
    drive->setupMotors(DRIVE_M1, DRIVE_M2);
    break;
  case runningback:
    robot = new Lineman();
    drive = new Drive(runningback, driveParams);
    drive->setupMotors(DRIVE_M1, DRIVE_M2);
    break;
  case quarterback_turret:
    robot = new QuarterbackTurret(
        M1_IDX,       // left flywheel
        M2_IDX,       // right flywheel
        M3_PIN,       // cradle
        M4_PIN,       // turret
        SPECBOT_PIN1, // assembly motor
        SPECBOT_PIN3, // magnetometer sda
        SPECBOT_PIN4, // magnetometer scl
        ENC1_CHA,     // turret encoder
        ENC1_CHB,     // turret encoder
        ENC2_CHB      // zeroing laser
    );
    break;
  case quarterback_base:
    drive = new Drive(quarterback_base, driveParams);
    drive->setupMotors(DRIVE_M1, DRIVE_M2);
    robot = new QuarterbackBase(drive);
    break;
  case receiver:
  case lineman:
  default: // Assume lineman
    robot = new Lineman();
    String debugMsg = "01: Instantiating Drive Class\n";
    Serial.print(debugMsg.c_str());
    drive = new Drive(lineman, driveParams);
    String debugMsg2 = "03: Call setupMotors\n";
    Serial.print(debugMsg2.c_str());
    drive->setupMotors(DRIVE_M1, DRIVE_M2);
  }

  drive->printSetup();

  //! Activate Pairing Process: this code is BLOCKING, not instantaneous
  activatePairing();

  ps5.attachOnConnect(onConnection);
  ps5.attachOnDisconnect(onDisconnect);
  HWSerial.begin(115200, SERIAL_8N1, 16, 17); // 9600 baudrate default for USBSabertooth
  {
    ; // wait for serial port to connect
  }

  /*
     __  __      _      ___   _   _     _        ___     ___    ____
    |  \/  |    / \    |_ _| | \ | |   | |      / _ \   / _ \  |  _ \
    | |\/| |   / _ \    | |  |  \| |   | |     | | | | | | | | | |_) |
    | |  | |  / ___ \   | |  | |\  |   | |___  | |_| | | |_| | |  __/
    |_|  |_| /_/   \_\ |___| |_| \_|   |_____|  \___/   \___/  |_|

  */

  // runs continuously after Arduino-like setup. controls driving and any special robot functionality during a game

  // Arduino-like loop()
  while (true)
  {

    if (ps5.isConnected())
    {
      // Serial.print(F("\r\nConnected"));
      // ps5.setLed(255, 0, 0);   // set LED red

      //* QBv3 Turret doesn't have drive, so this is a temporary measure to avoid NPEs and chaos
      // TODO: find better solution
      if (robotType != quarterback_turret)
      {
        drive->setStickPwr(ps5.LStickY(), ps5.RStickX());

        // determine BSN percentage (boost, slow, or normal)
        if (ps5.Touchpad())
        {
          drive->emergencyStop();
          drive->setSpeedScalar(Drive::BRAKE);
        }
        else if (ps5.R1())
        {
          drive->setSpeedScalar(Drive::BOOST);
          // ps5.setLed(0, 255, 0);   // set LED red
        }
        else if (ps5.L1())
        {
          drive->setSpeedScalar(Drive::SLOW);
        }
        else if (ps5.R2() && driveParams.motor_type == falcon)
        {
          // used to calibrate the max pwm signal for the falcon 500 motors
          drive->setSpeedValue(FALCON_CALIBRATION_FACTOR);
        }
        else
        {
          drive->setSpeedScalar(Drive::NORMAL);
        }

        // Manual Home / Away Position Setting
        if (dbOptions->debounceAndPressed(ps5.Options()))
        {
          switchTackleSensor();
        }

        //* Update the motors based on the inputs from the controller
        //* Can change functionality depending on subclass, like robot.action()
        drive->update();
        drive->printDebugInfo(); // comment this line out to reduce compile time and memory usage
        // drive->printCsvInfo(); // prints info to serial monitor in a csv (comma separated value) format
      }
      //! Performs all special robot actions depending on the instantiated Robot subclass
      robot->action();

      // DEBUGGING:
      // drive->printDebugInfo(); // comment this line out to reduce compile time and memory usage
      // drive->printCsvInfo(); // prints info to serial monitor in a csv (comma separated value) format

      delay(5);
    }
    else
    { // no response from PS5 controller within last 300 ms, so stop
      if (robotType != quarterback_turret)
      {
        // Emergency stop if the controller disconnects
        drive->emergencyStop();
      }
      else
      {
        ((QuarterbackTurret *)robot)->emergencyStop();
      }
    }
  }

  /**
   * @brief onConnection: Function to be called on controller connect
   */

  /**
   * @brief onDisconnect: Function to be called on controller disconnect
   * Stops bots from driving off and ramming into a wall or someone's foot if they disconnect
   */

  // WARNING: if program reaches end of function app_main() the MCU will restart.
}