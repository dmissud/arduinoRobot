#include "Application.h"

#include "CommandParser.h"

Application::Application() :
    m_componentCount(0)
{
}

bool Application::addComponent(Component* component)
{
    if (m_componentCount >= MAX_COMPONENTS)
    {
        return false;
    }

    // Ajouter le composant
    m_components[m_componentCount++] = component;

    return true;
}

void Application::run(const CommandParser::ParsedCommand& command) const
{
    // Chercher les composants correspondant au mot-clé
    bool commandProcessed = false;

    for (int i = 0; i < m_componentCount; i++)
    {
        if (m_components[i]->getKeywords() == command.keyword && m_components[i]->processCommand(command))
        {
            commandProcessed = true;
            break;
        }
    }

    if (!commandProcessed)
    {
        Serial.println("Commande non reconnue");
    }
}
