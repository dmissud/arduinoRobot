// RobotSystem.h
#ifndef ROBOT_SYSTEM_H
#define ROBOT_SYSTEM_H

#include <Adafruit_PWMServoDriver.h>
#include "Application.h"

class RobotSystem
{
private:
    // Variables membres
    Application m_app;
    Adafruit_PWMServoDriver m_pwm;

    // Constructeur privé pour empêcher l'instanciation directe
    RobotSystem();

public:
    // Interdire la copie et l'assignation
    RobotSystem(const RobotSystem&) = delete;
    RobotSystem& operator=(const RobotSystem&) = delete;

    // Méthode statique pour accéder à l'instance unique
    static RobotSystem& getInstance();

    void setup();
    void processCommands();
};

#endif // ROBOT_SYSTEM_H
