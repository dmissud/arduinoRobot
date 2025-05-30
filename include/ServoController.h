//
// Created by daniel on 18/05/25.
//
#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Component.h"

class ServoController final : public Component {
private:
    Adafruit_PWMServoDriver& m_pwm;  // Référence au contrôleur PWM
    uint8_t m_channel;               // Canal PWM du servo
    String m_name;                   // Nom du servo

    // Paramètres de configuration du servo
    static const int MIN_PULSE = 100;   // 0.5ms
    static const int MAX_PULSE = 650;  // 2.5ms
    static const int MAX_ANGLE = 180;
    static const int MIN_ANGLE = 0;

    // Convertit l'angle en impulsion PWM
    uint16_t angleToPulse(int angle) const;

public:
    ServoController(Adafruit_PWMServoDriver& pwm, uint8_t channel, const String& name);

    // Méthodes de l'interface Component
    String getKeywords() const override;
    bool processCommand(const CommandParser::ParsedCommand& command) override;

    // Méthodes spécifiques au servo
    void setAngle(int angle) const;
    void sweep() const;
};

#endif // SERVO_CONTROLLER_H