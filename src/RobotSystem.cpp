// RobotSystem.cpp
#include "RobotSystem.h"
#include "LEDController.h"
#include "ServoController.h"
#include "StatusController.h"

// Implémentation du Singleton
RobotSystem& RobotSystem::getInstance() {
    // Instance statique locale à la fonction
    // Elle est créée la première fois que getInstance() est appelée
    static RobotSystem instance;
    return instance;
}

RobotSystem::RobotSystem() : m_app(), m_pwm() {
}

void RobotSystem::setup() {
    Serial.begin(9600);
    Wire.begin();
    
    // Initialisation I2C
    m_pwm.begin();
    m_pwm.setPWMFreq(50); // Fréquence PWM standard

    // Créer plusieurs Controllers
    m_app.addComponent(new StatusController());
    m_app.addComponent(new LEDController(12, "led"));
    m_app.addComponent(new ServoController(m_pwm, 0, "base"));
    m_app.addComponent(new ServoController(m_pwm, 1, "elbow"));
    m_app.addComponent(new ServoController(m_pwm, 2, "arm"));
    m_app.addComponent(new ServoController(m_pwm, 3, "claw"));
}

void RobotSystem::processCommands() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        const CommandParser::ParsedCommand parsedCmd = CommandParser::parse(command);
        if (parsedCmd.isValid) {
            // Passer la commande à l'application
            m_app.run(parsedCmd);
        } else {
            Serial.println("Commande invalide : Non executée : " + command);
        }
    }
}
