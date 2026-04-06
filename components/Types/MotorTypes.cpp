#include <MotorTypes.h>

constexpr Pair<MotorType, const char *>
    motorTypeStrings[NUM_MOTOR_TYPES] =
        {
            {big_ampflow, "big_ampflow"},
            {small_ampflow, "small_ampflow"},
            {pancake_ampflow, "pancake_ampflow"},
            {falcon, "falcon"},
            {neo_vortex, "neo_vortex"},
            {small_12v, "small_12v"},
            {big_ampflow_pwm, "big_ampflow_pwm"}};

/**
 * @brief getMotorTypeString returns a string correlating to the passed motor type enum
 * @example for MotorType `falcon` the function returns "falcon"
 * @param type the enum to be converted to a string
 * @return a const char* representing the motor type
 */
const char *getMotorTypeString(MotorType type)
{
  return motorTypeStrings[static_cast<int>(type)].value;
}