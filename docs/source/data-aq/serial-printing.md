# Comma Separated Values
## Overview
This guide will show you how to correctly print headers and values to serial so that the `SerialMonitor.py` program will print it correctly to a `.csv` file. 


## Instructions
1. Your data should follow a comma-separated values (`.csv`) format, meaning that each header and value should be separated with commas. 

2. Under the `dev/data-aq` branch of the `ESP32PRCodebase` repository, you can find the following `printCsvInfo` function on lines 384-400 of `Drive.cpp`. This is an example of how to print basic information. This function can be changed however you like.

```cpp
/**
* @brief prints the internal variables to the serial monitor in a csv format
* this function is important for data acquisition
* @author Corbin Hibler
* Updated: 2023-10-30
*/
void Drive::printCsvInfo() {
	Serial.print(F("L_HAT_Y,"));
	Serial.print(stickForwardRev);
	Serial.print(F(",R_HAT_X,"));
	Serial.print(stickTurn);
	Serial.print(F(",Turn,"));
	Serial.print(lastTurnPwr);
	Serial.print(F(",Left Motor,"));
	Serial.print(requestedMotorPower[0]);
	Serial.print(F(",Right Motor,"));
	Serial.println(requestedMotorPower[1]);
}
```

3. Put simply, it should look something like this:

```cpp
Serial.print(F("Awesome Data 1,")); // First header does not need to start with comma, but it must end with one
Serial.print(dataVariable1); // Print the first variable you want to record
Serial.print(F(",Awesome Data 2,")); // All subsequent headers require commas on both sides
Serial.print(dataVariable2); // Print the second variable you want to record
Serial.print(F(",Awesome Data 3,")); // Repeat this format for all data points you need
Serial.println(dataVariable3); // Finish with a data variable, make sure it is a Serial.println function.
```

4. All data should be on **ONE LINE** in serial. This means you should only have a single `Serial.println()` statement at the end of your data.  Our `serial_monitor.py` program running on the Raspberry Pi will now be able to read and transcribe this information to a `.csv` file, with appropriate headers and values.