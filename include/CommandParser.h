#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <Arduino.h>

class CommandParser
{
public:
    struct ParsedCommand
    {
        String keyword;
        String name;
        String action;
        bool isValid = false;
    };

    static ParsedCommand parse(const String& command)
    {
        ParsedCommand result;
        result.isValid = false;

        // Trouver l'index de l'ouverture de parenthèse
        int openParenIndex = command.indexOf('(');
        if (openParenIndex == -1)
        {
            return result;
        }

        // Extraire le keyword (tout ce qui est avant la parenthèse)
        result.keyword = command.substring(0, openParenIndex);
        result.keyword.trim();

        // Vérifier que la commande se termine par une parenthèse fermante
        if (!command.endsWith(")"))
        {
            return result;
        }

        // Extraire le contenu entre parenthèses
        String innerContent = command.substring(openParenIndex + 1, command.length() - 1);
        innerContent.trim();

        // Trouver la virgule
        int commaIndex = innerContent.indexOf(',');
        if (commaIndex == -1)
        {
            return result;
        }

        // Extraire le nom et l'action
        result.name = innerContent.substring(0, commaIndex);
        result.action = innerContent.substring(commaIndex + 1);

        // Nettoyer les espaces
        result.name.trim();
        result.action.trim();

        // Valider que tous les champs sont non vides
        result.isValid = result.name.length() > 0;

        return result;
    }

    // Méthode de débogage pour afficher le résultat du parsing
    static void printParsedCommand(const ParsedCommand& cmd)
    {
        if (cmd.isValid)
        {
            Serial.println("Commande valide :");
            Serial.println("Keyword: " + cmd.keyword);
            Serial.println("Name: " + cmd.name);
            Serial.println("Action: " + cmd.action);
        }
        else
        {
            Serial.println("Commande invalide");
        }
    }
};

#endif // COMMAND_PARSER_H
