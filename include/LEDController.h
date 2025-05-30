#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include "Component.h"

class LEDController final : public Component {
private:
    uint8_t m_ledPin;
    String m_name;

public:
    LEDController(const uint8_t pin, const String& name);

    // Mots-clés associés à ce composant
    String getKeywords() const override;

    void turnOn() const;
    void turnOff() const;

    // Traitement de la commande
    bool processCommand(const CommandParser::ParsedCommand& command) override;
};

#endif // LED_CONTROLLER_H