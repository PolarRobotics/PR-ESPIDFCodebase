#include <TackleSensor.h>

bool homeOrAway = 0;

// takes location input and swaps it to the other
// @param location: input of 0 or 1 (home is 0, away is 1)
void switchTackleSensor()
{
    if (homeOrAway == 0)
    {
        homeOrAway = 1;
    }
    else
    {
        homeOrAway = 0;
    }

    digitalWrite(TACKLE_PIN, homeOrAway);
}