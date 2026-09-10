#include <QuarterbackBase.h>
#include "esp_log.h"

static const char *TAG = "QuarterbackBase";

// This for some reason has to be declared in the .cpp file and not the .h file so that it does not conflict with the same declaration in other .h files
HardwareSerial Uart_Base(1); // UART2
QuarterbackBase::QuarterbackBase(Drive *drive)
{
  this->drive = drive;

  // Setup digital WiFi pin
  pinMode(WIFI_PIN, OUTPUT);

  // Setup UART Pins
  Uart_Base.begin(115200, SERIAL_8N1, RX2, TX2);
}

void QuarterbackBase::action()
{
  // Send data to wifi ESP
  updateWriteMotorValues();
}

void QuarterbackBase::updateWriteMotorValues()
{
  motor1Value = drive->getMotorWifiValue(0);
  motor2Value = drive->getMotorWifiValue(1);
  UARTMessage = String(motor1Value) + "&" + String(motor2Value);
  UARTMessage = UARTMessage + "~";
  Uart_Base.print(UARTMessage);
  // ESP_LOGI(TAG, "Sent Message To ESP: ");
  ESP_LOGI(TAG, "%s", UARTMessage.c_str());
}