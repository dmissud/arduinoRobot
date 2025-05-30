#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "Application.h"
#include "CommandParser.h"
#include "LEDController.h"
#include "ServoController.h"
#include "StatusController.h"

Application app;
Adafruit_PWMServoDriver pwm;

void setup()
{
    Serial.begin(9600);
    Wire.begin();
    pwm = Adafruit_PWMServoDriver(); // Initialisation I2C
    pwm.begin(); // Initialisation du PCA9685
    pwm.setPWMFreq(50); // Fréquence PWM standard

    // Créer plusieurs Controllers
    app.addComponent(new StatusController());
    app.addComponent(new LEDController(12, "led"));
    app.addComponent(new ServoController(pwm, 0, "base"));
    app.addComponent(new ServoController(pwm, 1, "s1"));
}

void loop()
{
    if (Serial.available())
    {
        String command = Serial.readStringUntil('\n');
        command.trim();

        // Option 1 : Utilisation directe
        const CommandParser::ParsedCommand parsedCmd = CommandParser::parse(command);
        if (parsedCmd.isValid)
        {
            // Afficher les détails de la commande (optionnel)
            // CommandParser::printParsedCommand(parsedCmd);

            // Passer la commande à l'application
            app.run(parsedCmd);
        }
        else
        {
            Serial.println("Commande invalide : Non executée : " + command);
        }
    }
}
