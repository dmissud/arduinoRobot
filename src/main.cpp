#include <Arduino.h>
#include "RobotSystem.h"


void setup()
{
    RobotSystem::getInstance().setup();
}

void loop()
{
    RobotSystem::getInstance().processCommands();
}