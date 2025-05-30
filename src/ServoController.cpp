//
// Created by daniel on 18/05/25.
//

#include "ServoController.h"

ServoController::ServoController(Adafruit_PWMServoDriver& pwm, uint8_t channel, const String& name)
    : m_pwm(pwm), m_channel(channel), m_name(name)
{
    // Initialisation potentielle supplémentaire si nécessaire
}

uint16_t ServoController::angleToPulse(int angle) const {
    // Mappage linéaire de l'angle à la largeur d'impulsion
    return map(
        constrain(angle, MIN_ANGLE, MAX_ANGLE),
        MIN_ANGLE,
        MAX_ANGLE,
        MIN_PULSE,
        MAX_PULSE
    );
}

String ServoController::getKeywords() const {
    // Génère les mots-clés pour ce servo
    return "servo";
}

bool ServoController::processCommand(const CommandParser::ParsedCommand& command) {
    // Vérifier si le mot-clé correspond
    if (!command.name.startsWith(m_name)) {
        return false;
    }

    // Traiter les actions
    if (command.action == "sweep") {
        sweep();
        return true;
    }

    if (command.action.startsWith("angle")) {
        // Extraire la valeur de l'angle
        int angle = command.action.substring(6).toInt();

        if (angle < MIN_ANGLE || angle > MAX_ANGLE)
        {
            Serial.println("Angle invalide");
            return false;
        }
        setAngle(angle);
        Serial.print("Angle positionné à ");
        Serial.print(angle);
        Serial.println(" degrés");

        return true;
    }

    Serial.println("Action invalide");
    return false;
}

void ServoController::setAngle(int angle) const {
    // Assurer que l'angle est dans la plage valide
    angle = constrain(angle, MIN_ANGLE, MAX_ANGLE);

    // Convertir l'angle en impulsion PWM
    uint16_t pulse = angleToPulse(angle);
    Serial.print("Pulse positionné à ");
    Serial.print(pulse);
    Serial.print(" us, ");
    // Positionner le servo
    m_pwm.setPWM(m_channel, 0, pulse);

    Serial.print("Servo ");
    Serial.print(m_name);
    Serial.print(" positionné à ");
    Serial.print(angle);
    Serial.println(" degrés");
}

void ServoController::sweep() const {
    // Balayage du servo de 0 à 180 degrés et retour
    for (int angle = MIN_ANGLE; angle <= MAX_ANGLE; angle += 10) {
        setAngle(angle);
        delay(50);
    }

    for (int angle = MAX_ANGLE; angle >= MIN_ANGLE; angle -= 10) {
        setAngle(angle);
        delay(50);
    }
}
