/**
 * @brief Main Code File
 *
 * The place where the magic happens.
 * This code is what's run by default by the ESP32.
 * `setup()` is called once upon startup. Initialization and preparation occur
 *here. `loop()` is called infinitely after `setup()` completes. Most real-time
 *logic is here.
 **/

#include <Arduino.h>
#include <ps5Controller.h>  // ESP PS5 library, access using global instance `ps5`

#include "esp_log.h"

static const char *TAG = "MainRobot";

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
#include <Center.h>
#include <CenterConversion.h>
#include <Kicker.h>
#include <Lineman.h>
#include <Quarterback.h>
#include <QuarterbackBase.h>
#include <QuarterbackTurret.h>
#include <Robot.h>

// Types Includes
#include <BotTypes.h>

// Utilities Includes
#include <ConfigManager.h>
#include <PRConsole.h>
#include <TackleSensor.h>

// Sabertooth USB Serial Library
#include <sabertoothinst.h>

// Primary Parent Component Pointers
Robot *robot = nullptr;  // subclassed if needed
Drive *drive = nullptr;  // subclassed if needed

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
void onConnection();
void onDisconnect();

void linemanCallback();
void runningbackCallback();
void centerCallback();
void centerConversionCallback();
void kickerCallback();
void quarterbackCallback();
void quarterbackBaseCallback();

void onConnection()
{
  if (ps5.isConnected())
  {
    ESP_LOGI(TAG, "Controller Connected.");
    // ps5.setLed(0, 255, 0);   // set LED green
  }

  // TODO: perm sln
  if (robotType != quarterback_turret)
    drive->emergencyStop();
  else
    ((QuarterbackTurret *)robot)->emergencyStop();
}

void onDisconnect()
{
  ESP_LOGI(TAG, "Controller Disconnected.");

  // TODO: perm sln
  if (robotType != quarterback_turret)
    drive->emergencyStop();
  else
    ((QuarterbackTurret *)robot)->emergencyStop();
}

void linemanCallback()
{
  if (drive)
    drive->ps5Callback();
  checkOptionsTackle(dbOptions);
}

void runningbackCallback()
{
  if (drive)
    drive->ps5Callback(/*runningbackBoost=*/true);
  checkOptionsTackle(dbOptions);
}

void centerCallback()
{
  if (drive)
    drive->ps5Callback();
  checkOptionsTackle(dbOptions);
  if (!robot)
    return;

  Center *c = (Center *)robot;

  if (ps5.event.button_down.triangle)
    c->armControl(ArmStatus::HIGHER);
  else if (ps5.event.button_down.cross)
    c->armControl(ArmStatus::LOWER);
  else if (ps5.event.button_down.circle)
    c->armControl(ArmStatus::HOLD);
  if (ps5.event.button_up.triangle || ps5.event.button_up.cross ||
      ps5.event.button_up.circle)
    c->armControl(ArmStatus::STOP_ARM);

  if (ps5.event.button_down.up)
    c->clawControl(ClawStatus::OPEN);
  else if (ps5.event.button_down.down)
    c->clawControl(ClawStatus::CLOSE);
  if (ps5.event.button_up.up || ps5.event.button_up.down)
    c->clawControl(ClawStatus::STOP_CLAW);
}

void centerConversionCallback()
{
  if (drive)
    drive->ps5Callback();
  checkOptionsTackle(dbOptions);
  if (!robot)
    return;

  CenterConversion *cc = (CenterConversion *)robot;

  if (ps5.event.button_down.triangle)
    cc->actuatorControl(true);
  else if (ps5.event.button_down.cross)
    cc->actuatorControl(false);
  if (ps5.event.button_up.triangle || ps5.event.button_up.cross)
    cc->stop();
}

void kickerCallback()
{
  if (drive)
    drive->ps5Callback();
  checkOptionsTackle(dbOptions);
  if (!robot)
    return;

  Kicker *k = (Kicker *)robot;

  if (ps5.event.button_down.triangle)
    k->turnForward();
  else if (ps5.event.button_down.cross)
    k->turnReverse();
  if (ps5.event.button_up.triangle || ps5.event.button_up.cross)
    k->stop();
}

void quarterbackCallback()
{
  if (drive)
    drive->ps5Callback();
  checkOptionsTackle(dbOptions);
  if (!robot)
    return;

  Quarterback *qb = (Quarterback *)robot;

  if (ps5.event.button_down.up)
    qb->aim(QBAim::AIM_UP);
  else if (ps5.event.button_down.down)
    qb->aim(QBAim::AIM_DOWN);

  if (ps5.event.button_down.square)
    qb->toggleConveyor();
  else if (ps5.event.button_down.circle)
    qb->toggleFlywheels();

  if (ps5.event.button_down.triangle)
    qb->changeFWSpeed(SpeedStatus::INCREASE);
  else if (ps5.event.button_down.cross)
    qb->changeFWSpeed(SpeedStatus::DECREASE);
}

void quarterbackBaseCallback()
{
  if (drive)
    drive->ps5Callback();
  checkOptionsTackle(dbOptions);
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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TACKLE_PIN, OUTPUT);  // Try INPUT_PULLUP
  digitalWrite(TACKLE_PIN, 0);  // Initially sets tackle sensor to home

  // Initialize debouncer
  dbOptions = new Debouncer(TACKLE_SENSOR_DELAY);

  // Read robot info from "EEPROM" (ESP32 Preferences) using ConfigManager
  config.read();
  ESP_LOGI(TAG, "%s", config.toString().c_str());
  robotType = config.getBotType();
  driveParams = config.getDriveParams();

  startConsole(config);

  // Drive motor identifiers:
  // - Packet Serial robots: use Sabertooth motor indices (M1_IDX/M2_IDX)
  // - PWM robots: use GPIO pins (M1_PWM/M2_PWM)
  const bool driveUsesPwmPins =
      (MOTORTYPE_INTERFACE_ARRAY[driveParams.motor_type] == pwm);
  const uint8_t DRIVE_M1 = driveUsesPwmPins ? M1_PWM : M1_IDX;
  const uint8_t DRIVE_M2 = driveUsesPwmPins ? M2_PWM : M2_IDX;

  // work backwards from highest ordinal enum since lineman should be default
  // case
  switch (robotType)
  {
    //* Each case should have the following:
    // An initialization of `robot` as a new Robot subclass
    // An initialization of `drive` as a new Drive subclass
    // A call to drive->setupMotors (or downcast and call to override)
    // A ps5.attach() of that robot's event callback (except turret)
    case kicker:
      robot = new Kicker(SPECBOT_PIN1, SPECBOT_PIN2, ENC1_CHA, ENC1_CHB);
      drive = new Drive(kicker, driveParams);
      drive->setupMotors(DRIVE_M1, DRIVE_M2);
      ps5.attach(kickerCallback);
      break;
    case quarterback_old:
      robot = new Quarterback(SPECBOT_PIN1, SPECBOT_PIN2, SPECBOT_PIN3);
      drive = new Drive(quarterback_old, driveParams);
      drive->setupMotors(DRIVE_M1, DRIVE_M2);
      ps5.attach(quarterbackCallback);
      break;
    case center:
      robot = new Center(SPECBOT_PIN1, SPECBOT_PIN2);
      drive = new Drive(center, driveParams);
      drive->setupMotors(DRIVE_M1, DRIVE_M2);
      ps5.attach(centerCallback);
      break;
    case center_conversion:
      robot = new CenterConversion(SPECBOT_PIN1);
      drive = new Drive(center_conversion, driveParams);
      drive->setupMotors(DRIVE_M1, DRIVE_M2);
      ps5.attach(centerConversionCallback);
      break;
    case runningback:
      robot = new Lineman();
      drive = new Drive(runningback, driveParams);
      drive->setupMotors(DRIVE_M1, DRIVE_M2);
      ps5.attach(runningbackCallback);
      break;
    case quarterback_turret:
      robot = new QuarterbackTurret(
        M1_IDX,        // left flywheel
        M2_IDX,        // right flywheel
        M3_PIN,        // cradle
        M4_PIN,        // turret
        SPECBOT_PIN1,  // assembly motor
        SPECBOT_PIN3,  // magnetometer sda
        SPECBOT_PIN4,  // magnetometer scl
        ENC1_CHA,      // turret encoder
        ENC1_CHB,      // turret encoder
        ENC2_CHB       // zeroing laser
      );
      break;
    case quarterback_base:
      drive = new Drive(quarterback_base, driveParams);
      drive->setupMotors(DRIVE_M1, DRIVE_M2);
      robot = new QuarterbackBase(drive);
      ps5.attach(quarterbackBaseCallback);
      break;
    case receiver:
    case lineman:
    default:  // Assume lineman
      robot = new Lineman();
      {
        String debugMsg = "01: Instantiating Drive Class";
        ESP_LOGI(TAG, "%s", debugMsg.c_str());
      }
      drive = new Drive(lineman, driveParams);
      {
        String debugMsg2 = "03: Call setupMotors";
        ESP_LOGI(TAG, "%s", debugMsg2.c_str());
      }
      drive->setupMotors(DRIVE_M1, DRIVE_M2);
      ps5.attach(linemanCallback);
  }

  drive->printSetup();

  //! Activate Pairing Process: this code is BLOCKING, not instantaneous
  activatePairing();

  ESP_LOGI(TAG, "Attaching PS5 callbacks...");
  ps5.attachOnConnect(onConnection);
  ps5.attachOnDisconnect(onDisconnect);

  ESP_LOGI(TAG, "Initializing hardware serial...");
  HWSerial.begin(115200, SERIAL_8N1, 16,
                 17);  // 9600 baudrate default for USBSabertooth
  {
    ;  // wait for serial port to connect
  }

  /*
     __  __      _      ___   _   _     _        ___     ___    ____
    |  \/  |    / \    |_ _| | \ | |   | |      / _ \   / _ \  |  _ \
    | |\/| |   / _ \    | |  |  \| |   | |     | | | | | | | | | |_) |
    | |  | |  / ___ \   | |  | |\  |   | |___  | |_| | | |_| | |  __/
    |_|  |_| /_/   \_\ |___| |_| \_|   |_____|  \___/   \___/  |_|

  */

  // runs continuously after Arduino-like setup. controls driving and any
  // special robot functionality during a game

  // Arduino-like loop()
  while (true)
  {
    if (ps5.isConnected())
    {
      //* QBv3 Turret doesn't have drive, so this is a temporary measure to
      // avoid NPEs and chaos
      // TODO: find better solution
      if (robotType != quarterback_turret)
      {
        // Stick power, BSN, and Options→tackle are handled in ps5.attach callbacks
        drive->update();
      }
      //! Performs all special robot actions depending on the instantiated Robot
      //! subclass (periodic work; button edges handled in callbacks)
      robot->action();
    }
    else
    {  // no response from PS5 controller within last 300 ms, so stop
      // ESP_LOGI(TAG, "Controller disconnected");
      if (robotType != quarterback_turret)
      {
        // Emergency stop if the controller disconnects
        // ESP_LOGI(TAG, "Emergency stop due to controller disconnect.");
        drive->emergencyStop();
      }
      else
      {
        // ESP_LOGI(TAG, "Emergency stop due to controller disconnect for quarterback turret.");
        ((QuarterbackTurret *)robot)->emergencyStop();
      }
    }
    delay(5);
  }

  // WARNING: if program reaches end of function app_main() the MCU will
  // restart.
}
