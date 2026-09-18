#pragma once
// #define LOOPBACK
#define debug 1

#include <HardwareSerial.h>
#include <SPI.h>
#include <inverseKin.h>
#include <mcp2515.h>

#define SPI_CS_PIN 5
#define HALT_BUTTON 4

extern MCP2515 mcp2515;
extern MCP2515::ERROR err;
extern uint8_t motors[8];

// Parameter Set Packet
extern struct can_frame param_set;
#define param_set_id 0x205C000
#define param_set_dlc 5
extern uint8_t set_break_mode[5];
extern uint8_t set_coast_mode[5];

// Control packets to send (info obtained by datasheet in
// non-frc-spark-example/)
extern struct can_frame heartbeat;
#define heartbeat_id 0x2052C80
#define heartbeat_dlc 8
extern uint8_t heartbeat_data[8];

extern struct can_frame voltage_set;  // -14V to 14V
#define voltage_set_id 0x2051080
#define voltage_set_dlc 4

extern struct can_frame velocity_set;  // -1000 to 1000
#define velocity_set_id 0x2050480
#define velocity_set_dlc 8

extern struct can_frame position_set;  // 0 to 360
#define position_set_id 0x2050C80
#define position_set_dlc 8

extern struct can_frame halt;  // No data field
#define halt_id 0x0000040
#define halt_dlc 0

extern struct can_frame status_frame_1;  // Contains voltage data
#define status_frame_1_id 0x2051840

enum endianness
{
  little,
  big
};

/// @brief Initializes the CAN driver by resetting the MCP2515, setting the
/// bitrate, and setting the mode (loopback for testing, normal for actual use).
void initalizeCANDriver();

/// @brief Sets up a CAN packet with the given parameters
/// @param packet Pointer to the CAN packet to set up
/// @param packet_id The ID to assign to the packet
/// @param target_id The target ID for the packet
/// @param dlc The data length code (number of bytes in the data field)
/// @param data Pointer to the data to include in the packet
void setupPacket(
    can_frame *packet,
    uint32_t packet_id,
    uint32_t target_id,
    uint8_t dlc,
    uint8_t *data);

/// @brief Helper function to pack a float into an 8-byte array in either
/// big-endian or little-endian format, with the remaining bytes set to 0
void packFloatIntoArray(double value, uint8_t *data, endianness endianness);

/// @brief Creates a CAN data packet from wheel data
/// @param data The wheel data to include in the packet
/// @param can_data Pointer to the CAN data array to populate
void createDataCAN(WheelData data, uint8_t *can_data);

/// @brief Updates the motor controllers with new velocity and position data
/// @param data Pointer to the CAN data array containing the latest motor data
void updateMotors(uint8_t *data);

/// @brief Prints an error message based on the MCP2515 error code
/// @param err The error code
/// @param msg The CAN message associated with the error
void printError(MCP2515::ERROR err, can_frame *msg);

/// @brief Updates motor parameters *currently only kIdleMode to change
/// brake/coast mode (for driving motors only) based on a toggle input
void updateParameters(bool toggle);

/// @brief Reads a SPARK Status frame containing voltage data
void readStatusFrame1();

/// @brief Reads a SPARK Status frame containing voltage data
void printPacket(can_frame *msg);