#include "LEDController.h"

LEDController::LEDController(const uint8_t pin, const String& name)
    : m_ledPin(pin), m_name(name)
{
    pinMode(m_ledPin, OUTPUT);
}

String LEDController::getKeywords() const
{
    // Retourne les mots-clés associés à ce composant
    return "led";
}

void LEDController::turnOn() const
{
    digitalWrite(m_ledPin, HIGH);
    Serial.println("led(" + m_name + ", on)");
}

void LEDController::turnOff() const
{
    digitalWrite(m_ledPin, LOW);
    Serial.println("led(" + m_name + ", off)");
}

bool LEDController::processCommand(const CommandParser::ParsedCommand& command)
{
    // Vérifier si le mot-clé correspond
    if (!command.name.startsWith(m_name))
    {
        return false;
    }

    // Traiter l'action
    if (command.action == "on")
    {
        turnOn();
        return true;
    }
    else if (command.action == "off")
    {
        turnOff();
        return true;
    }

    return false;
}
