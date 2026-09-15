#include "CAN.h"
/**
 * @brief Implements functions for Non-FRC SPARK Motor Controller Communication using an MCP2515 CAN controller.
 * @author Quentin Osterhage
 * Adapted from: https://github.com/autowp/arduino-mcp2515
 */

// Indexes are 0-7; Driving/Turning motors alternate; hence "if <index> % 2 == 0" (or != 0) can be used for looping logic
uint8_t motors[8] = {0x02, 0x06, 0x01, 0x05, 0x03, 0x07, 0x04, 0x08};

/**
 * @brief
 * Motor Controller Layout
 * TOP: Driving controller ID
 * BOTTOM: Turning controller ID
 *
 *                  ^
 *                  | Fwd
 *       ___________________________
 *      |   ____             ____   |
 *      |  |LF  |           |RF  |  |
 *      |  |0x02|           |0x01|  |
 *      |  |0x06|           |0x05|  |
 *      |  |____|           |____|  |
 *      |           .               |
 *      |   ____             ____   |
 *      |  |LB  |           |RB  |  |
 *      |  |0x03|           |0x04|  |
 *      |  |0x07|           |0x08|  |
 *      |  |____|           |____|  |
 *      |___________________________|
 *
 */

MCP2515 mcp2515(SPI_CS_PIN);
MCP2515::ERROR err;

struct can_frame heartbeat;
struct can_frame halt;

struct can_frame param_set;

struct can_frame velocity_set;
struct can_frame position_set;
struct can_frame voltage_set;
struct can_frame status_frame_1;

uint8_t heartbeat_data[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t set_break_mode[5] = {0x00, 0x00, 0x00, 0x01, 0x06};
uint8_t set_coast_mode[5] = {0x00, 0x00, 0x00, 0x00, 0x06};

void initalizeCANDriver()
{
    mcp2515.reset();
    mcp2515.setBitrate(CAN_1000KBPS, MCP_8MHZ);
#ifdef LOOPBACK
    mcp2515.setLoopbackMode();
#else
    mcp2515.setNormalMode();
#endif
}

void setupPacket(can_frame *packet, uint32_t packet_id, uint32_t target_id, uint8_t dlc, uint8_t *data)
{
    packet->can_id = packet_id + target_id | CAN_EFF_FLAG;
    packet->can_dlc = dlc;
    if (data != NULL)
    {
        for (int i = 0; i < dlc; i++)
        {
            packet->data[i] = data[i];
        }
    }
}

void packFloatIntoArray(double value, uint8_t *data, endianness endianness)
{
    uint32_t bits;
    float float_value = static_cast<float>(value); // Convert double to float for 4 bytes
    // Copies the exact bits of the float
    std::memcpy(&bits, &float_value, sizeof(bits));

    switch (endianness)
    {
    case big:
        // Big-endian (MSB first)
        data[0] = (bits >> 24) & 0xFF;
        data[1] = (bits >> 16) & 0xFF;
        data[2] = (bits >> 8) & 0xFF;
        data[3] = bits & 0xFF;
        break;
    case little:
        // Little-endian (LSB first)
        data[0] = bits & 0xFF;
        data[1] = (bits >> 8) & 0xFF;
        data[2] = (bits >> 16) & 0xFF;
        data[3] = (bits >> 24) & 0xFF;
        break;
    }
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x00;
}

void createDataCAN(WheelData data, uint8_t *can_data)
{
    // Motor B: bytes 0-15
    packFloatIntoArray(data.wheels[0].magnitude, can_data + 0, little);
    packFloatIntoArray(data.wheels[0].angle_degrees, can_data + 8, little);

    // Motor A: bytes 16-31
    packFloatIntoArray(data.wheels[1].magnitude, can_data + 16, little);
    packFloatIntoArray(data.wheels[1].angle_degrees, can_data + 24, little);

    // Motor C: bytes 32-47
    packFloatIntoArray(data.wheels[2].magnitude, can_data + 32, little);
    packFloatIntoArray(data.wheels[2].angle_degrees, can_data + 40, little);

    // Motor D: bytes 48-63
    packFloatIntoArray(data.wheels[3].magnitude, can_data + 48, little);
    packFloatIntoArray(data.wheels[3].angle_degrees, can_data + 56, little);
}

void updateMotors(uint8_t *data)
{
    mcp2515.sendMessage(&heartbeat);
    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            setupPacket(&velocity_set, velocity_set_id, motors[i], velocity_set_dlc, &data[i * 8]);
            err = mcp2515.sendMessage(&velocity_set);
#ifdef debug
            printError(err, &velocity_set);
#endif
        }
        else
        {
            setupPacket(&position_set, position_set_id, motors[i], position_set_dlc, &data[i * 8]);
            err = mcp2515.sendMessage(&position_set);
#ifdef debug
            printError(err, &position_set);
#endif
        }
    }
}

void updateParameters(bool toggle)
{
    mcp2515.sendMessage(&heartbeat);
    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        { // Only update driving motors (A, B, C, D) not turning motors (E, F, G, H)
            if (toggle)
            {
                setupPacket(&param_set, param_set_id, motors[i], param_set_dlc, set_coast_mode);
            }
            else
            {
                setupPacket(&param_set, param_set_id, motors[i], param_set_dlc, set_break_mode);
            }
            err = mcp2515.sendMessage(&param_set);
#ifdef debug
            printError(err, &param_set);
#endif
        }
    }
}

void readStatusFrame1()
{
    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        { // Only read from driving motors (A, B, C, D)
            setupPacket(&status_frame_1, status_frame_1_id, i, 8, NULL);
            err = mcp2515.readMessage(&status_frame_1);
#ifdef debug
            printError(err, &status_frame_1);
            printPacket(&status_frame_1);
#endif
        }
    }
}

void printError(MCP2515::ERROR err, can_frame *msg)
{
    switch (err)
    {
    case MCP2515::ERROR_OK:
        Serial.printf("[Packet Sent]:\nID 0x%X\nData: ", msg->can_id & CAN_EFF_MASK);
        for (int i = 0; i < msg->can_dlc; i++)
        {
            Serial.printf("0x%x ", msg->data[i]);
        }
        Serial.println();
        break;
    case MCP2515::ERROR_FAIL:
        Serial.printf("Failed");
        break;
    case MCP2515::ERROR_ALLTXBUSY:
        Serial.printf("All TX buffers are busy\n");
        initalizeCANDriver();
        break;
    case MCP2515::ERROR_FAILINIT:
        Serial.printf("Failed to initialize\n");
        break;
    case MCP2515::ERROR_FAILTX:
        Serial.printf("Failed to transmit\n");
        break;
    case MCP2515::ERROR_NOMSG:
        Serial.printf("No message received\n");
        break;
    default:
        Serial.println("Unknown error");
    }
}

void printPacket(can_frame *msg)
{
    Serial.printf("ID 0x%X\nData: ", msg->can_id & CAN_EFF_MASK);
    for (int i = 0; i < msg->can_dlc; i++)
    {
        Serial.printf("0x%x ", msg->data[i]);
    }
    Serial.println();
}