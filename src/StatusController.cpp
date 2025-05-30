//
// Created by daniel on 29/05/25.
//

#include "StatusController.h"

StatusController::StatusController() {
    // Constructeur par défaut
}

String StatusController::getKeywords() const
{
    return "status";
}

bool StatusController::processCommand(const CommandParser::ParsedCommand& command) {
    // Vérifier si la commande est "status" sans arguments
    if ((command.name == "arduino") && (command.action == "ok")) {
        sendReadyStatus();
        return true;
    }

    return false;
}

void StatusController::sendReadyStatus() {
    Serial.println("ready");
}