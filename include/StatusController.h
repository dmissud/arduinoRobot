//
// Created by daniel on 29/05/25.
//

#ifndef STATUS_CONTROLLER_H
#define STATUS_CONTROLLER_H

#include <Arduino.h>
#include "../src/CommandParser.h"
#include "Component.h"

class StatusController final : public Component{
public:
    StatusController();

    // Mots-clés associés à ce composant
    String getKeywords() const override;
    bool processCommand(const CommandParser::ParsedCommand& command) override;

private:
    static void sendReadyStatus();
};

#endif // STATUS_CONTROLLER_H