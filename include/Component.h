#ifndef COMPONENT_H
#define COMPONENT_H

#include <Arduino.h>

#include "CommandParser.h"

class Component {
public:
    virtual ~Component() = default;

    // Retourne les mots-clés associés à ce composant
    virtual String getKeywords() const = 0;

    // Traite une commande spécifique
    virtual bool processCommand(const CommandParser::ParsedCommand& command) = 0;
};

#endif // COMPONENT_H